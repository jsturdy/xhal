BUILD_HOME   := $(shell dirname `cd ../; pwd`)
Project      := xhal
Package      := xhal
ShortPackage := xhal
LongPackage  := xhalcore
PackageName  := $(ShortPackage)
PackagePath  := $(shell pwd)
PackageDir   := pkg/$(ShortPackage)
Packager     := Mykhailo Dalchenko
Arch         := x86_64

ProjectPath:= $(BUILD_HOME)/$(Project)
ConfigDir  := $(ProjectPath)/config

include $(ConfigDir)/mfCommonDefs.mk
include $(ConfigDir)/mfPythonDefs.mk
include $(ConfigDir)/mfRPMRules.mk

PackageSourceDir:=$(PackagePath)/src/common
PackageObjectDir:=$(PackagePath)/src/linux/$(Arch)

XHAL_VER_MAJOR:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[1];}' | awk '{split($$0,b,":"); print b[2];}')
XHAL_VER_MINOR:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[2];}' | awk '{split($$0,b,":"); print b[2];}')
XHAL_VER_PATCH:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[3];}' | awk '{split($$0,b,":"); print b[2];}')

CCFLAGS=-fno-inline -Wall -pthread
ADDFLAGS=-g -fPIC -std=c++11 -m64

IncludeDirs+= $(XDAQ_ROOT)/include
IncludeDirs+= $(PackageIncludeDir)
INC=$(IncludeDirs:%=-I%)

Libraries+=-llog4cplus -lxerces-c -lwiscrpcsvc -lstdc++

LibraryDirs+=-L$(XDAQ_ROOT)/lib
LibraryDirs+=-L$(PackageLibraryDir)
LibraryDirs+=-L/opt/wiscrpcsvc/lib

LDFLAGS+= -shared $(LibraryDirs)

SRCS_XHAL   = $(wildcard $(PackageSourceDir)/*.cpp)
SRCS_UTILS  = $(wildcard $(PackageSourceDir)/utils/*.cpp)
SRCS_UTILS += $(wildcard $(PackageSourceDir)/rpc/*.cpp)
SRCS_XHALPY = $(wildcard $(PackageSourceDir)/python_wrappers/*.cpp)
SRCS_RPCMAN = $(wildcard $(PackageSourceDir)/rpc_manager/*.cpp)
# SRCS_EXES     = $(wildcard $(PackageSourceDir)/*.cxx)
# SRCS_TEST_EXES= $(wildcard $(PackageTestSourceDir)/*.cxx)

AUTODEPS_XHAL   = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_XHAL))
AUTODEPS_UTILS  = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_UTILS))
AUTODEPS_XHALPY = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_XHALPY))
AUTODEPS_RPCMAN = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_RPCMAN))

OBJS_XHAL   = $(patsubst %.d,%.o,$(AUTODEPS_XHAL))
OBJS_UTILS  = $(patsubst %.d,%.o,$(AUTODEPS_UTILS))
OBJS_XHALPY = $(patsubst %.d,%.o,$(AUTODEPS_XHALPY))
OBJS_RPCMAN = $(patsubst %.d,%.o,$(AUTODEPS_RPCMAN))

XHAL_LIB   = $(PackageLibraryDir)/libxhal.so
XHALPY_LIB = $(PackageLibraryDir)/xhalpy.so
RPCMAN_LIB = $(PackageLibraryDir)/librpcman.so

TargetLibraries:= xhal xhalpy rpcman

# destination path macro we'll use below
df = $(PackageObjectDir)/$(*F)

.PHONY: xhal rpc

## @xhalcore Compile all target libraries
build: $(TargetLibraries)

all: build

default: build
	@echo "Running default target"
	$(MakeDir) $(PackageDir)

## @xhalcore Prepare the package for building the RPM
rpmprep: build doc
	@echo "Running preprpm target"
	$(MakeDir) lib/arm
	$(MakeDir) $(PackageDir)/$(Package)
	@cp -rfp $(ProjectPath)/xhalarm/lib/* lib/arm
	@cp -rfp lib include Makefile $(PackageDir)/$(Package)
	@cp -rfp src $(PackageDir)/$(Package)
#	@cp -rfp --parents src/common $(PackageDir)/$(Package)
	@cp -rfp $(ProjectPath)/config $(PackageDir)
	$(MakeDir) $(RPMBUILD_DIR)/SOURCES
	cd $(PackageDir)/..; \
	    tar cjf $(RPMBUILD_DIR)/SOURCES/$(Project)-$(LongPackage)-$(PACKAGE_FULL_VERSION)-$(PACKAGE_NOARCH_RELEASE).tbz2 $(PackageName);
	$(RM) $(PackageDir)/$(Package)

## @xhalcore Compile the xhal RPC manager library
rpc: xhal $(RPCMAN_LIB)

## @xhalcore Compile the xhal library
xhal: $(XHAL_LIB)

## @xhalcore Compile the xhal python bindings
xhalpy: xhal rpc $(XHALPY_LIB)

doc:
	@echo "TO DO"

## adapted from http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
## Generic object creation rule, generate dependencies and use them later
$(PackageObjectDir)/%.o: $(PackageSourceDir)/%.cpp
	$(MakeDir) $(@D)
	$(CXX) $(CCFLAGS) $(ADDFLAGS) $(INC) -c -MT $@ -MMD -MP -MF $(@D)/$(*F).Td -o $@ $<
	mv $(@D)/$(*F).Td $(@D)/$(*F).d
	touch $@

## dummy rule for dependencies
$(PackageObjectDir)/%.d:

## mark dependencies and objects as not auto-removed
.PRECIOUS: $(PackageObjectDir)/%.d
.PRECIOUS: $(PackageObjectDir)/%.o

## Force rule for all target library names
$(TargetLibraries):

$(XHAL_LIB): $(OBJS_XHAL) $(OBJS_UTILS)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

$(XHALPY_LIB): $(OBJS_XHALPY)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) -L$(PYTHON_LIB_PREFIX) -o $(@D)/$(LibraryFull) $^ $(Libraries) -lboost_python -l$(PYTHON_LIB) -lxhal -lrpcman
	$(link-sonames)

$(RPCMAN_LIB): $(OBJS_RPCMAN)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

clean:
	$(RM) $(OBJS_UTILS) $(OBJS_XHAL) $(OBJS_RPCMAN) $(OBJS_XHALPY)
	$(RM) $(PackageLibraryDir)
	$(RM) $(PackageExecDir)
	$(RM) $(PackageDir)

cleandoc: 
	@echo "TO DO"

cleanall:
	$(RM) $(PackageObjectDir)
