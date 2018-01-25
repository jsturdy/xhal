#ifndef DAQ_MONITOR_H
#define DAQ_MONITOR_H

#include "xhal/rpc/utils.h"

DLLEXPORT uint32_t getmonTTCmain(uint32_t* result);
DLLEXPORT uint32_t getmonTRIGGERmain(uint32_t* result, uint32_t noh = 12);
DLLEXPORT uint32_t getmonTRIGGEROHmain(uint32_t* result, uint32_t noh = 12);
DLLEXPORT uint32_t getmonDAQmain(uint32_t* result);
DLLEXPORT uint32_t getmonDAQOHmain(uint32_t* result, uint32_t noh = 12);
DLLEXPORT uint32_t getmonOHmain(uint32_t* result, uint32_t noh = 12);

#endif
