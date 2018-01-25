#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "xhal/rpc/wiscrpcsvc.h"

#define DLLEXPORT extern "C"

#define STANDARD_CATCH \
	catch (wisc::RPCSvc::NotConnectedException &e) { \
		printf("Caught NotConnectedException: %s\n", e.message.c_str()); \
		return 1; \
	} \
	catch (wisc::RPCSvc::RPCErrorException &e) { \
		printf("Caught RPCErrorException: %s\n", e.message.c_str()); \
		return 1; \
	} \
	catch (wisc::RPCSvc::RPCException &e) { \
		printf("Caught exception: %s\n", e.message.c_str()); \
		return 1; \
	} \
	catch (wisc::RPCMsg::BadKeyException &e) { \
		printf("Caught exception: %s\n", e.key.c_str()); \
		return 0xdeaddead; \
	} 

#define ASSERT(x) do { \
    if (!(x)) { \
    	printf("Assertion Failed on line %u: %s\n", __LINE__, #x); \
    	return 1; \
    } \
} while (0)

static wisc::RPCSvc rpc;
static wisc::RPCMsg req, rsp;

wisc::RPCSvc* getRPCptr();
DLLEXPORT uint32_t init(char * hostname);
DLLEXPORT uint32_t getReg(uint32_t address);
DLLEXPORT uint32_t putReg(uint32_t address, uint32_t value);
DLLEXPORT uint32_t getList(uint32_t* addresses, uint32_t* result, ssize_t size);
DLLEXPORT uint32_t getBlock(uint32_t address, uint32_t* result, ssize_t size);
DLLEXPORT uint32_t update_atdb(char * xmlfilename);
DLLEXPORT uint32_t getRegInfoDB(char * regName);

#endif
