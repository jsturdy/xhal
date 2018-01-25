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
