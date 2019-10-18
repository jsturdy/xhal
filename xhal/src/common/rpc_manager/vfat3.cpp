#include <array>
#include "xhal/rpc/vfat3.h"

/***
 * @brief load configuration parameters to VFAT3 chips
 */
DLLEXPORT uint32_t configureVFAT3s(uint32_t ohN, uint32_t vfatMask)
{
    req = wisc::RPCMsg("vfat3.configureVFAT3s");

    req.set_word("vfatMask",vfatMask);
    req.set_word("ohN",ohN);
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

DLLEXPORT uint32_t configureVFAT3DacMonitor(uint32_t ohN, uint32_t vfatMask, uint32_t dacSelect){
    req = wisc::RPCMsg("vfat3.configureVFAT3DacMonitor");

    req.set_word("ohN",ohN);
    req.set_word("vfatMask",vfatMask);
    req.set_word("dacSelect",dacSelect);

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
} //End configureVFAT3DacMonitor(...)

DLLEXPORT uint32_t configureVFAT3DacMonitorMultiLink(uint32_t ohMask, uint32_t *ohVfatMaskArray, uint32_t dacSelect){
    req = wisc::RPCMsg("vfat3.configureVFAT3DacMonitorMultiLink");

    req.set_word("ohMask",ohMask);
    req.set_word_array("ohVfatMaskArray",ohVfatMaskArray,12);
    req.set_word("dacSelect",dacSelect);

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
} //End configureVFAT3DacMonitor(...)

DLLEXPORT uint32_t getChannelRegistersVFAT3(uint32_t ohN, uint32_t vfatMask, uint32_t *chanRegData, uint32_t nvfats){
    req = wisc::RPCMsg("vfat3.getChannelRegistersVFAT3");

    req.set_word("ohN",ohN);
    req.set_word("vfatMask",vfatMask);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    const uint32_t size = 128*nvfats; // nChannels depends on the GEM type
    if (rsp.get_key_exists("chanRegData")) {
        ASSERT(rsp.get_word_array_size("chanRegData") == size);
        rsp.get_word_array("chanRegData", chanRegData);
    } else {
        printf("No channel register data found");
        return 1;
    }

    return 0;
}

DLLEXPORT uint32_t getVFAT3ChipIDs(uint32_t * chipIDData, uint32_t ohN, uint32_t vfatMask, bool rawID, uint32_t nvfats)
{
    req = wisc::RPCMsg("vfat3.getVFAT3ChipIDs");

    req.set_word("ohN",ohN);
    req.set_word("vfatMask",vfatMask);
    req.set_word("rawID",rawID);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    std::string regBase = "GEM_AMC.OH.OH" + std::to_string(ohN) + ".GEB.VFAT";
    for(int vfat=0; vfat < nvfats; ++vfat)
    {
        if((vfatMask >> vfat) & 0x1){
            continue;
        }
        chipIDData[vfat] = rsp.get_word(regBase + std::to_string(vfat) + ".HW_CHIP_ID");
    }

    return 0;
} //End getVFAT3ChipIDs()

DLLEXPORT uint32_t readVFAT3ADC(uint32_t ohN, uint32_t *adcData, bool useExtRefADC, uint32_t vfatMask, uint32_t nvfats){
    req = wisc::RPCMsg("vfat3.readVFAT3ADC");

    req.set_word("ohN",ohN);
    req.set_word("useExtRefADC", useExtRefADC);
    req.set_word("vfatMask",vfatMask);

    wisc::RPCSvc* rpc_loc = getRPCptr();
    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    const uint32_t size = nvfats;

    if (rsp.get_key_exists("adcData")) {
        ASSERT(rsp.get_word_array_size("adcData") == size);
        rsp.get_word_array("adcData", adcData);
    } else {
        printf("No channel register data found");
        return 1;
    }

    return 0;
} //End readVFAT3ADC(...)

DLLEXPORT uint32_t readVFAT3ADCMultiLink(uint32_t ohMask, uint32_t *ohVfatMaskArray, uint32_t *adcDataAll, bool useExtRefADC, uint32_t nvfats){
    req = wisc::RPCMsg("vfat3.readVFAT3ADCMultiLink");

    req.set_word("ohMask",ohMask);
    req.set_word_array("ohVfatMaskArray",ohVfatMaskArray, 12);
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

    const uint32_t size = 12*nvfats;
    if (rsp.get_key_exists("adcDataAll")) {
        ASSERT(rsp.get_word_array_size("adcDataAll") == size);
        rsp.get_word_array("adcDataAll", adcDataAll);
    } else {
        printf("No channel register data found");
        return 1;
    }

    return 0;
} //End readVFAT3ADCMultiLink(...)

DLLEXPORT uint32_t setChannelRegistersVFAT3(uint32_t ohN, uint32_t vfatMask, uint32_t *calEnable, uint32_t *masks, uint32_t *trimARM, uint32_t *trimARMPol, uint32_t *trimZCC, uint32_t *trimZCCPol, uint32_t nvfats){
    req = wisc::RPCMsg("vfat3.setChannelRegistersVFAT3");

    req.set_word("ohN",ohN);
    req.set_word("vfatMask",vfatMask);

    req.set_word_array("calEnable",calEnable,128*nvfats);
    req.set_word_array("masks",masks,128*nvfats);
    req.set_word_array("trimARM",trimARM,128*nvfats);
    req.set_word_array("trimARMPol",trimARMPol,128*nvfats);
    req.set_word_array("trimZCC",trimZCC,128*nvfats);
    req.set_word_array("trimZCCPol",trimZCCPol,128*nvfats);


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


DLLEXPORT uint32_t setChannelRegistersVFAT3Simple(uint32_t ohN, uint32_t vfatMask, uint32_t *chanRegData, uint32_t nvfats){
    req = wisc::RPCMsg("vfat3.setChannelRegistersVFAT3");

    req.set_word("ohN",ohN);
    req.set_word("vfatMask",vfatMask);
    req.set_word("simple",true);

    req.set_word_array("chanRegData",chanRegData,128*nvfats);

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
