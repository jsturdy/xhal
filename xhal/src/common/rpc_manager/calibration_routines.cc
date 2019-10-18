#include "xhal/rpc/calibration_routines.h"

DLLEXPORT uint32_t checkSbitMappingWithCalPulse(uint32_t ohN, uint32_t vfatN, uint32_t mask, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t nevts, uint32_t L1Ainterval, uint32_t pulseDelay, uint32_t *data){
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

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    const uint32_t size = 128*8*nevts;
    if (rsp.get_key_exists("data")){
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data",data);
    }
    else{
        printf("No key found for data");
        return 1;
    }

    return 0;
} //End checkSbitMappingWithCalPulse()

DLLEXPORT uint32_t checkSbitRateWithCalPulse(uint32_t ohN, uint32_t vfatN, uint32_t mask, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t waitTime, uint32_t pulseRate, uint32_t pulseDelay, uint32_t *outDataCTP7Rate, uint32_t *outDataFPGAClusterCntRate, uint32_t *outDataVFATSBits){
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

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    const uint32_t size = 128;
    if (rsp.get_key_exists("outDataCTP7Rate")) {
        ASSERT(rsp.get_word_array_size("outDataCTP7Rate") == size);
        rsp.get_word_array("outDataCTP7Rate", outDataCTP7Rate);
    }
    else{
        printf("No key found for outDataCTP7Rate");
        return 1;
    }

    if (rsp.get_key_exists("outDataFPGAClusterCntRate")) {
        ASSERT(rsp.get_word_array_size("outDataFPGAClusterCntRate") == size);
        rsp.get_word_array("outDataFPGAClusterCntRate", outDataFPGAClusterCntRate);
    }
    else{
        printf("No key found for outDataFPGAClusterCntRate");
        return 1;
    }

    if (rsp.get_key_exists("outDataVFATSBits")) {
        ASSERT(rsp.get_word_array_size("outDataVFATSBits") == size);
        rsp.get_word_array("outDataVFATSBits", outDataVFATSBits);
    }
    else{
        printf("No key found for outDataVFATSBits");
        return 1;
    }

    return 0;
} //End checkSbitRateWithCalPulse()

DLLEXPORT uint32_t dacScan(uint32_t ohN, uint32_t dacSelect, uint32_t dacStep, uint32_t mask, bool useExtRefADC, uint32_t * results, uint32_t nvfats){
    req = wisc::RPCMsg("calibration_routines.dacScan");

    req.set_word("ohN", ohN);
    req.set_word("dacSelect", dacSelect);
    req.set_word("dacStep", dacStep);
    req.set_word("mask", mask);
    req.set_word("useExtRefADC", useExtRefADC);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    vfat3DACSize dacSize;
    const uint32_t size = (dacSize.max[dacSelect]+1)*nvfats/dacStep;
    if (rsp.get_key_exists("dacScanResults")) {
        ASSERT(rsp.get_word_array_size("dacScanResults") == size);
        rsp.get_word_array("dacScanResults", results);
    } else {
        printf("No dacScanResults key found");
        return 1;
    }

    return 0;
} //End dacScan()

DLLEXPORT uint32_t dacScanMultiLink(uint32_t ohMask, uint32_t NOH, uint32_t dacSelect, uint32_t dacStep, bool useExtRefADC, uint32_t * results, uint32_t nvfats){
    req = wisc::RPCMsg("calibration_routines.dacScanMultiLink");

    req.set_word("ohMask", ohMask);
    req.set_word("NOH",NOH);
    req.set_word("dacSelect", dacSelect);
    req.set_word("dacStep", dacStep);
    req.set_word("useExtRefADC", useExtRefADC);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    vfat3DACSize dacSize;
    const uint32_t size = NOH * (dacSize.max[dacSelect]+1)*nvfats/dacStep;
    if (rsp.get_key_exists("dacScanResultsAll")) {
        ASSERT(rsp.get_word_array_size("dacScanResultsAll") == size);
        rsp.get_word_array("dacScanResultsAll", results);
    } else {
        printf("No dacScanResultsAll key found\n");
        return 1;
    }

    return 0;
} //End dacScanMultiLink()

/***
 * @brief run a generic scan routine for a specific channel
 */
DLLEXPORT uint32_t genScan(uint32_t nevts, uint32_t ohN, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t mask, char * scanReg, bool useUltra, bool useExtTrig, uint32_t * result, uint32_t nvfats)
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
    if(useUltra){
        req.set_word("useUltra", useUltra);
    }
    req.set_word("useExtTrig", useExtTrig);
    req.set_string("scanReg", std::string(scanReg));

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }
    const uint32_t size = (dacMax - dacMin+1)*nvfats/dacStep;
    if (rsp.get_key_exists("data")) {
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data", result);
    } else {
        printf("No data key found");
        return 1;
    }

    return 0;
}

/***
 * @brief run a generic scan routine on all channels
 */
DLLEXPORT uint32_t genChannelScan(uint32_t nevts, uint32_t ohN, uint32_t mask, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, bool useExtTrig, char * scanReg, bool useUltra, uint32_t * result, uint32_t nvfats){
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
    if(useUltra){
        req.set_word("useUltra", useUltra);
    }
    req.set_string("scanReg", std::string(scanReg));

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }
    const uint32_t size = nvfats*128*(dacMax-dacMin+1)/dacStep;
    if (rsp.get_key_exists("data")) {
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data", result);
    } else {
        printf("No data key found");
        return 1;
    }

    return 0;
} //End genChannelScan()

