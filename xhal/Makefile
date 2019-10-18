TARGETS := arm x86_64

TARGETS.RPM        := $(patsubst %,%.rpm,         $(TARGETS))
TARGETS.CLEAN      := $(patsubst %,%.clean,       $(TARGETS))
TARGETS.CLEANRPM   := $(patsubst %,%.cleanrpm,    $(TARGETS))
TARGETS.CLEANALLRPM:= $(patsubst %,%.cleanallrpm, $(TARGETS))
TARGETS.CLEANALL   := $(patsubst %,%.cleanall,    $(TARGETS))
TARGETS.CHECKABI   := $(patsubst %,%.checkabi,    $(TARGETS))
TARGETS.INSTALL    := $(patsubst %,%.install,     $(TARGETS))
TARGETS.UNINSTALL  := $(patsubst %,%.uninstall,   $(TARGETS))
TARGETS.RELEASE    := $(patsubst %,%.release,     $(TARGETS))

.PHONY: $(TARGETS) \
	$(TARGETS.RPM) \
	$(TARGETS.CLEAN) \
	$(TARGETS.CLEANRPM) \
	$(TARGETS.CLEANALLRPM) \
	$(TARGETS.CLEANALL) \
	$(TARGETS.CHECKABI) \
	$(TARGETS.INSTALL) \
	$(TARGETS.UNINSTALL) \
	$(TARGETS.RELEASE)

.PHONY: all default install uninstall rpm release
.PHONY: clean cleanrpm cleandoc cleanallrpm cleanall

default: all

all: $(TARGETS) doc

rpm: $(TARGETS) $(TARGETS.RPM)

clean: $(TARGETS.CLEAN)

cleanall: $(TARGETS.CLEANALL) cleandoc

cleanrpm: $(TARGETS.CLEANRPM)

cleanallrpm: $(TARGETS.CLEANALLRPM)

checkabi: $(TARGETS.CHECKABI)

install: $(TARGETS.INSTALL)

uninstall: $(TARGETS.UNINSTALL)

release: $(TARGETS.RELEASE)

$(TARGETS):
	TargetArch=$@ $(MAKE) -f xhal.mk build

$(TARGETS.RPM):
	TargetArch=$(patsubst %.rpm,%,$@) $(MAKE) -f xhal.mk rpm

$(TARGETS.CLEAN):
	TargetArch=$(patsubst %.clean,%,$@) $(MAKE) -f xhal.mk clean

$(TARGETS.CLEANRPM):
	TargetArch=$(patsubst %.cleanrpm,%,$@) $(MAKE) -f xhal.mk cleanrpm

$(TARGETS.CLEANALLRPM):
	TargetArch=$(patsubst %.cleanallrpm,%,$@) $(MAKE) -f xhal.mk cleanallrpm

$(TARGETS.CLEANALL):
	TargetArch=$(patsubst %.cleanall,%,$@) $(MAKE) -f xhal.mk cleanall

$(TARGETS.CHECKABI):
	TargetArch=$(patsubst %.checkabi,%,$@) $(MAKE) -f xhal.mk checkabi

$(TARGETS.INSTALL):
	TargetArch=$(patsubst %.install,%,$@) $(MAKE) -f xhal.mk install

$(TARGETS.UNINSTALL):
	TargetArch=$(patsubst %.uninstall,%,$@) $(MAKE) -f xhal.mk uninstall

$(TARGETS.RELEASE):
	TargetArch=$(patsubst %.release,%,$@) $(MAKE) -f xhal.mk release

doc:
	@echo "TO DO"

cleandoc: 
	@echo "TO DO"

arm:

x86_64:

ctp7:

bcp:

apx:
