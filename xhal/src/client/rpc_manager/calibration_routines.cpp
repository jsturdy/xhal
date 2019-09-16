#include "xhal/client/rpcman/calibration_routines.h"

uint32_t xhal::client::rpcman::CalRoutines::checkSbitMappingWithCalPulse(uint32_t ohN, uint32_t vfatN, uint32_t mask, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t nevts, uint32_t L1Ainterval, uint32_t pulseDelay, uint32_t *data)
{
  req = wisc::RPCMsg("calibration_routines.checkSbitMappingWithCalPulse");

  req.set_word("ohN", ohN);
  req.set_word("vfatN", vfatN);
  req.set_word("mask", mask);
  req.set_word("useCalPulse", useCalPulse);
  req.set_word("currentPulse", currentPulse);
  req.set_word("calScaleFactor", calScaleFactor);
  req.set_word("nevts", nevts);
  req.set_word("L1Ainterval", L1Ainterval);
  req.set_word("pulseDelay", pulseDelay);

  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
    return 1;
  }

  const uint32_t size = 128*8*nevts;
  if (rsp.get_key_exists("data")){
    ASSERT(rsp.get_word_array_size("data") == size);
    rsp.get_word_array("data",data);
  } else {
    printf("No key found for data");
    return 1;
  }

  return 0;
}

uint32_t xhal::client::rpcman::CalRoutines::checkSbitRateWithCalPulse(uint32_t ohN, uint32_t vfatN, uint32_t mask, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t waitTime, uint32_t pulseRate, uint32_t pulseDelay, uint32_t *outDataCTP7Rate, uint32_t *outDataFPGAClusterCntRate, uint32_t *outDataVFATSBits)
{
  req = wisc::RPCMsg("calibration_routines.checkSbitRateWithCalPulse");

  req.set_word("ohN", ohN);
  req.set_word("vfatN", vfatN);
  req.set_word("mask", mask);
  req.set_word("useCalPulse", useCalPulse);
  req.set_word("currentPulse", currentPulse);
  req.set_word("calScaleFactor", calScaleFactor);
  req.set_word("waitTime", waitTime);
  req.set_word("pulseRate", pulseRate);
  req.set_word("pulseDelay", pulseDelay);

  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
    return 1;
  }

  const uint32_t size = 128;
  if (rsp.get_key_exists("outDataCTP7Rate")) {
    ASSERT(rsp.get_word_array_size("outDataCTP7Rate") == size);
    rsp.get_word_array("outDataCTP7Rate", outDataCTP7Rate);
  } else {
    printf("No key found for outDataCTP7Rate");
    return 1;
  }

  if (rsp.get_key_exists("outDataFPGAClusterCntRate")) {
    ASSERT(rsp.get_word_array_size("outDataFPGAClusterCntRate") == size);
    rsp.get_word_array("outDataFPGAClusterCntRate", outDataFPGAClusterCntRate);
  } else {
    printf("No key found for outDataFPGAClusterCntRate");
    return 1;
  }

  if (rsp.get_key_exists("outDataVFATSBits")) {
    ASSERT(rsp.get_word_array_size("outDataVFATSBits") == size);
    rsp.get_word_array("outDataVFATSBits", outDataVFATSBits);
  } else {
    printf("No key found for outDataVFATSBits");
    return 1;
  }

  return 0;
}

uint32_t xhal::client::rpcman::CalRoutines::genScan(uint32_t nevts, uint32_t ohN, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t mask, char * scanReg, bool useUltra, bool useExtTrig, uint32_t * result)
{
  req = wisc::RPCMsg("calibration_routines.genScan");

  req.set_word("nevts", nevts);
  req.set_word("ohN", ohN);
  req.set_word("dacMin", dacMin);
  req.set_word("dacMax", dacMax);
  req.set_word("dacStep", dacStep);
  req.set_word("ch", ch);
  req.set_word("useCalPulse", useCalPulse);
  req.set_word("currentPulse", currentPulse);
  req.set_word("calScaleFactor", calScaleFactor);
  req.set_word("mask", mask);
  if (useUltra) {
    req.set_word("useUltra", useUltra);
  }
  req.set_word("useExtTrig", useExtTrig);
  req.set_string("scanReg", std::string(scanReg));

  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
    return 1;
  }
  const uint32_t size = (dacMax - dacMin+1)*24/dacStep;
  if (rsp.get_key_exists("data")) {
    ASSERT(rsp.get_word_array_size("data") == size);
    rsp.get_word_array("data", result);
  } else {
    printf("No data key found");
    return 1;
  }

  return 0;
}

