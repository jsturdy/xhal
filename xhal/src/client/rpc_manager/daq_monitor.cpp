#include "xhal/client/rpcman/daq_monitor.h"

PyListUint32 xhal::client::rpcman::DaqMonitor::getmonTTCmain()
{
  PyListUint32 result;
  req = wisc::RPCMsg("amc.getmonTTCmain");
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  try {
    if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      //FIXME raise an exception
    } else {
      result.clear();
      result.push_back(rsp.get_word("MMCM_LOCKED"));
      result.push_back(rsp.get_word("TTC_SINGLE_ERROR_CNT"));
      result.push_back(rsp.get_word("BC0_LOCKED"));
      result.push_back(rsp.get_word("L1A_ID"));
      result.push_back(rsp.get_word("L1A_RATE"));
    }
  } STANDARD_CATCH;
  return result;
}

PyListUint32 xhal::client::rpcman::DaqMonitor::getmonTRIGGERmain(uint32_t noh)
{
  PyListUint32 result;
  req = wisc::RPCMsg("amc.getmonTRIGGERmain");
  req.set_word("NOH",noh);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  try {
    if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      // FIXME raise an exception
    } else {
      std::string t;
      result.push_back(rsp.get_word("OR_TRIGGER_RATE"));
      for (unsigned int i = 0; i < noh; i++) {
        t = "OH"+std::to_string(i)+".TRIGGER_RATE";
        result.push_back(rsp.get_word(t));
      }
    }
  } STANDARD_CATCH;
  return result;
}

PyListUint32 xhal::client::rpcman::DaqMonitor::getmonTRIGGEROHmain(uint32_t noh)
{
  PyListUint32 result(8*noh,0);
  req = wisc::RPCMsg("amc.getmonTRIGGEROHmain");
  req.set_word("NOH",noh);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  try {
    if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      // FIXME raise an exception
    }else {
      std::string t;
      for (unsigned int i = 0; i < noh; i++) {
        t = "OH"+std::to_string(i)+".LINK0_MISSED_COMMA_CNT";
        result[i] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".LINK1_MISSED_COMMA_CNT";
        result[i+noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".LINK0_OVERFLOW_CNT";
        result[i+2*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".LINK1_OVERFLOW_CNT";
        result[i+3*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".LINK0_UNDERFLOW_CNT";
        result[i+4*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".LINK1_UNDERFLOW_CNT";
        result[i+5*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".LINK0_SBIT_OVERFLOW_CNT";
        result[i+6*noh] = rsp.get_word(t);
        t = "OH"+std::to_string(i)+".LINK1_SBIT_OVERFLOW_CNT";
        result[i+7*noh] = rsp.get_word(t);
      }
    }
  } STANDARD_CATCH;
  return result;
}

PyListUint32 xhal::client::rpcman::DaqMonitor::getmonDAQmain()
{
  PyListUint32 result;
  req = wisc::RPCMsg("amc.getmonDAQmain");
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  try {
    if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      // FIXME raise an exception
    } else {
      result.push_back(rsp.get_word("DAQ_ENABLE"));
      result.push_back(rsp.get_word("DAQ_LINK_READY"));
      result.push_back(rsp.get_word("DAQ_LINK_AFULL"));
      result.push_back(rsp.get_word("DAQ_OFIFO_HAD_OFLOW"));
      result.push_back(rsp.get_word("L1A_FIFO_HAD_OFLOW"));
      result.push_back(rsp.get_word("L1A_FIFO_DATA_COUNT"));
      result.push_back(rsp.get_word("DAQ_FIFO_DATA_COUNT"));
      result.push_back(rsp.get_word("EVENT_SENT"));
      result.push_back(rsp.get_word("TTS_STATE"));
    }
  } STANDARD_CATCH;
  return result;
}

PyListUint32 xhal::client::rpcman::DaqMonitor::getmonDAQOHmain(uint32_t noh)
{
  PyListUint32 result(6*noh,0);
  req = wisc::RPCMsg("amc.getmonDAQOHmain");
  req.set_word("NOH",noh);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  try {
    if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      // FIXME raise an exception
    }else {
      std::string t;
      for (unsigned int i = 0; i < noh; i++) {
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
  } STANDARD_CATCH;
  return result;
}

PyListUint32 xhal::client::rpcman::DaqMonitor::getmonOHmain(uint32_t noh)
{
  PyListUint32 result(7*noh,0);
  req = wisc::RPCMsg("amc.getmonOHmain");
  req.set_word("NOH",noh);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  try {
    if (rsp.get_key_exists("error")) {
      printf("Error: %s",rsp.get_string("error").c_str());
      // FIXME raise an exception
    } else {
      std::string t;
      for (unsigned int i = 0; i < noh; i++) {
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
  } STANDARD_CATCH;
  return result;
}
