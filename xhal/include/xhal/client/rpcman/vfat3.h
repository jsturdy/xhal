#ifndef XHAL_CLIENT_RPCMAN_VFAT3_H
#define XHAL_CLIENT_RPCMAN_VFAT3_H

#include "xhal/client/XHALDevice.h"

namespace xhal {
  namespace client {
    namespace rpcman {
      /**
      * @class VFAT3
      * @brief Provides interface to call remote vfat3 methods
      */
      class VFAT3 : public xhal::client::XHALDevice
      {
        public:
        /**
         * @brief Default constructor
         *
         * Parses XML file and loads required modules
         * @param board_domain_name domain name of CTP7
         * @param address_table_filename XML address table file name
         */
        VFAT3(const std::string& board_domain_name, const std::string& address_table_filename);

        //FIXME provide docs
        /**
         * @brief load configuration parameters to VFAT3 chips
         */
        uint32_t configureVFAT3s(uint32_t ohN, uint32_t vfatMask);

        /**
         *  @brief reads channel registers of all unmasked vfats on ohN
         *  @param ohN Optohybrid optical link number
         *  @param vfatMask Bitmask of chip positions determining which chips to use
         *  @param chanRegData array pointer for channel register data with 3072 entries, the (vfat,chan) pairing determines the array index via: idx = vfat*128 + chan
         */
        uint32_t getChannelRegistersVFAT3(uint32_t ohN, uint32_t vfatMask, uint32_t *chanRegData);

        /**
         *  @brief sets all vfat3 channel registers
         *  @param ohN Optohybrid optical link number
         *  @param vfatMask Bitmask of chip positions determining which chips to use
         *  @param calEnable array pointer for calEnable with 3072 entries, the (vfat,chan) pairing determines the array index via: idx = vfat*128 + chan
         *  @param masks as calEnable but for channel masks
         *  @param trimARM as calEnable but for arming comparator trim value
         *  @param trimARMPol as calEnable but for arming comparator trim polarity
         *  @param trimZCC as calEnable but for zero crossing comparator trim value
         *  @param trimZCCPol as calEnable but for zero crossing comparator trim polarity
         */
        uint32_t setChannelRegistersVFAT3(uint32_t ohN, uint32_t vfatMask, uint32_t *calEnable, uint32_t *masks, uint32_t *trimARM, uint32_t *trimARMPol, uint32_t *trimZCC, uint32_t *trimZCCPol);

        /**
         *  @brief sets all vfat3 channel registers using a single channel register array
         *  @param ohN Optohybrid optical link number
         *  @param vfatMask Bitmask of chip positions determining which chips to use
         *  @param chanRegData array pointer for channel register data with 3072 entries, the (vfat,chan) pairing determines the array index via: idx = vfat*128 + chan
         */
        uint32_t setChannelRegistersVFAT3Simple(uint32_t ohN, uint32_t vfatMask, uint32_t *chanRegData);
      };
    }
  }
}

#endif // XHAL_CLIENT_RPCMAN_VFAT3_H