uint32_t xhal::client::rpcman::CalRoutines::genChannelScan(uint32_t nevts, uint32_t ohN, uint32_t mask, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, bool useExtTrig, char * scanReg, bool useUltra, uint32_t * result)
{
  req = wisc::RPCMsg("calibration_routines.genChannelScan");

  req.set_word("nevts", nevts);
  req.set_word("ohN", ohN);
  req.set_word("mask", mask);
  req.set_word("dacMin", dacMin);
  req.set_word("dacMax", dacMax);
  req.set_word("dacStep", dacStep);
  req.set_word("useCalPulse", useCalPulse);
  req.set_word("currentPulse", currentPulse);
  req.set_word("calScaleFactor", calScaleFactor);
  req.set_word("useExtTrig", useExtTrig);
  if (useUltra) {
    req.set_word("useUltra", useUltra);
  }
  req.set_string("scanReg", std::string(scanReg));

  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
    return 1;
  }
  const uint32_t size = 24*128*(dacMax-dacMin+1)/dacStep;
  if (rsp.get_key_exists("data")) {
    ASSERT(rsp.get_word_array_size("data") == size);
    rsp.get_word_array("data", result);
  } else {
    printf("No data key found");
    return 1;
  }

  return 0;
}

uint32_t xhal::client::rpcman::CalRoutines::sbitRateScan(uint32_t ohN, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, uint32_t maskOh, bool invertVFATPos, char * scanReg, uint32_t waitTime, uint32_t * resultDacVal, uint32_t * resultTrigRate, uint32_t * resultTrigRatePerVFAT, bool isParallel)
{
  req = wisc::RPCMsg("calibration_routines.sbitRateScan");


  req.set_word("ohN", ohN);
  req.set_word("maskOh", maskOh);
  req.set_word("invertVFATPos", invertVFATPos);
  req.set_word("isParallel", isParallel);
  req.set_word("dacMin", dacMin);
  req.set_word("dacMax", dacMax);
  req.set_word("dacStep", dacStep);
  req.set_word("ch", ch);
  req.set_string("scanReg", std::string(scanReg));
  req.set_word("waitTime", waitTime);

  //Check to make sure (dacMax-dacMin+1)/dacStep is an integer!
  if (0 != ((dacMax - dacMin + 1) % dacStep)) {
    printf("Caught an error: (dacMax - dacMin + 1)/dacStep must be an integer!\n");
    return 1;
  }
  const uint32_t size = (dacMax - dacMin+1)/dacStep;

  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
    return 1;
  }

  if (rsp.get_key_exists("data_dacVal")) {
    ASSERT(rsp.get_word_array_size("data_dacVal") == size);
    rsp.get_word_array("data_dacVal", resultDacVal);
  } else {
    printf("No key found for data dac values");
    return 1;
  }

  if (rsp.get_key_exists("data_trigRate")) {
    ASSERT(rsp.get_word_array_size("data_trigRate") == size);
    rsp.get_word_array("data_trigRate", resultTrigRate);
  } else {
    printf("No key found for data trigger rate values");
    return 1;
  }

  if (isParallel) {
    if (rsp.get_key_exists("data_trigRatePerVFAT")) {
      ASSERT(rsp.get_word_array_size("data_trigRatePerVFAT") == (size*24));
      rsp.get_word_array("data_trigRatePerVFAT", resultTrigRatePerVFAT);
    }
    else {
      printf("No key found for data trigger rate per vfat values");
      return 1;
    }
  }

  return 0;
}

uint32_t xhal::client::rpcman::CalRoutines::ttcGenConf(uint32_t ohN, uint32_t mode, uint32_t type, uint32_t pulseDelay, uint32_t L1Ainterval, uint32_t nPulses, bool enable)
{

  req = wisc::RPCMsg("calibration_routines.ttcGenConf");
  req.set_word("ohN", ohN);
  req.set_word("mode", mode);
  req.set_word("type", type);
  req.set_word("pulseDelay", pulseDelay);
  req.set_word("L1Ainterval", L1Ainterval);
  req.set_word("nPulses", nPulses);
  req.set_word("enable", enable);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    return 1;
  }
  return 0;
}

uint32_t xhal::client::rpcman::CalRoutines::ttcGenToggle(uint32_t ohN, bool enable)
{
  req = wisc::RPCMsg("calibration_routines.ttcGenToggle");
  req.set_word("ohN", ohN);
  req.set_word("enable", enable);
  try {
    rsp = rpc.call_method(req);
  } STANDARD_CATCH;

  if (rsp.get_key_exists("error")) {
    return 1;
  }
  return 0;
}
