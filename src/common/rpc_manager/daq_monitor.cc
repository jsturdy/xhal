#include "xhal/rpc/daq_monitor.h"

/***
 * @brief get an array of values for TTC main monitoring table
 */
DLLEXPORT uint32_t getmonTTCmain(uint32_t* result)
{
    req = wisc::RPCMsg("amc.getmonTTCmain");
    wisc::RPCSvc* rpc_loc = getRPCptr();
    try {
    	rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;
    
    try{
        if (rsp.get_key_exists("error")) {
            printf("Error: %s",rsp.get_string("error").c_str());
            return 1;
        } else {
            result[0] = rsp.get_word("MMCM_LOCKED");
            result[1] = rsp.get_word("TTC_SINGLE_ERROR_CNT");
            result[2] = rsp.get_word("BC0_LOCKED");
            result[3] = rsp.get_word("L1A_ID");
            result[4] = rsp.get_word("L1A_RATE");
        }
    }
    STANDARD_CATCH;
    return 0;
}

/***
 * @brief get an array of values for TRIGGER main monitoring table
 */
DLLEXPORT uint32_t getmonTRIGGERmain(uint32_t* result, uint32_t noh)
{
    req = wisc::RPCMsg("amc.getmonTRIGGERmain");
    req.set_word("NOH",noh);
    wisc::RPCSvc* rpc_loc = getRPCptr();
    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    try{
        if (rsp.get_key_exists("error")) {
            printf("Error: %s",rsp.get_string("error").c_str());
            return 1;
        } else {
            std::string t;
            result[0] = rsp.get_word("OR_TRIGGER_RATE");
            for (int i = 0; i < noh; i++) {
                t = "OH"+std::to_string(i)+".TRIGGER_RATE";
                result[i+1] = rsp.get_word(t);
            }
        }
    }
    STANDARD_CATCH;
    return 0;
}

/***
 * @brief get an array of values for TRIGGER OH main monitoring table
 */
DLLEXPORT uint32_t getmonTRIGGEROHmain(uint32_t* result, uint32_t noh)
{
    req = wisc::RPCMsg("amc.getmonTRIGGEROHmain");
    req.set_word("NOH",noh);
    wisc::RPCSvc* rpc_loc = getRPCptr();
    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    try{
        if (rsp.get_key_exists("error")) {
            printf("Error: %s",rsp.get_string("error").c_str());
            return 1;
        }else {
            std::string t;
            for (int i = 0; i < noh; i++) {
                t = "OH"+std::to_string(i)+".LINK0_NOT_VALID_CNT";
                result[i] = rsp.get_word(t);
                t = "OH"+std::to_string(i)+".LINK1_NOT_VALID_CNT";
                result[i+noh] = rsp.get_word(t);
            }
        }
    }
    STANDARD_CATCH;
    return 0;
}

/***
 * @brief get an array of values for DAQ main monitoring table
 */
DLLEXPORT uint32_t getmonDAQmain(uint32_t* result)
{
    req = wisc::RPCMsg("amc.getmonDAQmain");
    wisc::RPCSvc* rpc_loc = getRPCptr();
    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    try{
        if (rsp.get_key_exists("error")) {
            printf("Error: %s",rsp.get_string("error").c_str());
            return 1;
        } else {
            result[0] = rsp.get_word("DAQ_ENABLE");
            result[1] = rsp.get_word("DAQ_LINK_READY");
            result[2] = rsp.get_word("DAQ_LINK_AFULL");
            result[3] = rsp.get_word("DAQ_OFIFO_HAD_OFLOW");
            result[4] = rsp.get_word("L1A_FIFO_HAD_OFLOW");
            result[5] = rsp.get_word("L1A_FIFO_DATA_COUNT");
            result[6] = rsp.get_word("DAQ_FIFO_DATA_COUNT");
            result[7] = rsp.get_word("EVENT_SENT");
            result[8] = rsp.get_word("TTS_STATE");
        }
    }
    STANDARD_CATCH;
    return 0;
}

/***
 * @brief get an array of values for DAQ OH main monitoring table
 */
DLLEXPORT uint32_t getmonDAQOHmain(uint32_t* result, uint32_t noh)
{
    req = wisc::RPCMsg("amc.getmonDAQOHmain");
    req.set_word("NOH",noh);
    wisc::RPCSvc* rpc_loc = getRPCptr();
    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    try{
        if (rsp.get_key_exists("error")) {
            printf("Error: %s",rsp.get_string("error").c_str());
            return 1;
        }else {
            std::string t;
            for (int i = 0; i < noh; i++) {
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
    }
    STANDARD_CATCH;
    return 0;
}

/***
 * @brief get an array of values for OH main monitoring table
 */
DLLEXPORT uint32_t getmonOHmain(uint32_t* result, uint32_t noh)
{
    req = wisc::RPCMsg("amc.getmonOHmain");
    req.set_word("NOH",noh);
    wisc::RPCSvc* rpc_loc = getRPCptr();
    try {
        rsp = rpc_loc->call_method(req);
    }
    STANDARD_CATCH;

    try{
        if (rsp.get_key_exists("error")) {
            printf("Error: %s",rsp.get_string("error").c_str());
            return 1;
        } else {
            std::string t;
            for (int i = 0; i < noh; i++) {
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
    }
    STANDARD_CATCH;
    return 0;
}
