#ifndef DAQ_MONITOR_H
#define DAQ_MONITOR_H

#include "xhal/rpc/utils.h"

struct OHLinkMonitor{
    uint32_t gbtRdy[3];
    uint32_t gbtNotRdy[3];
    uint32_t gbtRxOverflow[3];
    uint32_t gbtRxUnderflow[3];
};

struct SCAMonitor{
    //Temperature Data
    uint32_t ohBoardTemp[9];
    //uint32_t ohFPGACoreTemp;
    uint32_t scaTemp;

    //Voltages
    uint32_t AVCCN;
    uint32_t AVTTN;
    uint32_t voltage1V0_INT;
    uint32_t voltage1V8F;
    uint32_t voltage1V5;
    uint32_t voltage2V5_IO;
    uint32_t voltage3V0;
    uint32_t voltage1V8;
    uint32_t VTRX_RSSI2;
    uint32_t VTRX_RSSI1;
};

struct SysmonMonitor{
    //Alarms
    bool isOverTemp;
    bool isInVCCAuxAlarm;
    bool isInVCCIntAlarm;

    //Counters
    uint32_t cntOverTemp;
    uint32_t cntVCCAuxAlarm;
    uint32_t cntVCCIntAlarm;

    //Values
    uint32_t fpgaCoreTemp;
    uint32_t fpgaCore1V0;
    uint32_t fpgaCore2V5_IO;
};

struct VFATLinkMonitor{
    uint32_t daqCRCErrCnt[24];
    uint32_t daqEvtCnt[24];
    //uint32_t linkGood[24];
    uint32_t syncErrCnt[24];
};

DLLEXPORT uint32_t getmonTTCmain(uint32_t* result);
DLLEXPORT uint32_t getmonTRIGGERmain(uint32_t* result, uint32_t noh = 12, uint32_t ohMask = 0xfff);
DLLEXPORT uint32_t getmonTRIGGEROHmain(uint32_t* result, uint32_t noh = 12, uint32_t ohMask = 0xff);
DLLEXPORT uint32_t getmonDAQmain(uint32_t* result);
DLLEXPORT uint32_t getmonDAQOHmain(uint32_t* result, uint32_t noh = 12, uint32_t ohMask = 0xfff);
DLLEXPORT uint32_t getmonGBTLink(struct OHLinkMonitor *ohLinkMon, uint32_t noh = 12, uint32_t ohMask = 0xfff, bool doReset = false, uint32_t NGBT=3);
DLLEXPORT uint32_t getmonOHLink(struct OHLinkMonitor *ohLinkMon, struct VFATLinkMonitor *vfatLinkMon, uint32_t noh = 12, uint32_t ohMask = 0xfff, bool doReset = false);
DLLEXPORT uint32_t getmonOHmain(uint32_t* result, uint32_t noh = 12, uint32_t ohMask = 0xfff);
DLLEXPORT uint32_t getmonOHSCAmain(struct SCAMonitor *scaMon, uint32_t noh = 12, uint32_t ohMask = 0xfff);
DLLEXPORT uint32_t getmonOHSysmon(struct SysmonMonitor *sysmon, uint32_t noh =12, uint32_t ohMask = 0xfff, bool doReset=false);
DLLEXPORT uint32_t getmonVFATLink(struct VFATLinkMonitor *vfatLinkMon, uint32_t noh =12, uint32_t ohMask = 0xfff, bool doReset = false);

#endif
