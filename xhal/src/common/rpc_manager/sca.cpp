#include "xhal/rpc/sca.h"

DLLEXPORT uint32_t readSCAADCSensor(const uint32_t ohMask, const uint32_t ch, uint32_t* result)
{
    req = wisc::RPCMsg("amc.readSCAADCSensor");

    req.set_word("ohMask", ohMask);
    req.set_word("ch", ch);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }
    const uint32_t size = count_1bits(ohMask);

    if (rsp.get_key_exists("data")){
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data",result);
    }
    else{
        printf("No key found for data");
        return 1;
    }

    return 0;
} //End readSCAADCSensor(...)

DLLEXPORT uint32_t readSCAADCTemperatureSensors(const uint32_t ohMask, uint32_t* result)
{
    req = wisc::RPCMsg("sca.readSCAADCTemperatureSensors");

    req.set_word("ohMask", ohMask);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    const uint32_t size = count_1bits(ohMask)*5;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    if (rsp.get_key_exists("data")){
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data",result);
    }
    else{
        printf("No key found for data");
        return 1;
    }

    return 0;
} //End readSCAADCTemperatureSensors(...)

DLLEXPORT uint32_t readSCAADCVoltageSensors(const uint32_t ohMask, uint32_t* result)
{
    req = wisc::RPCMsg("sca.readSCAADCVoltageSensors");

    req.set_word("ohMask", ohMask);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    const uint32_t size = count_1bits(ohMask)*6;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    if (rsp.get_key_exists("data")){
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data",result);
    }
    else{
        printf("No key found for data");
        return 1;
    }
 
    return 0;
} //End readSCAADCVoltageSensors(...)

DLLEXPORT uint32_t readSCAADCSignalStrengthSensors(const uint32_t ohMask, uint32_t* result)
{
    req = wisc::RPCMsg("sca.readSCAADCSignalStrengthSensors");

    req.set_word("ohMask", ohMask);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    const uint32_t size = count_1bits(ohMask)*3;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    if (rsp.get_key_exists("data")){
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data",result);
    }
    else{
        printf("No key found for data");
        return 1;
    }

    return 0;
} //End readSCAADCSignalStrengthSensors(...)

DLLEXPORT uint32_t readAllSCAADCSensors(const uint32_t ohMask, uint32_t* result)
{
    req = wisc::RPCMsg("sca.readAllSCAADCSensors");

    req.set_word("ohMask", ohMask);

    wisc::RPCSvc* rpc_loc = getRPCptr();

    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    const uint32_t size = count_1bits(ohMask)*14;

    if (rsp.get_key_exists("error")) {
        printf("Caught an error: %s\n", (rsp.get_string("error")).c_str());
        return 1;
    }

    if (rsp.get_key_exists("data")){
        ASSERT(rsp.get_word_array_size("data") == size);
        rsp.get_word_array("data",result);
    }
    else{
        printf("No key found for data");
        return 1;
    }

    return 0;
} //End readAllSCAADCSensors(...)

