#include "xhal/rpc/calibration_routines.h"

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

/***
 * @brief run a generic scan routine
 */
DLLEXPORT uint32_t genScan(uint32_t nevts, uint32_t ohN, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, bool useCalPulse, uint32_t mask, char * scanReg, bool useUltra, bool useExtTrig, uint32_t * result)
{
    req = wisc::RPCMsg("calibration_routines.genScan");

    req.set_word("nevts", nevts);
    req.set_word("ohN", ohN);
    req.set_word("dacMin", dacMin);
    req.set_word("dacMax", dacMax);
    req.set_word("dacStep", dacStep);
    req.set_word("ch", ch);
    req.set_word("useCalPulse", useCalPulse);
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

DLLEXPORT uint32_t sbitRateScan(uint32_t ohN, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, uint32_t maskOh, bool invertVFATPos, char * scanReg, uint32_t waitTime, uint32_t * resultDacVal, uint32_t * resultTrigRate){
    req = wisc::RPCMsg("calibration_routines.sbitRateScan");


    req.set_word("ohN", ohN);
    req.set_word("maskOh", maskOh);
    req.set_word("invertVFATPos", invertVFATPos);
    req.set_word("dacMin", dacMin);
    req.set_word("dacMax", dacMax);
    req.set_word("dacStep", dacStep);
    req.set_word("ch", ch);
    req.set_string("scanReg", std::string(scanReg));
    req.set_word("waitTime", waitTime);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    //Check to make sure (dacMax-dacMin+1)/dacStep is an integer!
    if( 0 != ((dacMax - dacMin + 1) % dacStep) ){
        printf("Caught an error: (dacMax - dacMin + 1)/dacStep must be an integer!\n");
        return 1;
    }
    const uint32_t size = (dacMax - dacMin+1)/dacStep;

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

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

    return 0;
} //End sbitRateScan(...)
