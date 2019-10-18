#ifndef GBT_H
#define GBT_H

#include "xhal/rpc/utils.h"

DLLEXPORT uint32_t scanGBTPhases(uint32_t *result, uint32_t ohN, uint32_t nScans=100, uint32_t phaseMin=0, uint32_t phaseMax=15, uint32_t phaseStep=1, uint32_t nVFAT=24, uint32_t nVerificationReads=10);
DLLEXPORT uint32_t writeGBTConfig(uint32_t ohN, uint32_t gbtN, uint32_t configSize, uint8_t *config);
DLLEXPORT uint32_t writeGBTPhase(uint32_t ohN, uint32_t vfatN, uint8_t phase);

#endif
