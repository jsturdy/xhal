#ifndef XHAL_CLIENT_RPCMAN_CALIBRATION_ROUTINES_H
#define XHAL_CLIENT_RPCMAN_CALIBRATION_ROUTINES_H

#include "xhal/client/XHALInterface.h"

namespace xhal {
  namespace client {
    namespace rpcman {
      /**
      * @class CalRoutines
      * @brief Provides interface to call remote calibration_routines methods
      */
      class CalRoutines : public xhal::client::XHALInterface
      {
        public:
          /**
           * @brief Default constructor
           *
           * Loads the neccessary remote modules
           * @param board_domain_name domain name of CTP7
           */
          CalRoutines(const std::string& board_domain_name) :
            xhal::client::XHALInterface(board_domain_name)
          {
            this->loadModule("calibration_routines", "calibration_routines v1.0.1");
          }

          ~CalRoutines() {}

          //FIXME Add documentation
          uint32_t checkSbitMappingWithCalPulse(uint32_t ohN, uint32_t vfatN, uint32_t mask, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t nevts, uint32_t L1Ainterval, uint32_t pulseDelay, uint32_t *data);

          //FIXME Add documentation
          uint32_t checkSbitRateWithCalPulse(uint32_t ohN, uint32_t vfatN, uint32_t mask, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t waitTime, uint32_t pulseRate, uint32_t pulseDelay, uint32_t *outDataCTP7Rate, uint32_t *outDataFPGAClusterCntRate, uint32_t *outDataVFATSBits);

          /**
           * @brief Runs a generic scan routine for a specific channel of a VFAT chip
           *
           * @param nevts Number of events per scan point
           * @param ohN Optical link
           * @param dacMin Min value of scanned variable
           * @param dacMax Max value of scanned variable
           * @param dacStep Step parameter
           * @param ch VFAT channel
           * @param useCalPulse Indicates whether to use internal calibration pulses
           * @param currentPulse Indicates whether to use current or voltage internal calibration pulse
           * @param calScaleFactor FIXME
           * @param mask FIXME
           * @param scanReg Register to scan FIXME: add possible values
           * @param useUltra Indicates whether to use FW-implemented ultraScan
           * @param useExtTrig FIXME: do we need both this and useCalPulse?
           * @param result An array carrying scan results
           */
          uint32_t genScan(uint32_t nevts, uint32_t ohN, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, uint32_t mask, char * scanReg, bool useUltra, bool useExtTrig, uint32_t * result);

          /**
           * @brief Runs a generic scan routine on all channels of a VFAT chip
           *
           * @param nevts Number of events per scan point
           * @param ohN Optical link
           * @param dacMin Min value of scanned variable
           * @param dacMax Max value of scanned variable
           * @param dacStep Step parameter
           * @param useCalPulse Indicates whether to use internal calibration pulses
           * @param currentPulse Indicates whether to use current or voltage internal calibration pulse
           * @param calScaleFactor FIXME
           * @param mask FIXME
           * @param scanReg Register to scan FIXME: add possible values
           * @param useUltra Indicates whether to use FW-implemented ultraScan
           * @param useExtTrig FIXME: do we need both this and useCalPulse?
           * @param result An array carrying scan results
           */
          //FIXME Should we rearrange parameters so they are in the same order as in genScan?
          uint32_t genChannelScan(uint32_t nevts, uint32_t ohN, uint32_t mask, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, bool useCalPulse, bool currentPulse, uint32_t calScaleFactor, bool useExtTrig, char * scanReg, bool useUltra, uint32_t * result);

          //FIXME Add documentation
          uint32_t sbitRateScan(uint32_t ohN, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t ch, uint32_t maskOh, bool invertVFATPos, char * scanReg, uint32_t waitTime, uint32_t * resultDacVal, uint32_t * resultTrigRate, uint32_t * resultTrigRatePerVFAT, bool isParallel);

          /**
           * @brief configure TTC generator
           *
           * v3  electronics Behavior:
           *      pulseDelay (only for enable = true), delay between CalPulse and L1A
           *      L1Ainterval (only for enable = true), how often to repeat signals
           *      enable = true (false) ignore (take) ttc commands from backplane for this AMC (affects all links)
           * v2b electronics behavior:
           *      Configure the T1 controller
           *      mode: 0 (Single T1 signal),
           *            1 (CalPulse followed by L1A),
           *            2 (pattern)
           *      type (only for mode 0, type of T1 signal to send):
           *            0 L1A
           *            1 CalPulse
           *            2 Resync
           *            3 BC0
           *      pulseDelay (only for mode 1), delay between CalPulse and L1A
           *      L1Ainterval (only for mode 0,1), how often to repeat signals
           *      nPulses how many signals to send (0 is continuous)
           *      enable = true (false) start (stop) the T1Controller for link ohN
           */
          uint32_t ttcGenConf(uint32_t ohN, uint32_t mode, uint32_t type, uint32_t pulseDelay, uint32_t L1Ainterval, uint32_t nPulses, bool enable);

          /**
           * @brief Toggles TTC behavior
           *
           * v3  electronics: enable = true (false) ignore (take) ttc commands from backplane for this AMC
           * v2b electronics: enable = true (false) start (stop) the T1Controller for link ohN
           */
          uint32_t ttcGenToggle(uint32_t ohN, bool enable);
      };
    }
  }
}
#endif // XHAL_CLIENT_RPCMAN_CALIBRATION_ROUTINES_H
