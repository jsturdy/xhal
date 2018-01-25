#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xhal/rpc/wiscrpcsvc.h"

#define DLLEXPORT extern "C"

#define STANDARD_CATCH \
	catch (wisc::RPCSvc::NotConnectedException &e) { \
		printf("Caught NotConnectedException: %s\n", e.message.c_str()); \
		return 1; \
	} \
	catch (wisc::RPCSvc::RPCErrorException &e) { \
		printf("Caught RPCErrorException: %s\n", e.message.c_str()); \
		return 1; \
	} \
	catch (wisc::RPCSvc::RPCException &e) { \
		printf("Caught exception: %s\n", e.message.c_str()); \
		return 1; \
	} \
	catch (wisc::RPCMsg::BadKeyException &e) { \
		printf("Caught exception: %s\n", e.key.c_str()); \
		return 0xdeaddead; \
	} 

#define ASSERT(x) do { \
		if (!(x)) { \
			printf("Assertion Failed on line %u: %s\n", __LINE__, #x); \
			return 1; \
		} \
	} while (0)

static wisc::RPCSvc rpc;
static wisc::RPCMsg req, rsp;

DLLEXPORT uint32_t init(char * hostname)
{
  try {
		rpc.connect(hostname);
	}
	catch (wisc::RPCSvc::ConnectionFailedException &e) {
		printf("Caught RPCErrorException: %s\n", e.message.c_str());
		return 1;
	}
	catch (wisc::RPCSvc::RPCException &e) {
		printf("Caught exception: %s\n", e.message.c_str());
		return 1;
	}

  try {
  		ASSERT(rpc.load_module("memory", "memory v1.0.1"));
  		ASSERT(rpc.load_module("extras", "extras v1.0.1"));
  		ASSERT(rpc.load_module("utils", "utils v1.0.1"));
  		ASSERT(rpc.load_module("optohybrid", "optohybrid v1.0.1"));
  		ASSERT(rpc.load_module("amc", "amc v1.0.1"));
  	}
  	STANDARD_CATCH;

  return 0;
}

DLLEXPORT uint32_t update_atdb(char * xmlfilename)
{
	req = wisc::RPCMsg("utils.update_address_table");
	req.set_string("at_xml", xmlfilename);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

	if (rsp.get_key_exists("error")) {
    return 1;
  }
	return 0;
}

DLLEXPORT uint32_t getRegInfoDB(char * regName)
{
  uint32_t address, mask;
  std::string permissions;
	req = wisc::RPCMsg("utils.readRegFromDB");
	req.set_string("reg_name", regName);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

	if (rsp.get_key_exists("error")) {
    return 1;
  }
  printf("Register %s is found \n",regName);
  try {
    address = rsp.get_word("address");
    mask = rsp.get_word("mask");
    permissions = rsp.get_string("permissions");

    printf("Address: 0x%8x, permissions: %s, mask: 0x%8x \n", address, permissions.c_str(), mask);
  }
	STANDARD_CATCH;
	return 0;
}

DLLEXPORT uint32_t configureVT1(char * ohN, char * config, unsigned int vt1 = 0x64)
{
	req = wisc::RPCMsg("optohybrid.loadVT1");
	req.set_string("oh_number", ohN);
  req.set_word("vt1",vt1);
	if (config && config[0]!='\0') req.set_string("thresh_config_filename", config);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

	if (rsp.get_key_exists("error")) {
    printf("Error: %s",rsp.get_string("error").c_str());
    return 1;
  }
	return 0;
}

DLLEXPORT uint32_t configureTRIMDAC(char * ohN, char * config)
{
	req = wisc::RPCMsg("optohybrid.loadTRIMDAC");
	req.set_string("oh_number", ohN);
	req.set_string("trim_config_filename", config);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

	if (rsp.get_key_exists("error")) {
    printf("Error: %s",rsp.get_string("error").c_str());
    return 1;
  }
	return 0;
}

DLLEXPORT uint32_t configureVFATs(char * ohN, char * trim_config, char * thresh_config, unsigned int set_run = 1, unsigned int vt1 = 0x64)
{
	req = wisc::RPCMsg("optohybrid.configureVFATs");
	req.set_string("oh_number", ohN);
  req.set_word("vt1",vt1);
	req.set_string("trim_config_filename", trim_config);
  if (set_run) req.set_word("set_run", set_run);
	if (thresh_config && thresh_config[0]!='\0') req.set_string("thresh_config_filename", thresh_config);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

	if (rsp.get_key_exists("error")) {
    printf("Error: %s",rsp.get_string("error").c_str());
    return 1;
  }
	return 0;
}

DLLEXPORT uint32_t getmonTTCmain(uint32_t* result)
{
	req = wisc::RPCMsg("amc.getmonTTCmain");
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

  try{
	  if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      return 1;
    } else {
      result[0] = rsp.get_word("MMCM_LOCKED");
      result[1] = rsp.get_word("TTC_SINGLE_ERROR_CNT");
      result[2] = rsp.get_word("BC0_LOCKED");
      result[3] = rsp.get_word("L1A_ID");
      result[4] = rsp.get_word("L1A_RATE");
    }
  }
	STANDARD_CATCH;

	return 0;
}

DLLEXPORT uint32_t getmonTRIGGERmain(uint32_t* result, uint32_t noh = 10)
{
	req = wisc::RPCMsg("amc.getmonTRIGGERmain");
  req.set_word("NOH",noh);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

  try{
	  if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      return 1;
    } else {
      std::string t;
      result[0] = rsp.get_word("OR_TRIGGER_RATE");
      for (int i = 0; i < noh; i++) {
        t = "OH"+std::to_string(i)+".TRIGGER_RATE";
        result[i+1] = rsp.get_word(t);
      }
    }
  }
	STANDARD_CATCH;

	return 0;
}

DLLEXPORT uint32_t getmonTRIGGEROHmain(uint32_t* result, uint32_t noh = 10)
{
	req = wisc::RPCMsg("amc.getmonTRIGGEROHmain");
  req.set_word("NOH",noh);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

  try{
	  if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      return 1;
    } else {
      std::string t;
      for (int i = 0; i < noh; i++) {
        t = "OH"+std::to_string(i)+".LINK0_NOT_VALID_CNT";
        result[i] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".LINK1_NOT_VALID_CNT";
        result[i+noh] = rsp.get_word(t);
      }
    }
  }
	STANDARD_CATCH;

	return 0;
}

DLLEXPORT uint32_t getmonDAQmain(uint32_t* result)
{
	req = wisc::RPCMsg("amc.getmonDAQmain");
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

  try{
	  if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      return 1;
    } else {
      result[0] = rsp.get_word("DAQ_ENABLE");
      result[1] = rsp.get_word("DAQ_LINK_READY");
      result[2] = rsp.get_word("DAQ_LINK_AFULL");
      result[3] = rsp.get_word("DAQ_OFIFO_HAD_OFLOW");
      result[4] = rsp.get_word("L1A_FIFO_HAD_OFLOW");
      result[5] = rsp.get_word("L1A_FIFO_DATA_COUNT");
      result[6] = rsp.get_word("DAQ_FIFO_DATA_COUNT");
      result[7] = rsp.get_word("EVENT_SENT");
      result[8] = rsp.get_word("TTS_STATE");
    }
  }
	STANDARD_CATCH;

	return 0;
}

DLLEXPORT uint32_t getmonDAQOHmain(uint32_t* result, uint32_t noh = 10)
{
	req = wisc::RPCMsg("amc.getmonDAQOHmain");
  req.set_word("NOH",noh);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

  try{
	  if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      return 1;
    } else {
      std::string t;
      for (int i = 0; i < noh; i++) {
        t = "OH"+std::to_string(i)+".STATUS.EVT_SIZE_ERR";
        result[i] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".STATUS.EVENT_FIFO_HAD_OFLOW";
        result[i+noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".STATUS.INPUT_FIFO_HAD_OFLOW";
        result[i+2*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".STATUS.INPUT_FIFO_HAD_UFLOW";
        result[i+3*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".STATUS.VFAT_TOO_MANY";
        result[i+4*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".STATUS.VFAT_NO_MARKER";
        result[i+5*noh] = rsp.get_word(t);
      }
    }
  }
	STANDARD_CATCH;

	return 0;
}

DLLEXPORT uint32_t getmonOHmain(uint32_t* result, uint32_t noh = 10)
{
	req = wisc::RPCMsg("amc.getmonOHmain");
  req.set_word("NOH",noh);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

  try{
	  if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      return 1;
    } else {
      std::string t;
      for (int i = 0; i < noh; i++) {
        t = "OH"+std::to_string(i)+".FW_VERSION";
        result[i] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".EVENT_COUNTER";
        result[i+noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".EVENT_RATE";
        result[i+2*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".GTX.TRK_ERR";
        result[i+3*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".GTX.TRG_ERR";
        result[i+4*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".GBT.TRK_ERR";
        result[i+5*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".CORR_VFAT_BLK_CNT";
        result[i+6*noh] = rsp.get_word(t);
      }
    }
  }
	STANDARD_CATCH;

	return 0;
}

DLLEXPORT unsigned long getReg(unsigned int address)
{
	req = wisc::RPCMsg("memory.read");
	req.set_word("address", address);
	req.set_word("count", 1);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

	uint32_t result;
  try{
	  if (rsp.get_key_exists("error")) {
      return 0xdeaddead;
    } else {
	    ASSERT(rsp.get_word_array_size("data") == 1);
      rsp.get_word_array("data", &result);
    }
  }
	STANDARD_CATCH;

	return result;
}

DLLEXPORT unsigned long getBlock(unsigned int address, uint32_t* result, ssize_t size)
{
 	req = wisc::RPCMsg("extras.blockread");
	req.set_word("address", address);
	req.set_word("count", size);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

  try{
	  if (rsp.get_key_exists("error")) {
      return 1;
    } else {
	    ASSERT(rsp.get_word_array_size("data") == size);
      rsp.get_word_array("data", result);
    }
  }
	STANDARD_CATCH;
  return 0;
}

DLLEXPORT unsigned long getList(uint32_t* addresses, uint32_t* result, ssize_t size)
{
 	req = wisc::RPCMsg("extras.listread");
	req.set_word_array("addresses", addresses,size);
	req.set_word("count", size);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;

  try{
	  if (rsp.get_key_exists("error")) {
      return 1;
    } else {
	    ASSERT(rsp.get_word_array_size("data") == size);
      rsp.get_word_array("data", result);
    }
  }
	STANDARD_CATCH;
  return 0;
}
DLLEXPORT unsigned long putReg(unsigned int address, unsigned int value)
{
	req = wisc::RPCMsg("memory.write");
	req.set_word("address", address);
	req.set_word_array("data", &value,1);
	try {
		rsp = rpc.call_method(req);
	}
	STANDARD_CATCH;
  if (rsp.get_key_exists("error")) {
    return 0xdeaddead;
  } else return value;
}
