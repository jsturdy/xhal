#include "xhal/client/XHALDevice.h"

xhal::client::XHALDevice::XHALDevice(const std::string& board_domain_name, const std::string& address_table_filename) :
  xhal::client::XHALInterface(board_domain_name),
  m_address_table_filename(address_table_filename)
{
  //FIXME Implement try-catch
  XHAL_DEBUG("Address table name " << m_address_table_filename);
  m_parser = new xhal::common::utils::XHALXMLParser(m_address_table_filename);
  XHAL_DEBUG("XHALXML parser created");
  m_parser->setLogLevel(2);
  m_parser->parseXML();
  this->loadModule("memory","memory v1.0.1");
  this->loadModule("extras","extras v1.0.1");
}

void xhal::client::XHALDevice::reconnect()
{
  if (!isConnected) {
    this->connect();
    this->loadModule("memory","memory v1.0.1");
    this->loadModule("extras","extras v1.0.1");
  } else {
    XHAL_ERROR("Interface already connected. Reconnection failed");
    throw xhal::common::utils::XHALRPCException("RPC exception: Interface already connected. Reconnection failed");
  }
}

uint32_t xhal::client::XHALDevice::readReg(std::string regName)
{
  if (auto t_node = m_parser->getNode(regName.c_str())) {
    m_node = t_node.get();
    req = wisc::RPCMsg("memory.read");
    req.set_word("address", m_node.real_address);
    req.set_word("count", 1);
    try {
      rsp = rpc.call_method(req);
    } STANDARD_CATCH;
    uint32_t result;
    if (rsp.get_key_exists("error")) {
      XHAL_ERROR("RPC response returned error, readReg failed");
      throw xhal::common::utils::XHALException("Error during register access");
    } else {
      try {
        ASSERT(rsp.get_word_array_size("data") == 1);
        rsp.get_word_array("data", &result);
      } STANDARD_CATCH;
    }
    XHAL_DEBUG("RESULT: " << std::hex << result);
    XHAL_DEBUG("Node mask: " << std::hex << m_node.mask);
    uint32_t mask = m_node.mask;
    result = result & mask;
    XHAL_DEBUG("RESULT after applying mask: " << std::hex << result);
    for (int i = 0; i < 32; i++) {
      if (mask & 1) {
        break;
      } else {
        mask = mask >> 1;
        result = result >> 1;
      }
    }
    return result;
  } else {
    XHAL_ERROR("Register not found in address table!");
    throw xhal::common::utils::XHALXMLParserException(strcat("XHAL XML exception: can't find node", regName.c_str()));
  }
}

// In current shape implements raw address reading... Should the signature be update? FIXME
uint32_t xhal::client::XHALDevice::readReg(uint32_t address)
{
  req = wisc::RPCMsg("memory.read");
  req.set_word("address", address);
  req.set_word("count", 1);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;
  uint32_t result;
  if (rsp.get_key_exists("error")) {
    XHAL_ERROR("RPC response returned error, readReg failed");
    throw xhal::common::utils::XHALException("Error during register access");
  } else {
    try {
      ASSERT(rsp.get_word_array_size("data") == 1);
      rsp.get_word_array("data", &result);
    } STANDARD_CATCH;
  }
  //uint32_t mask = m_node.mask;
  //result = result & mask;
  //for (int i = 0; i < 32; i++) {
  //  if (result & 0x01) {
  //    break;
  //  }else {
  //    result = result >> 1;
  //  }
  //}
  return result;
}

void xhal::client::XHALDevice::writeReg(std::string regName, uint32_t value)
{
  if (auto t_node = m_parser->getNode(regName.c_str())) {
    m_node = t_node.get();
    if (m_node.mask == 0xFFFFFFFF) {
      req = wisc::RPCMsg("memory.write");
      req.set_word("address", m_node.real_address);
      req.set_word("count", 1);
      req.set_word("data", value);
      try {
      	rsp = rpc.call_method(req);
      } STANDARD_CATCH;
      if (rsp.get_key_exists("error")) {
        XHAL_ERROR("RPC response returned error, writeReg failed");
        throw xhal::common::utils::XHALException("Error during register access");
      }
    } else {
      uint32_t current_val = this->readReg(m_node.real_address);
      int shift_amount = 0;
      uint32_t mask = m_node.mask;
      for (int i = 0; i < 32; i++) {
        if (mask & 1) {
          break;
        } else {
          shift_amount +=1;
          mask = mask >> 1;
        }
      }
      uint32_t val_to_write = value << shift_amount;
      val_to_write = (val_to_write & m_node.mask) | (current_val & ~m_node.mask);
      req = wisc::RPCMsg("memory.write");
      req.set_word("address", m_node.real_address);
      req.set_word_array("data", &val_to_write,1);
      try {
      	rsp = rpc.call_method(req);
      } STANDARD_CATCH;
      if (rsp.get_key_exists("error")) {
        XHAL_ERROR("RPC response returned error, writeReg failed");
        throw xhal::common::utils::XHALException("Error during register access");
      }
    }
  } else {
    XHAL_ERROR("Register not found in address table!");
    throw xhal::common::utils::XHALXMLParserException(strcat("XHAL XML exception: can't find node", regName.c_str()));
  }
}

uint32_t xhal::client::XHALDevice::getList(uint32_t* addresses, uint32_t* result, ssize_t size)
{
  req = wisc::RPCMsg("extras.listread");
  req.set_word_array("addresses", addresses,size);
  req.set_word("count", size);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  try {
    if (rsp.get_key_exists("error")) {
      return 1;
    } else {
      ASSERT(rsp.get_word_array_size("data") == size);
      rsp.get_word_array("data", result);
    }
  } STANDARD_CATCH;
  return 0;
}

uint32_t xhal::client::XHALDevice::getBlock(uint32_t address, uint32_t* result, ssize_t size)
{
  req = wisc::RPCMsg("extras.blockread");
  req.set_word("address", address);
  req.set_word("count", size);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  try {
    if (rsp.get_key_exists("error")) {
      return 1;//FIXME throw an exception
    } else {
      ASSERT(rsp.get_word_array_size("data") == size);
      rsp.get_word_array("data", result);
    }
  } STANDARD_CATCH;
  return 0;
}
