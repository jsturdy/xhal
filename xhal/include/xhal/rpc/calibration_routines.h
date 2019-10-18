#ifndef CALIBRATION_ROUTINES_H
#define CALIBRATION_ROUTINES_H

#include "xhal/rpc/utils.h"

struct vfat3DACSize{
    uint32_t max[41];

    vfat3DACSize(){
        //ADC Measures Current
        //I wonder if this dacMin and dacMax info could be added to the LMDB...?
        //max[0] CFG_IREF should not be scanned per VFAT3 team instructions
        max[1] = 0xff; //CFG_CAL_DAC (as current)
        max[2] = 0xff; //CFG_BIAS_PRE_I_BIT
        max[3] = 0x3f; //CFG_BIAS_PRE_I_BLCC
        max[4] = 0x3f; //CFG_BIAS_PRE_I_BSF
        max[5] = 0xff; //CFG_BIAS_SH_I_BFCAS
        max[6] = 0xff; //CFG_BIAS_SH_I_BDIFF
        max[7] = 0xff; //CFG_BIAS_SD_I_BDIFF
        max[8] = 0xff; //CFG_BIAS_SD_I_BFCAS
        max[9] = 0x3f; //CFG_BIAS_SD_I_BSF
        max[10] = 0x3f; //CFG_BIAS_CFD_DAC_1
        max[11] = 0x3f; //CFG_BIAS_CFD_DAC_2
        max[12] = 0x3f; //CFG_HYST
        max[14] = 0xff; //CFG_THR_ARM_DAC
        max[15] = 0xff; //CFG_THR_ZCC_DAC
        //max[16] = ???

        //ADC Measures Voltage
        max[33] = 0xff; //CFG_CAL_DAC (as voltage)
        max[34] = 0xff; //CFG_BIAS_PRE_VREF
        max[35] = 0xff; //CFG_THR_ARM_DAC
        max[36] = 0xff; //CFG_THR_ZCC_DAC
        max[39] = 0x3; //CFG_ADC_VREF
        //max[41] = ???
    }
};

DLLEXPORT uint32_t checkSbitMappingWithCalPulse(uint32_t ohN, uint32_t vfatN, uint32_t mask, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t nevts, uint32_t L1Ainterval, uint32_t pulseDelay, uint32_t *data);
DLLEXPORT uint32_t checkSbitRateWithCalPulse(uint32_t ohN, uint32_t vfatN, uint32_t mask, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t waitTime, uint32_t pulseRate, uint32_t pulseDelay, uint32_t *outDataCTP7Rate, uint32_t *outDataFPGAClusterCntRate, uint32_t *outDataVFATSBits);
DLLEXPORT uint32_t dacScan(uint32_t ohN, uint32_t dacSelect, uint32_t dacStep, uint32_t mask, bool useExtRefADC, uint32_t * results, uint32_t nvfats=24);
DLLEXPORT uint32_t dacScanMultiLink(uint32_t ohMask, uint32_t NOH, uint32_t dacSelect, uint32_t dacStep, bool useExtRefADC, uint32_t * results, uint32_t nvfats=24);
DLLEXPORT uint32_t genScan(uint32_t nevts, uint32_t ohN, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep,
        uint32_t ch, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t mask, char * scanReg, bool useUltra, bool useExtTrig,
        uint32_t * result, uint32_t nvfats=24);
DLLEXPORT uint32_t genChannelScan(uint32_t nevts, uint32_t ohN, uint32_t mask, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, bool useExtTrig, char * scanReg, bool useUltra, uint32_t * result, uint32_t nvfats=24);
DLLEXPORT uint32_t sbitRateScan(uint32_t ohMask, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, char * scanReg,
                                uint32_t * resultDacVal, uint32_t * resultTrigRate, uint32_t * resultTrigRatePerVFAT, uint32_t nvfats=24, uint32_t waitTime = 1);
DLLEXPORT uint32_t ttcGenConf(uint32_t ohN, uint32_t mode, uint32_t type, uint32_t pulseDelay,
        uint32_t L1Ainterval, uint32_t nPulses, bool enable);
DLLEXPORT uint32_t ttcGenToggle(uint32_t ohN, bool enable);
DLLEXPORT uint32_t confCalPulse(uint32_t ohN, uint32_t mask, uint32_t ch, bool toggleOn, bool currentPulse, uint32_t calScaleFactor);

#endif
