#include "xhal/client/rpcman/utils.h"

uint32_t xhal::client::rpcman::Utils::update_atdb(char * xmlfilename)
{
  req = wisc::RPCMsg("utils.update_address_table");
  req.set_string("at_xml", xmlfilename);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    XHAL_ERROR("Address table update failed!");
    throw xhal::common::utils::XHALException("Error during address table update");
  }
  return 0;
}

uint32_t xhal::client::rpcman::Utils::getRegInfoDB(char * regName)
{
  uint32_t address, mask;
  std::string permissions;
  req = wisc::RPCMsg("utils.readRegFromDB");
  req.set_string("reg_name", regName);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    return 1;
  }
  printf("Register %s is found \n",regName);
  try {
    address = rsp.get_word("address");
    mask = rsp.get_word("mask");
    permissions = rsp.get_string("permissions");

    printf("Address: 0x%8x, permissions: %s, mask: 0x%8x \n", address, permissions.c_str(), mask);
  } STANDARD_CATCH;
  return 0;
}
