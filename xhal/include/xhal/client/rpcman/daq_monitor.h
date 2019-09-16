#ifndef XHAL_CLIENT_RPCMAN_DAQ_MONITOR_H
#define XHAL_CLIENT_RPCMAN_DAQ_MONITOR_H

#include "xhal/client/XHALInterface.h"
#include "xhal/client/utils/PyTypes.h"

namespace xhal {
  namespace client {
    namespace rpcman {
      /**
      * @class DaqMonitor
      * @brief Provides interface to call remote daq_monitor methods
      */
      class DaqMonitor : public xhal::client::XHALInterface
      {
        public:
          /**
           * @brief Default constructor
           *
           * Loads the neccessary remote modules
           * @param board_domain_name domain name of CTP7
           */
          DaqMonitor(const std::string& board_domain_name) :
            xhal::client::XHALInterface(board_domain_name)
          {
            this->loadModule("amc", "amc v1.0.1");
          }

          ~DaqMonitor() {}

          /**
           * @brief get an array of values for TTC main monitoring table
           *
           * @return an array of monitoring values
           */
          //uint32_t getmonTTCmain(uint32_t* result);
          PyListUint32 getmonTTCmain();

          /**
           * @brief get an array of values for TRIGGER main monitoring table
           *
           * @param noh Number of expected optical links, default value 12
           * @return an array of monitoring values
           */
          PyListUint32 getmonTRIGGERmain(uint32_t noh = 12);

          /**
           * @brief get an array of values for TRIGGER OH main monitoring table
           *
           * @param noh Number of expected optical links, default value 12
           * @return an array of monitoring values
           */
          PyListUint32 getmonTRIGGEROHmain(uint32_t noh = 12);

          /**
           * @brief get an array of values for DAQ main monitoring table
           *
           * @return an array of monitoring values
           */
          PyListUint32 getmonDAQmain();

          /**
           * @brief get an array of values for DAQ OH main monitoring table
           *
           * @param noh Number of expected optical links, default value 12
           * @return an array of monitoring values
           */
          PyListUint32 getmonDAQOHmain(uint32_t noh = 12);

          /**
           * @brief get an array of values for OH main monitoring table
           *
           * @param noh Number of expected optical links, default value 12
           * @return an array of monitoring values
           */
          PyListUint32 getmonOHmain(uint32_t noh = 12);
      };
    }
  }
}

#endif // XHAL_CLIENT_RPCMAN_DAQ_MONITOR_H