DLLEXPORT uint32_t sbitRateScan(uint32_t ohMask, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, char * scanReg, uint32_t * resultDacVal, uint32_t * resultTrigRate, uint32_t * resultTrigRatePerVFAT, uint32_t nvfats, uint32_t waitTime)
{
    req = wisc::RPCMsg("calibration_routines.sbitRateScan");

    req.set_word("dacMin", dacMin);
    req.set_word("dacMax", dacMax);
    req.set_word("dacStep", dacStep);
    req.set_word("ch", ch);
    req.set_word("ohMask", ohMask);
    req.set_word("waitTime", waitTime);    
    req.set_string("scanReg", std::string(scanReg));

    wisc::RPCSvc* rpc_loc = getRPCptr();

    //Check to make sure (dacMax-dacMin+1)/dacStep is an integer!
    if( 0 != ((dacMax - dacMin + 1) % dacStep) ){
        printf("Caught an error: (dacMax - dacMin + 1)/dacStep must be an integer!\n");
        return 1;
    }
    const uint32_t size = 12 * (dacMax - dacMin+1)/dacStep;

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    if (rsp.get_key_exists("outDataDacValue")) {
        ASSERT(rsp.get_word_array_size("outDataDacValue") == size);
        rsp.get_word_array("outDataDacValue", resultDacVal);
    } else {
        printf("No key found for data dac values\n");
        return 1;
    }

    if (rsp.get_key_exists("outDataCTP7Rate")) {
        ASSERT(rsp.get_word_array_size("outDataCTP7Rate") == size);
        rsp.get_word_array("outDataCTP7Rate", resultTrigRate);
    } else {
        printf("No key found for data trigger rate values\n");
        return 1;
    }

    if (rsp.get_key_exists("outDataVFATRate")) {
        ASSERT(rsp.get_word_array_size("outDataVFATRate") == (size*nvfats));
        rsp.get_word_array("outDataVFATRate", resultTrigRatePerVFAT);
    }
    else{
        printf("No key found for data trigger rate per vfat values\n");
        return 1;
    }

    return 0;
} //End sbitRateScan(...)

/***
 * @brief configure TTC generator
 */
DLLEXPORT uint32_t ttcGenConf(uint32_t ohN, uint32_t mode, uint32_t type, uint32_t pulseDelay, uint32_t L1Ainterval, uint32_t nPulses, bool enable)
{
    /*
     * v3  electronics Behavior:
     *      pulseDelay (only for enable = true), delay between CalPulse and L1A
     *      L1Ainterval (only for enable = true), how often to repeat signals
     *      enable = true (false) ignore (take) ttc commands from backplane for this AMC (affects all links)
     * v2b electronics behavior:
     *      Configure the T1 controller
     *      mode: 0 (Single T1 signal),
     *            1 (CalPulse followed by L1A),
     *            2 (pattern)
     *      type (only for mode 0, type of T1 signal to send):
     *            0 L1A
     *            1 CalPulse
     *            2 Resync
     *            3 BC0
     *      pulseDelay (only for mode 1), delay between CalPulse and L1A
     *      L1Ainterval (only for mode 0,1), how often to repeat signals
     *      nPulses how many signals to send (0 is continuous)
     *      enable = true (false) start (stop) the T1Controller for link ohN
     */

    wisc::RPCSvc* rpc_loc = getRPCptr();
    req = wisc::RPCMsg("calibration_routines.ttcGenConf");
    req.set_word("ohN", ohN);
    req.set_word("mode", mode);
    req.set_word("type", type);
    req.set_word("pulseDelay", pulseDelay);
    req.set_word("L1Ainterval", L1Ainterval);
    req.set_word("nPulses", nPulses);
    req.set_word("enable", enable);
    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        return 1;
    }
    return 0;
}

DLLEXPORT uint32_t ttcGenToggle(uint32_t ohN, bool enable){
    /*
     * v3  electronics: enable = true (false) ignore (take) ttc commands from backplane for this AMC
     * v2b electronics: enable = true (false) start (stop) the T1Controller for link ohN
     */

    wisc::RPCSvc* rpc_loc = getRPCptr();
    req = wisc::RPCMsg("calibration_routines.ttcGenToggle");
    req.set_word("ohN", ohN);
    req.set_word("enable", enable);
    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        return 1;
    }
    return 0;
} //End ttcGenToggle(...)

DLLEXPORT uint32_t confCalPulse(uint32_t ohN, uint32_t mask, uint32_t ch, bool toggleOn, bool currentPulse, uint32_t calScaleFactor) {
    req = wisc::RPCMsg("calibration_routines.confCalPulse");

    req.set_word("ohN", ohN);
    req.set_word("ch", ch);
    req.set_word("mask", mask);
    req.set_word("toggleOn",toggleOn);
    req.set_word("currentPulse", currentPulse);
    req.set_word("calScaleFactor", calScaleFactor);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    return 0;
} //End confCalPulse()
