#include "xhal/rpc/daq_monitor.h"

/***
 * @brief get an array of values for TTC main monitoring table
 */
DLLEXPORT uint32_t getmonTTCmain(uint32_t* result)
{
    req = wisc::RPCMsg("daq_monitor.getmonTTCmain");
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
DLLEXPORT uint32_t getmonTRIGGERmain(uint32_t* result, uint32_t noh, uint32_t ohMask)
{
    req = wisc::RPCMsg("daq_monitor.getmonTRIGGERmain");
    req.set_word("NOH",noh);
    req.set_word("ohMask", ohMask);
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
            for (unsigned int ohN = 0; ohN < noh; ohN++) {
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }
                t = "OH"+std::to_string(ohN)+".TRIGGER_RATE";
                result[ohN+1] = rsp.get_word(t);
            }
        }
    }
    STANDARD_CATCH;
    return 0;
}

/***
 * @brief get an array of values for TRIGGER OH main monitoring table
 */
DLLEXPORT uint32_t getmonTRIGGEROHmain(uint32_t* result, uint32_t noh, uint32_t ohMask)
{
    req = wisc::RPCMsg("daq_monitor.getmonTRIGGEROHmain");
    req.set_word("NOH",noh);
    req.set_word("ohMask", ohMask);
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
            for (unsigned int ohN = 0; ohN < noh; ohN++) {
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }
                t = "OH"+std::to_string(ohN)+".LINK0_MISSED_COMMA_CNT";
                result[ohN] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".LINK1_MISSED_COMMA_CNT";
                result[ohN+noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".LINK0_OVERFLOW_CNT";
                result[ohN+2*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".LINK1_OVERFLOW_CNT";
                result[ohN+3*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".LINK0_UNDERFLOW_CNT";
                result[ohN+4*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".LINK1_UNDERFLOW_CNT";
                result[ohN+5*noh] = rsp.get_word(t);
                 t = "OH"+std::to_string(ohN)+".LINK0_SBIT_OVERFLOW_CNT";
                result[ohN+6*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".LINK1_SBIT_OVERFLOW_CNT";
                result[ohN+7*noh] = rsp.get_word(t);
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
    req = wisc::RPCMsg("daq_monitor.getmonDAQmain");
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
DLLEXPORT uint32_t getmonDAQOHmain(uint32_t* result, uint32_t noh, uint32_t ohMask)
{
    req = wisc::RPCMsg("daq_monitor.getmonDAQOHmain");
    req.set_word("NOH",noh);
    req.set_word("ohMask", ohMask);
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
            for (unsigned int ohN = 0; ohN < noh; ohN++) {
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }
                t = "OH"+std::to_string(ohN)+".STATUS.EVT_SIZE_ERR";
                result[ohN] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".STATUS.EVENT_FIFO_HAD_OFLOW";
                result[ohN+noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".STATUS.INPUT_FIFO_HAD_OFLOW";
                result[ohN+2*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".STATUS.INPUT_FIFO_HAD_UFLOW";
                result[ohN+3*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".STATUS.VFAT_TOO_MANY";
                result[ohN+4*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".STATUS.VFAT_NO_MARKER";
                result[ohN+5*noh] = rsp.get_word(t);
            }
        }
    }
    STANDARD_CATCH;
    return 0;
}

DLLEXPORT uint32_t getmonGBTLink(struct OHLinkMonitor *ohLinkMon, uint32_t noh, uint32_t ohMask, bool doReset, uint32_t NGBT)
{
    req = wisc::RPCMsg("daq_monitor.getmonGBTLink");
    req.set_word("NOH",noh);
    req.set_word("doReset",doReset);
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
            for(unsigned int ohN = 0; ohN < noh; ++ohN){
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }

                std::string strOHN = "OH" + std::to_string(ohN) + ".";

                for(int gbtN = 0; gbtN < NGBT; ++gbtN){
                    std::string strGBTN = "GBT" + std::to_string(gbtN) + ".";

                    ohLinkMon[ohN].gbtRdy[gbtN] = rsp.get_word(strOHN + strGBTN + "READY");
                    ohLinkMon[ohN].gbtNotRdy[gbtN] = rsp.get_word(strOHN + strGBTN + "WAS_NOT_READY");
                    ohLinkMon[ohN].gbtRxOverflow[gbtN] = rsp.get_word(strOHN + strGBTN + "RX_HAD_OVERFLOW");
                    ohLinkMon[ohN].gbtRxUnderflow[gbtN] = rsp.get_word(strOHN + strGBTN + "RX_HAD_UNDERFLOW");
                } //End Loop Over GBT
            } //End Loop Over OH
        } //End Case: No Error
    } //End try block
    STANDARD_CATCH;

    return 0;
} //End getmonGBTLink()

DLLEXPORT uint32_t getmonOHLink(struct OHLinkMonitor *ohLinkMon, struct VFATLinkMonitor *vfatLinkMon, uint32_t noh, uint32_t ohMask, bool doReset)
{
    req = wisc::RPCMsg("daq_monitor.getmonOHLink");
    req.set_word("NOH",noh);
    req.set_word("doReset",doReset);
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
            for(unsigned int ohN = 0; ohN < noh; ++ohN){
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }

                std::string strOHN = "OH" + std::to_string(ohN) + ".";

                for(int gbtN = 0; gbtN < 3; ++gbtN){
                    std::string strGBTN = "GBT" + std::to_string(gbtN) + ".";

                    ohLinkMon[ohN].gbtRdy[gbtN] = rsp.get_word(strOHN + strGBTN + "READY");
                    ohLinkMon[ohN].gbtNotRdy[gbtN] = rsp.get_word(strOHN + strGBTN + "WAS_NOT_READY");
                    ohLinkMon[ohN].gbtRxOverflow[gbtN] = rsp.get_word(strOHN + strGBTN + "RX_HAD_OVERFLOW");
                    ohLinkMon[ohN].gbtRxUnderflow[gbtN] = rsp.get_word(strOHN + strGBTN + "RX_HAD_UNDERFLOW");
                } //End Loop Over GBT

                for(int vfatN = 0; vfatN < 24; ++vfatN){
                    std::string strVFATN = "VFAT" + std::to_string(vfatN) + ".";

                    vfatLinkMon[ohN].daqCRCErrCnt[vfatN] = rsp.get_word(strOHN + strVFATN + "SYNC_ERR_CNT");
                    vfatLinkMon[ohN].daqEvtCnt[vfatN] = rsp.get_word(strOHN + strVFATN + "DAQ_EVENT_CNT");
                    vfatLinkMon[ohN].syncErrCnt[vfatN] = rsp.get_word(strOHN + strVFATN + "DAQ_CRC_ERROR_CNT");
                } //End Loop Over VFAT
            } //End Loop Over OH
        } //End Case: No Error
    } //End try block
    STANDARD_CATCH;

    return 0;
} //End getmonOHLink()

/***
 * @brief get an array of values for OH main monitoring table
 */
DLLEXPORT uint32_t getmonOHmain(uint32_t* result, uint32_t noh, uint32_t ohMask)
{
    req = wisc::RPCMsg("daq_monitor.getmonOHmain");
    req.set_word("NOH",noh);
    req.set_word("ohMask", ohMask);
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
            for (unsigned int ohN = 0; ohN < noh; ohN++) {
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }

                t = "OH"+std::to_string(ohN)+".FW_VERSION";
                result[ohN] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".EVENT_COUNTER";
                result[ohN+noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".EVENT_RATE";
                result[ohN+2*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".GTX.TRK_ERR";
                result[ohN+3*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".GTX.TRG_ERR";
                result[ohN+4*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".GBT.TRK_ERR";
                result[ohN+5*noh] = rsp.get_word(t);
                t = "OH"+std::to_string(ohN)+".CORR_VFAT_BLK_CNT";
                result[ohN+6*noh] = rsp.get_word(t);
            }
        }
    }
    STANDARD_CATCH;
    return 0;
}

DLLEXPORT uint32_t getmonOHSCAmain(struct SCAMonitor *scaMon, uint32_t noh, uint32_t ohMask){
    req = wisc::RPCMsg("daq_monitor.getmonOHSCAmain");
    req.set_word("NOH",noh);
    req.set_word("ohMask", ohMask);
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
            for (unsigned int ohN = 0; ohN < noh; ++ohN) {
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }

                std::string strKeyBase = "OH" + std::to_string(ohN);

                // Temperature Values
                scaMon[ohN].scaTemp = rsp.get_word(strKeyBase + ".SCA_TEMP");
                for(int tempVal=1; tempVal <= 9; ++tempVal){
                    scaMon[ohN].ohBoardTemp[tempVal-1] = rsp.get_word(strKeyBase + ".BOARD_TEMP" + std::to_string(tempVal));
                } //End loop over temperature values

                // Voltage Values
                scaMon[ohN].AVCCN = rsp.get_word(strKeyBase + ".AVCCN");
                scaMon[ohN].AVTTN = rsp.get_word(strKeyBase + ".AVTTN");

                scaMon[ohN].voltage1V0_INT = rsp.get_word(strKeyBase + ".1V0_INT");
                scaMon[ohN].voltage1V8F = rsp.get_word(strKeyBase + ".1V8F");
                scaMon[ohN].voltage1V5 = rsp.get_word(strKeyBase + ".1V5");
                scaMon[ohN].voltage2V5_IO = rsp.get_word(strKeyBase + ".2V5_IO");
                scaMon[ohN].voltage3V0 = rsp.get_word(strKeyBase + ".3V0");
                scaMon[ohN].voltage1V8 = rsp.get_word(strKeyBase + ".1V8");

                scaMon[ohN].VTRX_RSSI2 = rsp.get_word(strKeyBase + ".VTRX_RSSI2");
                scaMon[ohN].VTRX_RSSI1 = rsp.get_word(strKeyBase + ".VTRX_RSSI1");
            } //End loop over OH's
        } //End case no error key
    } //End try
    STANDARD_CATCH;
    return 0;
} //End getmonOHSCAmain()

