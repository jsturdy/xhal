#ifndef AMC_H
#define AMC_H

#include "xhal/rpc/utils.h"
#include "xhal/utils/Exception.h"

#include<vector>
#include<string>

/*! \fn DLLEXPORT uint32_t getOHVFATMask(uint32_t ohN, uint32_t * vfatMask)
 *  \brief Determines the vfatMask for optohybrid ohN.
 *  \details Reads the SYNC_ERR_CNT counter for each VFAT on ohN.  If for a given VFAT the counter returns a non-zero value the given VFAT will be masked.
 *  \param ohN Optohybrid optical link number
 *  \returns uint32_t where bits [23:0] represent the vfat mask.  If the "error" key exists in the RPC response returns instead overflow (0xFFFFFFFF)
 */
DLLEXPORT uint32_t getOHVFATMask(uint32_t ohN);

/*! \fn DLLEXPORT uint32_t getOHVFATMaskMultiLink(uint32_t ohMask, uint32_t * ohVfatMaskArray)
 *  \brief As getOHVFATMask(...) but for all optohybrids specified in ohMask
 *  \param ohMask A 12 bit number which specifies which optohybrids to read from.  Having a value of 1 in the n^th bit indicates that the n^th optohybrid should be considered.
 *  \param ohVfatMaskArray Pointer to an array of length 12.  After the call completes each element will be the bitmask of chip positions determining which chips to use for the optohybrid number corresponding to the element index.
 */
DLLEXPORT uint32_t getOHVFATMaskMultiLink(uint32_t ohMask, uint32_t * ohVfatMaskArray);

/*! \fn DLLEXPORT uint32_t repeatedRegRead( const char * regName, uint32_t nReads, bool breakOnFailure)
 *  \brief repeatedly reads the register regName up to nReads number of times
 *  \param regName full node name of the register to be read
 *  \param nReads maximum number of read attempts to be performed
 *  \param breakOnFailure if true the read process will stop before nReads is reached if a single read fails
 *  \returns sum of VFAT slow control error counters
 */
DLLEXPORT uint32_t repeatedRegRead( const char * regName, uint32_t nReads=1000, bool breakOnFailure=true);

/*! \fn DLLEXPORT uint32_t sbitReadOut(uint32_t ohN, uint32_t acquireTime, uint32_t * approxLiveTime, bool * maxNetworkSizeReached, uint32_t * storedSbits)
 *  \brief SBIT readout from optohybrid ohN for a number of seconds given by acquireTime; data is written to a file directory specified by outFilePath
 *  \param ohN Optohybrid optical link number
 *  \param acquireTime acquisition time on the wall clock in seconds
 *  \return An integer from the set {0, 1, EIO}; where 0 indicates successful completion, 1 indicates an RPC error, and EIO is a platform dependent error code representing a file IO error
 */
DLLEXPORT uint32_t sbitReadOut(uint32_t ohN, uint32_t acquireTime, char * outFilePath);

#endif
