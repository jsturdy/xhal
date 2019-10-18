#include "xhal/rpc/gbt.h"

DLLEXPORT uint32_t scanGBTPhases(uint32_t *results, uint32_t ohN, uint32_t nScans, uint32_t phaseMin, uint32_t phaseMax, uint32_t phaseStep, uint32_t nVFAT, uint32_t nVerificationReads)
{
    req = wisc::RPCMsg("gbt.scanGBTPhases");

    req.set_word("ohN", ohN);
    req.set_word("nScans", nScans);
    req.set_word("phaseMin", phaseMin);
    req.set_word("phaseMax", phaseMax);
    req.set_word("phaseStep", phaseStep);
    req.set_word("nVerificationReads", nVerificationReads);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    for (unsigned int vfatN = 0; vfatN < nVFAT; vfatN++)
    {
        const std::string resultsKey = "OH" + std::to_string(ohN) + ".VFAT" + std::to_string(vfatN);
        if (rsp.get_key_exists(resultsKey)){
            ASSERT(rsp.get_word_array_size(resultsKey) == 16);
            rsp.get_word_array(resultsKey, results + 16*vfatN);
        }
        else{
            printf("Key '%s' not found.", resultsKey.c_str());
            return 1;
        }
    }

    return 0;
} //End scanGBTPhase(...)

DLLEXPORT uint32_t writeGBTConfig(uint32_t ohN, uint32_t gbtN, uint32_t configSize, uint8_t *config){
    req = wisc::RPCMsg("gbt.writeGBTConfig");

    req.set_word("ohN", ohN);
    req.set_word("gbtN", gbtN);
    req.set_binarydata("config", config, configSize);

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
} //End writeGBTConfig(...)

DLLEXPORT uint32_t writeGBTPhase(uint32_t ohN, uint32_t vfatN, uint8_t phase){
    req = wisc::RPCMsg("gbt.writeGBTPhase");

    req.set_word("ohN", ohN);
    req.set_word("vfatN", vfatN);
    req.set_word("phase", phase);

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
} //End writeGBTPhase(...)

