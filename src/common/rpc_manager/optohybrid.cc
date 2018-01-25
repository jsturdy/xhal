#include "xhal/rpc/optohybrid.h"

DLLEXPORT uint32_t broadcastRead(uint32_t ohN, char * regName, uint32_t vfatMask, uint32_t * result){
    /* User supplies the VFAT node name as reg_name, examples:
     *
     *    v2b electronics: reg_name = "VThreshold1" to get VT1
     *    v3 electronics: reg_name = "CFG_THR_ARM_DAC"
     *
     *    Supplying only a substr of VFAT Node name will crash
     */

    req = wisc::RPCMsg("optohybrid.broadcastRead");

    req.set_string("reg_name",std::string(regName));
    req.set_word("oh_number",ohN);
    req.set_word("mask",vfatMask);
    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }
    else if (rsp.get_key_exists("data")) {
        const uint32_t size = 24;
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data", result);
    } else {
        printf("No data key found");
        return 1;
    }
    return 0;
} //End broadcastRead

DLLEXPORT uint32_t broadcastWrite(uint32_t ohN, char * regName, uint32_t value, uint32_t vfatMask){
    /* User supplies the VFAT node name as reg_name, examples:
     *
     *    v2b electronics: reg_name = "VThreshold1" to get VT1
     *    v3 electronics: reg_name = "CFG_THR_ARM_DAC"
     *
     *    Supplying only a substr of VFAT Node name will crash
     */

    req = wisc::RPCMsg("optohybrid.broadcastWrite");

    req.set_string("reg_name",std::string(regName));
    req.set_word("oh_number",ohN);
    req.set_word("value",value);
    req.set_word("mask",vfatMask);
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
}


DLLEXPORT uint32_t configureScanModule(uint32_t ohN, uint32_t vfatN, uint32_t scanmode, bool useUltra,
        uint32_t vfatMask, uint32_t ch, uint32_t nevts, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep){
    req = wisc::RPCMsg("optohybrid.configureScanModule");

    req.set_word("ohN",ohN);
    req.set_word("scanmode",scanmode);
    if (useUltra){
        req.set_word("useUltra",useUltra);
        req.set_word("mask",vfatMask);
    }
    else{
        req.set_word("vfatN",vfatN);
    }
    req.set_word("ch", ch); //channel
    req.set_word("nevts",nevts);
    req.set_word("dacMin",dacMin);
    req.set_word("dacMax",dacMax);
    req.set_word("dacStep",dacStep);

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
} //End configureScanModule(...)

DLLEXPORT uint32_t printScanConfiguration(uint32_t ohN, bool useUltra){
    req = wisc::RPCMsg("optohybrid.printScanConfiguration");

    req.set_word("ohN",ohN);
    if (useUltra){
        req.set_word("useUltra",useUltra);
    }

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
} //End printScanConfiguration(...)

DLLEXPORT uint32_t startScanModule(uint32_t ohN, bool useUltra){
    req = wisc::RPCMsg("optohybrid.startScanModule");

    req.set_word("ohN",ohN);
    if (useUltra){
        req.set_word("useUltra",useUltra);
    }

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
} //End startScanModule(...)

DLLEXPORT uint32_t getUltraScanResults(uint32_t ohN, uint32_t nevts, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t * result){
    req = wisc::RPCMsg("optohybrid.getUltraScanResults");

    req.set_word("ohN",ohN);
    req.set_word("nevts",nevts);
    req.set_word("dacMin",dacMin);
    req.set_word("dacMax",dacMax);
    req.set_word("dacStep",dacStep);

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
} //End getUltraScanResults(...)

DLLEXPORT uint32_t stopCalPulse2AllChannels(uint32_t ohN, uint32_t mask, uint32_t ch_min, uint32_t ch_max){
    req = wisc::RPCMsg("optohybrid.stopCalPulse2AllChannels");

    req.set_word("ohN",ohN);
    req.set_word("mask",mask);
    req.set_word("ch_min",ch_min);
    req.set_word("ch_max",ch_max);

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
}
