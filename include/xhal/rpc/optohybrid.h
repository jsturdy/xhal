#ifndef OPTOHYBRID_H
#define OPTOHYBRID_H

#include "xhal/rpc/utils.h"

DLLEXPORT uint32_t broadcastRead(uint32_t ohN, char * regName, uint32_t vfatMask, uint32_t * result);
DLLEXPORT uint32_t broadcastWrite(uint32_t ohN, char * regName, uint32_t value, uint32_t vfatMask);
DLLEXPORT uint32_t configureScanModule(uint32_t ohN, uint32_t vfatN, uint32_t scanmode, bool useUltra,
        uint32_t vfatMask, uint32_t ch, uint32_t nevts, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep);
DLLEXPORT uint32_t printScanConfiguration(uint32_t ohN, bool useUltra);
DLLEXPORT uint32_t startScanModule(uint32_t ohN, bool useUltra);
DLLEXPORT uint32_t getUltraScanResults(uint32_t ohN, uint32_t nevts, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t * result);
DLLEXPORT uint32_t stopCalPulse2AllChannels(uint32_t ohN, uint32_t mask, uint32_t ch_min, uint32_t ch_max);

#endif
