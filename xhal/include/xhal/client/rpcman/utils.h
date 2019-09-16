#ifndef XHAL_CLIENT_RPCMAN_UTILS_H
#define XHAL_CLIENT_RPCMAN_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "xhal/client/XHALInterface.h"

namespace xhal {
  namespace client {
    namespace rpcman {
      /**
      * @class Utils
      * @brief Provides interface to call remote utility methods
      */
      class Utils : public xhal::client::XHALInterface
      {
        public:
          /**
           * @brief Default constructor
           *
           * Loads the neccessary remote modules
           * @param board_domain_name domain name of CTP7
           */
          Utils(const std::string& board_domain_name) :
            xhal::client::XHALInterface(board_domain_name)
          {
            this->loadModule("utils", "utils v1.0.1");
          }

          ~Utils() {}

          uint32_t update_atdb(char * xmlfilename);
          uint32_t getRegInfoDB(char * regName);
      };

    }
  }
}
#endif // XHAL_CLIENT_RPCMAN_UTILS_H
