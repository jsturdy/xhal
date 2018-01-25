CC=g++
CCFLAGS=-O0 -g3 -fno-inline -Wall -fPIC -pthread -m64
ADDFLAGS=-std=gnu++14

IncludeDirs = /opt/xdaq/include
IncludeDirs += /opt/rh/devtoolset-6/root/usr/include
IncludeDirs += ${XHAL_ROOT}/include
INC=$(IncludeDirs:%=-I%)

Libraries+= -llog4cplus -lxerces-c 
LibraryDirs+=-L/opt/xdaq/lib
LibraryDirs+=-L${XHAL_ROOT}/lib/x86_64
LIB=$(LibraryDirs)
LIB+= $(Libraries)

LDFLAGS= -shared
SRCS = $(shell echo src/common/utils/*.cpp)
SRCS_XHAL = $(shell echo src/common/*.cpp)
OBJS = $(SRCS:.cpp=.o)
OBJS_XHAL = $(SRCS_XHAL:.cpp=.o)

SRCS_TEST = test/main_test.cpp
OBJS_TEST = $(SRCS_TEST:.cpp=.o)

SRCS_RPC_SA = $(shell echo src/common/rpc_standalone_client/*.cpp)
OBJS_RPC_SA = $(SRCS_RPC_SA:.cpp=.o)

SRCS_RPC_MAN = $(shell echo src/common/rpc_manager/*.cc)
OBJS_RPC_MAN = $(SRCS_RPC_MAN:.cc=.o)


TARGET_LIB=${XHAL_ROOT}/lib/x86_64/libxhal.so
TARGET_TEST=${XHAL_ROOT}/test/bin/test
RPC_SA_LIB=${XHAL_ROOT}/lib/x86_64/librwreg.so
RPC_MAN_LIB=${XHAL_ROOT}/lib/x86_64/librpcman.so

all:${TARGET_LIB} ${RPC_SA_LIB}

rpc:${RPC_MAN_LIB}

test:${TARGET_TEST}

utils:${TARGET_LIB}

$(TARGET_LIB): $(OBJS) $(OBJS_XHAL)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) ${LDFLAGS} $(INC) $(LIB) -lwiscrpcsvc -o $@ $^

#$(SRCS:.cpp=.d):%.d:%.cpp
$(OBJS):$(SRCS)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) $(LIB) -c -o $@ $<

$(OBJS_XHAL):$(SRCS_XHAL)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) $(LIB) -c -o $@ $<

test:${TARGET_TEST}
$(TARGET_TEST): $(OBJS_TEST)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) $(LIB) -lxhal -lwiscrpcsvc -o $@ $^

$(OBJS_TEST):$(SRCS_TEST)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) $(LIB) -lxhal -c -o $@ $<

rpc_standalone:${RPC_SA_LIB}
$(RPC_SA_LIB): $(OBJS_RPC_SA)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) ${LDFLAGS} $(INC) -L${XHAL_ROOT}/lib/x86_64/ -lwiscrpcsvc -o $@ $^

$(OBJS_RPC_SA):$(SRCS_RPC_SA)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) -L${XHAL_ROOT}/lib/x86_64 -lwiscrpcsvc -c -o $@ $<

$(RPC_MAN_LIB): $(OBJS_RPC_MAN)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) ${LDFLAGS} $(INC) -L${XHAL_ROOT}/lib/x86_64/ -lwiscrpcsvc -o $@ $^

$(OBJS_RPC_MAN):$(SRCS_RPC_MAN)
	    $(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) -L${XHAL_ROOT}/lib/x86_64 -lwiscrpcsvc -c $(@:%.o=%.cc) -o $@ 


.PHONY: clean
clean:
	    -${RM} ${TARGET_LIB} ${OBJS} ${OBJS_XHAL} ${TARGET_TEST} ${OBJS_TEST} ${RPC_SA_LIB} ${OBJS_RPC_SA} ${RPC_MAN_LIB} ${OBJS_RPC_MAN}



