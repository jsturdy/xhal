#include "xhal/utils/XHALXMLParser.h"
#include "lmdb_cpp_wrapper.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sstream>
#include <fstream>
#include <time.h>
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#include <vector>
#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::string serialize(xhal::utils::Node n)
{
  if (n.mask > 0xffffffff) {
    std::cout << "MASK ERROR" << std::endl;
    exit(1);
  } else {
    return std::to_string((uint32_t)n.real_address)+"|"+n.permission+"|"+std::to_string((uint32_t)n.mask);
  }
}

int main()
{
  xhal::utils::XHALXMLParser * m_parser = new xhal::utils::XHALXMLParser("/mnt/persistent/texas/gem_amc_top.xml");
  try 
  {
    m_parser->setLogLevel(2);
    m_parser->parseXML();
  } catch (...){
    std::cout << "Parsing failed" << std::endl;
    return 1;
  }
  std::cout << "Address table parsed successfully" << std::endl;
  std::unordered_map<std::string,xhal::utils::Node> m_parsed_at;
  m_parsed_at = m_parser->getAllNodes();
  xhal::utils::Node t_node;
  std::string t_key;
  std::string t_value;

  clock_t start, end;
  double cpu_time_used;
  double cpu_time_used_open;
  double cpu_time_used_init;
  
  start = clock();

  auto env = lmdb::env::create();
  env.set_mapsize(1UL * 1024UL * 1024UL * 40UL); /* 40 MiB */
  env.open("./example.mdb", 0, 0664);

  end = clock();
  cpu_time_used_open = ((double) (end - start)) * 1000000/ CLOCKS_PER_SEC;

  lmdb::val key_;
  lmdb::val value_;
  auto wtxn = lmdb::txn::begin(env);
  auto dbi = lmdb::dbi::open(wtxn, nullptr);
   m_parsed_at.erase("top");
  for (auto it:m_parsed_at)
  {
    t_key = it.first;
    t_key = t_key.substr(4);
    //std::cout << "Node name " << t_key <<std::endl;
    t_node = it.second;
    //t_value = std::to_string(t_node.address);
    t_value = serialize(t_node);
    //std::cout << "Node name " << t_key << "\n" << "Serialized " << t_value << std::endl;
    key_.assign(t_key);
    value_.assign(t_value);
    std::cout << "Node name " << key_.data() << " " << "Info " << value_.data() << std::endl;
    dbi.put(wtxn, key_, value_);
  }

  //auto env = lmdb::env::create();
  //env.open("./example.mdb", 0, 0664);

  //auto wtxn = lmdb::txn::begin(env);
  //auto dbi = lmdb::dbi::open(wtxn, nullptr);
  //dbi.put(wtxn, "username", "jhacker");
  //dbi.put(wtxn, "email", "jhacker@example.org");
  //dbi.put(wtxn, "fullname", "J. Random Hacker");
  wtxn.commit();

  /* Fetch key/value pairs in a read-only transaction: */
  //auto rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
  //auto cursor = lmdb::cursor::open(rtxn, dbi);
  //std::string key, value;
  //while (cursor.get(key, value, MDB_NEXT)) {
  //  std::printf("key: '%s', value: '%s'\n", key.c_str(), value.c_str());
  //}
  //cursor.close();
  //rtxn.abort();
  
  std::cout << "========================================================================================================" << std::endl;
  std::cout << "========================================================================================================" << std::endl;
  std::cout << "========================================================================================================" << std::endl;

  start = clock();

  auto rtxn_ = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
  //auto dbi = lmdb::dbi::open(rtxn_, nullptr);
  auto cursor_ = lmdb::cursor::open(rtxn_, dbi);

  end = clock();
  cpu_time_used_init = ((double) (end - start)) * 1000000/ CLOCKS_PER_SEC;
  key_.assign("GEM_AMC.OH.OH2.GEB.VFATS.VFAT0.VCal");
  //lmdb::val key_{"top.GEM_AMC.GEM_SYSTEM.BOARD_ID"};
  bool found;
  start = clock();
  for (int i=0; i<1000; i++)
  {
    //cursor_.find("top.GEM_AMC.OH.OH2.GEB.VFATS.VFAT0.VCal");
    //cursor_.find("top.GEM_AMC.GEM_SYSTEM.BOARD_ID");
    //cursor_.get(key, value, MDB_SET);
    found = dbi.get(rtxn_,key_,value_);
    //std::printf("key: '%s', value: '%s'\n", "top.GEM_AMC.OH.OH2.GEB.VFATS.VFAT0.VCal", value.c_str());
    if (found) {
      std::printf("key: '%s', value: '%s'\n", key_.data(), value_.data());
      //std::printf("key: '%s', value: '%s'\n", key.c_str(), value.c_str());
    } else {
      std::cout <<"NOT FOUND!!!!!!!!!" << std::endl;
    }
  }
  end = clock();
  cpu_time_used = ((double) (end - start)) * 1000000/ CLOCKS_PER_SEC;
  std::cout << "DB open took " << cpu_time_used_open << " microseconds" << std::endl;
  std::cout << "Handler init took " << cpu_time_used_init << " microseconds" << std::endl;
  std::cout << "1000 finds took " << cpu_time_used << " microseconds" << std::endl;

  std::cout << "========================================================================================================" << std::endl;
  std::cout << "========================================================================================================" << std::endl;
  std::cout << "========================================================================================================" << std::endl;

  std::string config_file = "/mnt/persistent/texas/test/chConfig_GEMINIm01L1.txt";
  std::ifstream infile(config_file);
  std::string line;
  int vfatN, vfatCH, trim, mask;
  uint32_t reg_address, reg_mask;
  std::string permissions;
  std::vector<std::string> temp;
  std::getline(infile,line);// skip first line
  std::string reg_basename = "GEM_AMC.OH.OH2.GEB.VFATS.VFATX.VFATChannels.ChanReg";
  while (std::getline(infile,line))
  {
    std::stringstream iss(line);
    if (!(iss >> vfatN >> vfatCH >> trim >> mask)) {
      std::cout << "ERROR SETTINGS READING" << std::endl;
      break;
    } else {
      std::cout << "VFAT Number " << vfatN << " Channel " << vfatCH << " Trim Value " << trim << " Mask Value " << mask << std::endl;
      reg_basename.replace(29,1,SSTR(vfatN));
      reg_basename += SSTR(vfatCH);
      key_.assign(reg_basename);
      std::cout << "Looking for key " << key_.data() << std::endl;
      found = dbi.get(rtxn_,key_,value_);
      if (found) {
        std::printf("key: '%s', value: '%s', value size: '%d'\n", key_.data(), value_.data(), value_.size());
        //std::printf("key: '%s', value: '%s'\n", key.c_str(), value.c_str());
        t_value = std::string(value_.data());
        t_value = t_value.substr(0,value_.size());
        temp = split(t_value,'|');
        std::cout << "Tokens: " << temp[0] << " " << temp[1] << " " << temp[2] << std::endl;
        reg_address = stol(temp[0]);
        permissions = temp[1];
        reg_mask = stoll(temp[2]);
        std::cout << "Register address " << std::hex << reg_address << " Register permissions " << permissions << " Register mask " << reg_mask << std::dec << std::endl;
      } else {
        std::cout <<"NOT FOUND!!!!!!!!!" << std::endl;
      }
      reg_basename = "GEM_AMC.OH.OH2.GEB.VFATS.VFATX.VFATChannels.ChanReg";
    }
  }

  cursor_.close();
  rtxn_.abort();

  //for (auto it:m_parsed_at)
  //{
  //  t_key = it.first;
  //  std::cout << "Entry key " << t_key <<std::endl;
  //}

  return 0;
}
