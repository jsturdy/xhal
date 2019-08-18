BUILD_HOME   := $(shell dirname `cd ../; pwd`)
Project      := xhal
Package      := xhal
ShortPackage := xhal
LongPackage  := xhalarm
PackageName  := $(ShortPackage)
PackagePath  := $(shell pwd)
PackageDir   := pkg/$(ShortPackage)
Packager     := Mykhailo Dalchenko
Arch         := arm

ProjectPath:= $(BUILD_HOME)/$(Project)
ConfigDir  := $(ProjectPath)/config

include $(ConfigDir)/mfCommonDefs.mk
include $(ConfigDir)/mfZynq.mk
include $(ConfigDir)/mfRPMRules.mk

PackageSourceDir:=$(ProjectPath)/xhalcore/src/common
PackageIncludeDir:=$(ProjectPath)/xhalcore/include
PackageObjectDir:=$(PackagePath)/src/linux/$(Arch)

XHAL_VER_MAJOR:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[1];}' | awk '{split($$0,b,":"); print b[2];}')
XHAL_VER_MINOR:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[2];}' | awk '{split($$0,b,":"); print b[2];}')
XHAL_VER_PATCH:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[3];}' | awk '{split($$0,b,":"); print b[2];}')

ADDFLAGS=-g -std=gnu++14

IncludeDirs+=$(PackageIncludeDir)
INC=$(IncludeDirs:%=-I%)

Libraries+=-llog4cplus -lxerces-c -lstdc++

LDFLAGS+=-shared $(LibraryDirs)

SRCS_XHAL = $(shell echo $(PackageSourceDir)/utils/*.cpp)
SRCS_XHAL+= $(shell echo $(PackageSourceDir)/rpc/*.cpp)

AUTODEPS_XHAL = $(patsubst $(PackageSourceDir)/%.cpp,$(PackageObjectDir)/%.d,$(SRCS_XHAL))

OBJS_XHAL = $(patsubst %.d,%.o,$(AUTODEPS_XHAL))

XHAL_LIB = $(PackageLibraryDir)/libxhal.so

TargetLibraries:= xhal

# destination path macro we'll use below
df = $(PackageObjectDir)/$(*F)

## @xhalarm Compile all target libraries
build: $(TargetLibraries)

all: build

default: build

## @xhalarm Prepare the package for building the RPM
rpmprep: build
	@echo "Running preprpm target"
	$(MakeDir) $(PackageDir)/$(Package)
	@cp -rfp lib include src Makefile $(PackageDir)/$(Package)
	@cp -rfp $(ProjectPath)/config $(PackageDir)
	cd $(ProjectPath); cp -rfp xhalcore/{include,src} $(PackagePath)/$(PackageDir)
#	cd $(ProjectPath); cp -rfp --parents xhalcore/{include,src/common} $(PackagePath)/$(PackageDir)
	$(MakeDir) $(RPMBUILD_DIR)/SOURCES
	cd $(PackageDir)/..; \
	    tar cjf $(RPMBUILD_DIR)/SOURCES/$(Project)-$(LongPackage)-$(PACKAGE_FULL_VERSION)-$(PACKAGE_NOARCH_RELEASE).tbz2 $(PackageName);

## @xhalarm Compile the xhal library
xhal: $(XHAL_LIB)

doc:
	@echo "TO DO"

## adapted from http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
## Generic object creation rule, generate dependencies and use them later
$(PackageObjectDir)/%.o: $(PackageSourceDir)/%.cpp
	$(MakeDir) $(@D)
	$(CC) $(CFLAGS) $(ADDFLAGS) $(INC) -c -MT $@ -MMD -MP -MF $(@D)/$(*F).Td -o $@ $<
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
	$(CC) $(ADDFLAGS) $(LDFLAGS) -o $(@D)/$(LibraryFull) $^ $(Libraries)
	$(link-sonames)

## FIXME obsolete? only for xcompiled things?
# %.o: %.c
# 	$(CC) -std=gnu99 -c $(CFLAGS) -o $@ $<
# %.o: %.cc
# 	$(CXX) -std=c++0x -c $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJS_XHAL)
	$(RM) $(PackageLibraryDir)
	$(RM) $(PackageExecDir)
	$(RM) $(PackageDir)

cleandoc: 
	@echo "TO DO"

cleanall:
	$(RM) $(PackageObjectDir)
