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

## For now, default behaviour is no soname
UseSONAMEs=no

ProjectPath:=$(BUILD_HOME)/$(Project)

ConfigDir:=$(ProjectPath)/config

include $(ConfigDir)/mfCommonDefs.mk

ifeq ($(Arch),x86_64)
include $(ConfigDir)/mfPythonDefs.mk
CFLAGS=-Wall -pthread
ADDFLAGS=-fPIC -std=c++14 -std=gnu++14 -m64
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
+IncludeDirs+=/opt/rh/devtoolset-6/root/usr/include
IncludeDirs+= $(PackageIncludeDir)
INC=$(IncludeDirs:%=-I%)

Libraries+= -llog4cplus -lxerces-c -lstdc++
ifeq ($(Arch),x86_64)
Libraries+=-lwiscrpcsvc
LibraryDirs+=$(XDAQ_ROOT)/lib
LibraryDirs+=/opt/wiscrpcsvc/lib
else

endif

LibraryDirs+=$(PackageLibraryDir)

LDFLAGS+=$(LibraryDirs:%=-L%)

LIB=$(LibraryDirs)
LIB+=$(Libraries)

SRCS_XHAL     = $(wildcard $(PackageSourceDir)/common/utils/*.cpp)
AUTODEPS_XHAL = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_XHAL))
OBJS_XHAL     = $(patsubst %.d,%.o,$(AUTODEPS_XHAL))

ifeq ($(Arch),x86_64)
SRCS_XHAL   += $(wildcard $(PackageSourceDir)/common/*.cpp)
SRCS_RPC_MAN = $(wildcard $(PackageSourceDir)/common/rpc_manager/*.cpp)

AUTODEPS_XHAL   += $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_XHAL))
AUTODEPS_RPC_MAN = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_RPC_MAN))

OBJS_XHAL    += $(patsubst %.d,%.o,$(AUTODEPS_XHAL))
OBJS_RPC_MAN = $(patsubst %.d,%.o,$(AUTODEPS_RPC_MAN))

RPCMAN_LIB   = $(PackageLibraryDir)/librpcman.so
endif

XHAL_LIB = $(PackageLibraryDir)/libxhal.so

ifeq ($(Arch),x86_64)
TargetLibraries:=xhal rpc
else
TargetLibraries:=xhal
endif

## Override the RPM_DIR variable because we're a special case
RPM_DIR:=$(ProjectPath)/$(PackageName)/$(LongPackage)/rpm
include $(ConfigDir)/mfRPMRules.mk

$(PackageSpecFile): $(ProjectPath)/$(PackageName)/spec.template

# destination path macro we'll use below
df = $(PackageObjectDir)/$(*F)

.PHONY: clean xhalcore rpc prerpm

## @xhal Compile all target libraries
build: $(TargetLibraries)

all:

default: $(TARGETS)

## @xhal Prepare the package for building the RPM
rpmprep: build doc

# Define as dependency everything that should cause a rebuild
TarballDependencies = $(XHAL_LIB) Makefile xhal.mk spec.template $(PackageIncludeDir)/packageinfo.h
ifeq ($(Arch),x86_64)
TarballDependencies+=$(RPCMAN_LIB)
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
xhal: $(XHAL_LIB)

## @xhal Compile the xhal-client library
rpc: $(RPCMAN_LIB)

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
	$(CXX) $(ADDFLAGS) $(LDFLAGS) $(SOFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

$(RPCMAN_LIB): $(OBJS_RPC_MAN)
	$(MakeDir) -p $(@D)
	$(CXX) $(ADDFLAGS) $(LDFLAGS) $(SOFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

ifeq ($(Arch),x86_64)
else
PETA_PATH?=/opt/gem-peta-stage
TARGET_BOARD?=ctp7
.PHONY: crosslibinstall crosslibuninstall

install: crosslibinstall

## @xhal install libraries for cross-compilation
crosslibinstall:
	echo "Installing cross-compiler libs"
	if [ -d $(PackageLibraryDir) ]; then \
	   cd $(PackageLibraryDir); \
	   find . -type f -exec sh -ec 'install -D -m 755 $$0 $(INSTALL_PREFIX)$(PETA_PATH)/$(TARGET_BOARD)/$(INSTALL_PATH)/lib/$$0' {} \; ; \
	   find . -type l -exec sh -ec 'if [ -n "$${0}" ]; then ln -sf $$(basename $$(readlink $$0)) $(INSTALL_PREFIX)$(PETA_PATH)/$(TARGET_BOARD)/$(INSTALL_PATH)/lib/$${0##./}; fi' {} \; ; \
	fi

uninstall: crosslibuninstall

## @xhal uninstall libraries for cross-compilation
crosslibuninstall:
	$(RM) $(INSTALL_PREFIX)$(PETA_PATH)/$(TARGET_BOARD)/$(INSTALL_PATH)
endif

clean:
	$(RM) $(OBJS_XHAL) $(OBJS_RPC_MAN)
	$(RM) $(PackageLibraryDir)
	$(RM) $(PackageExecDir)
	$(RM) $(PackagePath)/$(PackageDir)

cleanall:
	$(RM) $(PackageObjectDir)
	$(RM) $(PackagePath)

# default:
# 	@echo "Running default target"
# 	$(MakeDir) $(PackageDir)

# _rpmprep: preprpm
# 	@echo "Running _rpmprep target"
# preprpm: default
# 	@echo "Running preprpm target"
# 	@cp -rf lib $(PackageDir)

# build: xhalcore rpc

# _all:${XHALCORE_LIB} ${RPCMAN_LIB} 

# rpc:${RPCMAN_LIB}

# xhalcore:${XHALCORE_LIB}

# $(XHALCORE_LIB): $(OBJS_UTILS) $(OBJS_XHAL)
# 	@mkdir -p $(PackageLibraryDir)/
# 	$(CC) $(CCFLAGS) $(ADDFLAGS) ${LDFLAGS} $(INC) $(LIB) -o $@ $^

# $(OBJS_UTILS):$(SRCS_UTILS)
# 	    $(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) $(LIB) -c -o $@ $<

# $(OBJS_XHAL):$(SRCS_XHAL)
# 	    $(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) $(LIB) -c -o $@ $<

# $(RPCMAN_LIB): $(OBJS_RPC_MAN)
# 	$(CC) $(CCFLAGS) $(ADDFLAGS) ${LDFLAGS} $(INC) $(LIB) -o $@ $^

# $(OBJS_RPC_MAN):$(SRCS_RPC_MAN)
# 	$(CC) $(CCFLAGS) $(ADDFLAGS) $(INC) $(LIB) -c $(@:%.o=%.cc) -o $@ 

# clean:
# 	-${RM} ${XHALCORE_LIB} ${OBJS_UTILS} ${OBJS_XHAL} ${RPCMAN_LIB} ${OBJS_RPC_MAN}
# 	-rm -rf $(PackageDir)

# cleandoc: 
# 	@echo "TO DO"
