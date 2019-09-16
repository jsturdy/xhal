BUILD_HOME   := $(shell dirname `cd ../; pwd`)
Project      := xhal
Package      := xhal
ShortPackage := xhal
LongPackage  := $(TargetArch)
PackageName  := $(ShortPackage)
PackagePath  := $(TargetArch)
PackageDir   := pkg/$(ShortPackage)
Packager     := Mykhailo Dalchenko
Arch         := $(TargetArch)

ProjectPath:=$(BUILD_HOME)/$(Project)

ConfigDir:=$(ProjectPath)/config

include $(ConfigDir)/mfCommonDefs.mk

ifeq ($(Arch),x86_64)
include $(ConfigDir)/mfPythonDefs.mk
CFLAGS=-Wall -pthread
ADDFLAGS=-fPIC -std=c++11 -std=gnu++11 -m64
else
include $(ConfigDir)/mfZynq.mk
ADDFLAGS=-std=gnu++14
endif

ADDFLAGS+=$(OPTFLAGS)

PackageSourceDir:=src
PackageIncludeDir:=include
PackageObjectDir:=$(PackagePath)/src/linux/$(Arch)
PackageLibraryDir:=$(PackagePath)/lib
PackageExecDir:=$(PackagePath)/bin
PackageDocsDir:=$(PackagePath)/doc/_build/html

XHAL_VER_MAJOR:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[1];}' | awk '{split($$0,b,":"); print b[2];}')
XHAL_VER_MINOR:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[2];}' | awk '{split($$0,b,":"); print b[2];}')
XHAL_VER_PATCH:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[3];}' | awk '{split($$0,b,":"); print b[2];}')

IncludeDirs+= $(XDAQ_ROOT)/include
IncludeDirs+= $(PackageIncludeDir)
IncludeDirs+= $(PackageIncludeDir)/xhal/extern
# IncludeDirs+= $(PackageIncludeDir)/server
# IncludeDirs+= $(PackageIncludeDir)/client
INC=$(IncludeDirs:%=-I%)

ifeq ($(Arch),x86_64)
Libraries+=-llog4cplus -lxerces-c -lwiscrpcsvc -lstdc++
LibraryDirs+=-L$(XDAQ_ROOT)/lib
LibraryDirs+=-L/opt/wiscrpcsvc/lib
else
Libraries+=-llog4cplus -lxerces-c -lstdc++
endif

LibraryDirs+=-L$(PackageLibraryDir)

LDFLAGS+= -shared $(LibraryDirs)

