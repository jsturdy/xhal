#
# Makefile for reg_interface package
#

BUILD_HOME := $(shell dirname `cd ../../; pwd`)

Project      := xhal
ShortProject := xhal
Namespace    := xhal
Package      := reg_interface_gem
ShortPackage := reg_interface_gem
LongPackage  := reg_interface_gem
PackageName  := $(ShortPackage)
PackagePath  := $(shell pwd)
PackageDir   := pkg/$(Namespace)/$(ShortPackage)
ScriptDir    := pkg/$(Namespace)/scripts

ProjectBase = $(BUILD_HOME)/$(Project)
ConfigDir   = $(ProjectBase)/config

include $(ConfigDir)/mfCommonDefs.mk
include $(ConfigDir)/mfPythonDefs.mk

# Explicitly define the modules that are being exported (for PEP420 compliance)
PythonModules = ["$(Namespace).$(ShortPackage)"]
$(info PythonModules=${PythonModules})

REG_INTERFACE_GEM_VER_MAJOR:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[1];}' | awk '{split($$0,b,":"); print b[2];}')
REG_INTERFACE_GEM_VER_MINOR:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[2];}' | awk '{split($$0,b,":"); print b[2];}')
REG_INTERFACE_GEM_VER_PATCH:=$(shell $(ConfigDir)/tag2rel.sh | awk '{split($$0,a," "); print a[3];}' | awk '{split($$0,b,":"); print b[2];}')

include $(ConfigDir)/mfPythonRPM.mk
include $(ConfigDir)/mfSphinx.mk

PythonSources=$(wildcard core/*.py)
PythonSources+=$(wildcard scripts/*.py)

default:
	$(MakeDir) $(PackageDir)
	@echo "__path__ = __import__('pkgutil').extend_path(__path__, __name__)" > pkg/$(Namespace)/__init__.py
	@cp -rf __init__.py $(PackageDir)

# Override, as this package uses pkg/setup.py as the template file
$(PackageSetupFile): pkg/setup.py
$(PackagePrepFile): $(PythonSources) Makefile | default
	$(MakeDir) $(ScriptDir)
	@cp -rf scripts/*  $(ScriptDir)
	@cp -rf core $(PackageDir)
	-cp -rf README.md LICENSE CHANGELOG.md MANIFEST.in requirements.txt $(PackageDir)
	-cp -rf README.md LICENSE CHANGELOG.md MANIFEST.in requirements.txt pkg
	touch $@

clean:
	@echo "Running clean target"
	-rm -rf $(ScriptDir)
	-rm -rf $(PackageDir)
	-rm -f  pkg/$(Namespace)/__init__.py
	-rm -f  pkg/README.md
	-rm -f  pkg/LICENSE
	-rm -f  pkg/MANIFEST.in
	-rm -f  pkg/CHANGELOG.md
	-rm -f  pkg/requirements.txt

print-env:
	@echo BUILD_HOME     $(BUILD_HOME)
	@echo PackagePath    $(PackagePath)
	@echo GIT_VERSION    $(GIT_VERSION)
	@echo PYTHON_VERSION $(PYTHON_VERSION)
	@echo GEMDEVELOPER   $(GEMDEVELOPER)
