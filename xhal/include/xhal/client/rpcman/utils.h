#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "xhal/XHALInterface.h"

namespace xhal {
  namespace rpc {
    /**
    * @class Utils
    * @brief Provides interface to call remote utility methods 
    */
    class Utils : public XHALInterface
    {
      public:
        /**
         * @brief Default constructor
         *
         * Loads the neccessary remote modules
         * @param board_domain_name domain name of CTP7
         */
        Utils(const std::string& board_domain_name):xhal::XHALInterface(board_domain_name){this->loadModule("utils", "utils v1.0.1");}
        ~Utils(){}
        uint32_t update_atdb(char * xmlfilename);
        uint32_t getRegInfoDB(char * regName);
    }; 
 
  }
}
#endif