DLLEXPORT uint32_t getmonOHSysmon(struct SysmonMonitor *sysmon, uint32_t noh, uint32_t ohMask, bool doReset){
    req = wisc::RPCMsg("daq_monitor.getmonOHSysmon");
    req.set_word("NOH",noh);
    req.set_word("ohMask", ohMask);
    req.set_word("doReset", doReset);
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
            for (unsigned int ohN = 0; ohN < noh; ++ohN) {
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }

                std::string strKeyBase = "OH" + std::to_string(ohN);

                //Read Alarm conditions & counters - OVERTEMP
                try{
                    sysmon[ohN].isOverTemp = rsp.get_word(strKeyBase + ".OVERTEMP");
                    sysmon[ohN].cntOverTemp = rsp.get_word(strKeyBase + ".CNT_OVERTEMP");
                }
                STANDARD_CATCH;

                //Read Alarm conditions & counters - VCCAUX_ALARM
                try{
                    sysmon[ohN].isInVCCAuxAlarm = rsp.get_word(strKeyBase + ".VCCAUX_ALARM");
                    sysmon[ohN].cntVCCAuxAlarm = rsp.get_word(strKeyBase + ".CNT_VCCAUX_ALARM");
                }
                STANDARD_CATCH;

                //Read Alarm conditions & counters - VCCINT_ALARM
                try{
                    sysmon[ohN].isInVCCIntAlarm = rsp.get_word(strKeyBase + ".VCCINT_ALARM");
                    sysmon[ohN].cntVCCIntAlarm = rsp.get_word(strKeyBase + ".CNT_VCCINT_ALARM");
                }
                STANDARD_CATCH;

                //Read FPGA Core
                try{
                    sysmon[ohN].fpgaCoreTemp = rsp.get_word(strKeyBase + ".FPGA_CORE_TEMP");
                    sysmon[ohN].fpgaCore1V0 = rsp.get_word(strKeyBase + ".FPGA_CORE_1V0");
                    sysmon[ohN].fpgaCore2V5_IO = rsp.get_word(strKeyBase + ".FPGA_CORE_2V5_IO");
                }
                STANDARD_CATCH;
            } //End loop over OH's
        } //End case no error key
    } //End try
    STANDARD_CATCH;

    return 0;
} //End getmonOHSysmon()

