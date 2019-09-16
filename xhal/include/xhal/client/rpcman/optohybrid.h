#ifndef XHAL_CLIENT_RPCMAN_OPTOHYBRID_H
#define XHAL_CLIENT_RPCMAN_OPTOHYBRID_H

#include "xhal/client/XHALDevice.h"

namespace xhal {
  namespace client {
    namespace rpcman {
      /**
      * @class Optohybrid
      * @brief Provides interface to call remote optohybrid methods
      */
      class Optohybrid : public xhal::client::XHALDevice
      {
        public:
        /**
         * @brief Default constructor
         *
         * Parses XML file and loads required modules
         * @param board_domain_name domain name of CTP7
         * @param address_table_filename XML address table file name
         */
        Optohybrid(const std::string& board_domain_name, const std::string& address_table_filename);

        //FIXME provide documentation
        uint32_t broadcastRead(uint32_t ohN, char * regName, uint32_t vfatMask, uint32_t * result);
        uint32_t broadcastWrite(uint32_t ohN, char * regName, uint32_t value, uint32_t vfatMask);
        uint32_t configureScanModule(uint32_t ohN, uint32_t vfatN, uint32_t scanmode, bool useUltra, uint32_t vfatMask, uint32_t ch, uint32_t nevts, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep);
        uint32_t printScanConfiguration(uint32_t ohN, bool useUltra);
        uint32_t startScanModule(uint32_t ohN, bool useUltra);
        uint32_t getUltraScanResults(uint32_t ohN, uint32_t nevts, uint32_t dacMin, uint32_t dacMax, uint32_t dacStep, uint32_t * result);
        uint32_t stopCalPulse2AllChannels(uint32_t ohN, uint32_t mask, uint32_t ch_min, uint32_t ch_max);
      };
    }
  }
}

#endif // XHAL_CLIENT_RPCMAN_OPTOHYBRID_H