SRCS_XHAL   = $(wildcard $(PackageSourceDir)/common/utils/*.cpp)
SRCS_XHAL  += $(wildcard $(PackageSourceDir)/common/rpc/*.cpp)
SRCS_CLIENT = $(wildcard $(PackageSourceDir)/client/*.cpp)
SRCS_SERVER = $(wildcard $(PackageSourceDir)/server/*.cpp)
SRCS_XHALPY = $(wildcard $(PackageSourceDir)/client/python_wrappers/*.cpp)
SRCS_RPCMAN = $(wildcard $(PackageSourceDir)/client/rpc_manager/*.cpp)
# SRCS_EXES     = $(wildcard $(PackageSourceDir)/*.cxx)
# SRCS_TEST_EXES= $(wildcard $(PackageTestSourceDir)/*.cxx)

AUTODEPS_XHAL   = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_XHAL))
AUTODEPS_CLIENT = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_CLIENT))
AUTODEPS_SERVER = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_SERVER))
AUTODEPS_XHALPY = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_XHALPY))
AUTODEPS_RPCMAN = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_RPCMAN))

OBJS_XHAL   = $(patsubst %.d,%.o,$(AUTODEPS_XHAL))
OBJS_CLIENT = $(patsubst %.d,%.o,$(AUTODEPS_CLIENT))
OBJS_SERVER = $(patsubst %.d,%.o,$(AUTODEPS_SERVER))
OBJS_XHALPY = $(patsubst %.d,%.o,$(AUTODEPS_XHALPY))
OBJS_RPCMAN = $(patsubst %.d,%.o,$(AUTODEPS_RPCMAN))

XHAL_LIB   = $(PackageLibraryDir)/libxhal-base.so
CLIENT_LIB = $(PackageLibraryDir)/libxhal-client.so
SERVER_LIB = $(PackageLibraryDir)/libxhal-server.so
XHALPY_LIB = $(PackageLibraryDir)/xhalpy.so
RPCMAN_LIB = $(PackageLibraryDir)/libxhal-rpcman.so

ifeq ($(Arch),x86_64)
TargetLibraries:= xhal-base xhal-server xhal-client xhalpy xhal-rpcman
else
TargetLibraries:= xhal-base xhal-server
endif

## Update spec file for subpackage specific requirements
ifndef BASE_REQUIRED_PACKAGE_LIST
BASE_REQUIRED_PACKAGE_LIST=$(shell awk 'BEGIN{IGNORECASE=1} /define XHAL_BASE_REQUIRED_PACKAGE_LIST/ {print $$3;}' $(PackageIncludeDir)/packageinfo.h)
endif

ifndef CLIENT_REQUIRED_PACKAGE_LIST
CLIENT_REQUIRED_PACKAGE_LIST=$(shell awk 'BEGIN{IGNORECASE=1} /define XHAL_CLIENT_REQUIRED_PACKAGE_LIST/ {print $$3;}' $(PackageIncludeDir)/packageinfo.h)
endif

ifndef SERVER_REQUIRED_PACKAGE_LIST
SERVER_REQUIRED_PACKAGE_LIST=$(shell awk 'BEGIN{IGNORECASE=1} /define XHAL_SERVER_REQUIRED_PACKAGE_LIST/ {print $$3;}' $(PackageIncludeDir)/packageinfo.h)
endif

ifndef BASE_BUILD_REQUIRED_PACKAGE_LIST
BASE_BUILD_REQUIRED_PACKAGE_LIST=$(shell awk 'BEGIN{IGNORECASE=1} /define XHAL_BASE_BUILD_REQUIRED_PACKAGE_LIST/ {print $$3;}' $(PackageIncludeDir)/packageinfo.h)
endif

ifndef CLIENT_BUILD_REQUIRED_PACKAGE_LIST
CLIENT_BUILD_REQUIRED_PACKAGE_LIST=$(shell awk 'BEGIN{IGNORECASE=1} /define XHAL_CLIENT_BUILD_REQUIRED_PACKAGE_LIST/ {print $$3;}' $(PackageIncludeDir)/packageinfo.h)
endif

ifndef SERVER_BUILD_REQUIRED_PACKAGE_LIST
SERVER_BUILD_REQUIRED_PACKAGE_LIST=$(shell awk 'BEGIN{IGNORECASE=1} /define XHAL_SERVER_BUILD_REQUIRED_PACKAGE_LIST/ {print $$3;}' $(PackageIncludeDir)/packageinfo.h)
endif

## Override the RPM_DIR variable because we're a special case
RPM_DIR:=$(ProjectPath)/$(PackageName)/$(LongPackage)/rpm
include $(ConfigDir)/mfRPMRules.mk

$(PackageSpecFile): $(ProjectPath)/$(PackageName)/spec.template

specificspecupdate: $(PackageSpecFile)
	echo Running specific spec update
	sed -i 's#__base_requires_list__#$(BASE_REQUIRED_PACKAGE_LIST)#'                 $<
	sed -i 's#__base_build_requires_list__#$(BASE_BUILD_REQUIRED_PACKAGE_LIST)#'     $<
	sed -i 's#__client_requires_list__#$(CLIENT_REQUIRED_PACKAGE_LIST)#'             $<
	sed -i 's#__client_build_requires_list__#$(CLIENT_BUILD_REQUIRED_PACKAGE_LIST)#' $<
	sed -i 's#__server_requires_list__#$(SERVER_REQUIRED_PACKAGE_LIST)#'             $<
	sed -i 's#__server_build_requires_list__#$(SERVER_BUILD_REQUIRED_PACKAGE_LIST)#' $<

# destination path macro we'll use below
df = $(PackageObjectDir)/$(*F)

.PHONY: xhal-base xhal-client xhal-server xhal-rpcman

## @xhal Compile all target libraries
build: $(TargetLibraries)

all:

default: $(TARGETS)

## @xhal Prepare the package for building the RPM
rpmprep: build doc

# Define as dependency everything that should cause a rebuild
TarballDependencies = $(XHAL_LIB) $(SERVER_LIB) Makefile xhal.mk spec.template
ifeq ($(Arch),x86_64)
TarballDependencies+= $(CLIENT_LIB) $(XHALPY_LIB) $(RPCMAN_LIB)
else
endif

## this needs to reproduce the compiled tree because... wrong headed
## either do the make in the spec file, or don't make up your mind!
$(PackageSourceTarball): $(TarballDependencies)
	$(MakeDir) $(PackagePath)/$(PackageDir)
ifeq ($(Arch),x86_64)
	echo nothing to do
else
	$(MakeDir) $(PackagePath)/$(PackageDir)/gem-peta-stage/ctp7/$(INSTALL_PATH)/lib
	@cp -rfp $(PackageLibraryDir)/* $(PackagePath)/$(PackageDir)/gem-peta-stage/ctp7/$(INSTALL_PATH)/lib
endif
	$(MakeDir) $(RPM_DIR)
	@cp -rfp spec.template $(PackagePath)
	$(MakeDir) $(PackagePath)/$(PackageDir)/$(PackageName)/$(LongPackage)
	@cp -rfp --parents $(PackageObjectDir) $(PackagePath)/$(PackageDir)/$(PackageName)
	@cp -rfp --parents $(PackageLibraryDir) $(PackagePath)/$(PackageDir)/$(PackageName)
	-cp -rfp --parents $(PackageExecDir) $(PackagePath)/$(PackageDir)/$(PackageName)
	@cp -rfp $(PackageSourceDir) $(PackagePath)/$(PackageDir)/$(PackageName)
	@cp -rfp $(PackageIncludeDir) $(PackagePath)/$(PackageDir)/$(PackageName)
	@cp -rfp xhal.mk $(PackagePath)/$(PackageDir)/$(PackageName)/Makefile
	@cp -rfp $(ProjectPath)/config $(PackagePath)/$(PackageDir)
#	cd $(ProjectPath); cp -rfp --parents xhal/Makefile $(PackagePath)/$(PackageDir)
#	cd $(ProjectPath); cp -rfp --parents xhal/{include,src} $(PackagePath)/$(PackageDir)
	cd $(PackagePath)/$(PackageDir)/..; \
	    tar cjf $(PackageSourceTarball) . ;
#	$(RM) $(PackagePath)/$(PackageDir)

## @xhal Compile the xhal library
xhal-base: $(XHAL_LIB)

## @xhal Compile the xhal-client library
xhal-client: $(CLIENT_LIB)

## @xhal Compile the xhal-server library
xhal-server: $(SERVER_LIB)

## @xhal Compile the xhal RPC manager library
xhal-rpcman: xhal-base $(RPCMAN_LIB)

## @xhal Compile the xhal python bindings
xhalpy: xhal-base xhal-rpcman $(XHALPY_LIB)

## adapted from http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
## Generic object creation rule, generate dependencies and use them later
$(PackageObjectDir)/%.o: $(PackageSourceDir)/%.cpp Makefile
	$(MakeDir) $(@D)
	$(CXX) $(CFLAGS) $(ADDFLAGS) $(INC) -c -MT $@ -MMD -MP -MF $(@D)/$(*F).Td -o $@ $<
	mv $(@D)/$(*F).Td $(@D)/$(*F).d
	touch $@

## dummy rule for dependencies
$(PackageObjectDir)/%.d:

## mark dependencies and objects as not auto-removed
.PRECIOUS: $(PackageObjectDir)/%.d
.PRECIOUS: $(PackageObjectDir)/%.o

## Force rule for all target library names
$(TargetLibraries):

$(XHAL_LIB): $(OBJS_XHAL)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

$(CLIENT_LIB): $(OBJS_CLIENT)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

$(SERVER_LIB): $(OBJS_SERVER)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

$(RPCMAN_LIB): $(OBJS_RPCMAN)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

$(XHALPY_LIB): $(OBJS_XHALPY) $(XHAL_LIB) $(RPCMAN_LIB)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) -L$(PYTHON_LIB_PREFIX) -o $(@D)/$(LibraryFull) $^ $(Libraries) -lboost_python -l$(PYTHON_LIB) -lxhal-base -lxhal-rpcman
	$(link-sonames)

clean:
	$(RM) $(OBJS_XHAL) $(OBJS_CLIENT) $(OBJS_SERVER) $(OBJS_RPCMAN) $(OBJS_XHALPY)
	$(RM) $(PackageLibraryDir)
	$(RM) $(PackageExecDir)
	$(RM) $(PackagePath)/$(PackageDir)

cleanall:
	$(RM) $(PackageObjectDir)
	$(RM) $(PackagePath)