DLLEXPORT uint32_t getmonVFATLink(struct VFATLinkMonitor *vfatLinkMon, uint32_t noh, uint32_t ohMask, bool doReset)
{
    req = wisc::RPCMsg("daq_monitor.getmonVFATLink");
    req.set_word("NOH",noh);
    req.set_word("doReset",doReset);
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
            for(unsigned int ohN = 0; ohN < noh; ++ohN){
                // If this Optohybrid is masked skip it
                if(!((ohMask >> ohN) & 0x1)){
                    continue;
                }

                std::string strOHN = "OH" + std::to_string(ohN) + ".";

                for(int vfatN = 0; vfatN < 24; ++vfatN){
                    std::string strVFATN = "VFAT" + std::to_string(vfatN) + ".";

                    if (rsp.get_key_exists(strOHN + strVFATN + "DAQ_CRC_ERROR_CNT")) {
                        vfatLinkMon[ohN].daqCRCErrCnt[vfatN] = rsp.get_word(strOHN + strVFATN + "DAQ_CRC_ERROR_CNT");
                        vfatLinkMon[ohN].daqEvtCnt[vfatN] = rsp.get_word(strOHN + strVFATN + "DAQ_EVENT_CNT");
                        vfatLinkMon[ohN].syncErrCnt[vfatN] = rsp.get_word(strOHN + strVFATN + "SYNC_ERR_CNT");
                    } else {
                        vfatLinkMon[ohN].daqCRCErrCnt[vfatN] = 0xdeaddead;
                        vfatLinkMon[ohN].daqEvtCnt[vfatN] = 0xdeaddead;
                        vfatLinkMon[ohN].syncErrCnt[vfatN] = 0xdeaddead;
                    }
                } //End Loop Over VFAT
            } //End Loop Over OH
        } //End Case: No Error
    } //End try block
    STANDARD_CATCH;

    return 0;
} //End getmonVFATLink()
