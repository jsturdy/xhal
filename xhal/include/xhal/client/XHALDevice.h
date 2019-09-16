/**
 * @file XHALDevice.h
 * Hardware device for XHAL
 *
 * @author Mykhailo Dalchenko
 * @version 1.0
 */

#ifndef XHAL_CLIENT_XHALDEVICE_H
#define XHAL_CLIENT_XHALDEVICE_H

#include "xhal/common/utils/XHALXMLParser.h"
#include "xhal/client/XHALInterface.h"

#include <string>

namespace xhal {
  namespace client {
    /**
     * @class XHALDevice
     * @brief provide interface to call remote procedures at Zynq CPU and basic FW registers manipulation
     */
    class XHALDevice : public XHALInterface
    {
      public:
        /**
         * @brief Default constructor
         *
         * Parses XML file
         * @param board_domain_name domain name of CTP7
         * @param address_table_filename XML address table file name
         */
        XHALDevice(const std::string& board_domain_name, const std::string& address_table_filename);

        virtual ~XHALDevice() {}

        /**
         * @brief Reconnect to RPC service and reload required modules
         */
        virtual void reconnect();

        /**
         * @brief read FW register by its name
         * applies reading mask if any
         */
        uint32_t readReg(std::string regName);
        /**
         * @brief read FW register by its address
         * reg mask is ignored!!
         */
        uint32_t readReg(uint32_t address);
        /**
         * @brief write FW register by its name
         * applies read/write mask if any
         */
        void writeReg(std::string regName, uint32_t value);
        //void writeReg(uint32_t address, uint32_t value);

        /**
         * @brief Read list of FW registers by their addresses
         * FIXME reg mask is ignored??
         *
         * @param addresses An array of register addresses
         * @param result An array of register values
         * @param size Size of the above arrays
         */
        uint32_t getList(uint32_t* addresses, uint32_t* result, ssize_t size);

        /**
         * @brief Read block of 32-bit registers starting from a given address
         *
         * @param address Starting address
         * @param result An array of register values
         * @param size Size of the above array
         */
        uint32_t getBlock(uint32_t address, uint32_t* result, ssize_t size);

      private:
        std::string m_address_table_filename;
        xhal::common::utils::XHALXMLParser * m_parser;
        xhal::common::utils::Node m_node;
    };
  }
}

#endif  // XHAL_CLIENT_XHALDEVICE_H
