#ifndef SCA_H
#define SCA_H

#include "xhal/rpc/utils.h"

/*!
*  \fn uint32_t readSCAADCSensor()
*  \brief Read individual SCA ADC sensor
*  \param[in] const uint32_t ohMask : This specifies which OH's to read from
*  \param[in] const uint32_t ch : Name of SCA ADC sensor to read
*
*  \param[out] uint32_t* result : ADC data is returned as an array of 32-bit words formatted as:
*                                 bit [27]: data present
*                                 bits [26:24]: link ID
*                                 bits [23:21]: constant 0's
*                                 bits [20:16]: ADC channel ID
*                                 bits [15:12]: constant 0's
*                                 bits [11:0]: ADC data

*  \return Error code (0 if AOK)
*/
DLLEXPORT uint32_t readSCAADCSensor(const uint32_t ohMask, const uint32_t ch, uint32_t* result);
/*!
 *  \fn void readSCAADCTemperatureSensors(const RPCMsg *request, RPCMsg *response)
 *  \brief Read all SCA ADC temperature sensors. They are 0x00, 0x04, 0x07, and 0x08.
 *  \param[in] const uint32_t ohMask : This specifies which OH's to read from
 *
 *  FIXME provide better description of the return array
 *  \param[out] uint32_t* result : ADC data is returned as an array of 32-bit words formatted as:
 *                    bit [27]: data present
 *                    bits [26:24]: link ID
 *                    bits [23:21]: constant 0's
 *                    bits [20:16]: ADC channel ID
 *                    bits [15:12]: constant 0's
 *                    bits [11:0]: ADC data
 *
 *  \return Error code (0 if AOK)
 */
DLLEXPORT uint32_t readSCAADCTemperatureSensors(const uint32_t ohMask, uint32_t * result);
/*!
 *  \fn void readSCAADCVoltageSensors(const RPCMsg *request, RPCMsg *response)
 *  \brief Read all SCA ADC voltages sensors. They are 1B, 1E, 11, 0E, 18 and 0F. 
 *  \param[in] const uint32_t ohMask : This specifies which OH's to read from
 *
 *  FIXME provide better description of the return array
 *  \param[out] uint32_t* result : ADC data is returned as an array of 32-bit words formatted as:
 *                    bit [27]: data present
 *                    bits [26:24]: link ID
 *                    bits [23:21]: constant 0's
 *                    bits [20:16]: ADC channel ID
 *                    bits [15:12]: constant 0's
 *                    bits [11:0]: ADC data
 *
 *  \return Error code (0 if AOK)
 */
DLLEXPORT uint32_t readSCAADCVoltageSensors(const uint32_t ohmask, uint32_t * result);
/*!
 *  \fn void readSCAADCSignalStrengthSensors(const RPCMsg *request, RPCMsg *response)
 *  \brief Read the SCA ADC signal strength sensors. They are 15, 13 and 12. 
 *  \param[in] const uint32_t ohMask : This specifies which OH's to read from
 *
 *  \param[out] uint32_t* result : ADC data is returned as an array of 32-bit words formatted as:
 *                    bit [27]: data present
 *                    bits [26:24]: link ID
 *                    bits [23:21]: constant 0's
 *                    bits [20:16]: ADC channel ID
 *                    bits [15:12]: constant 0's
 *                    bits [11:0]: ADC data
 *
 *  \return Error code (0 if AOK)
 */
DLLEXPORT uint32_t readSCAADCSignalStrengthSensors(const uint32_t ohmask, uint32_t * result);
/*!
 *  \fn void readAllSCAADCSensors(const RPCMsg *request, RPCMsg *response)
 *  \brief Read all connected SCA ADC sensors. 
 *  \param[in] const uint32_t ohMask : This specifies which OH's to read from
 *
 *  \param[out] uint32_t* result : ADC data is returned as an array of 32-bit words formatted as:
 *                    bit [27]: data present
 *                    bits [26:24]: link ID
 *                    bits [23:21]: constant 0's
 *                    bits [20:16]: ADC channel ID
 *                    bits [15:12]: constant 0's
 *                    bits [11:0]: ADC data
 *
 *  \return Error code (0 if AOK)
 */
DLLEXPORT uint32_t readAllSCAADCSensors(const uint32_t ohmask, uint32_t * result);

#endif
