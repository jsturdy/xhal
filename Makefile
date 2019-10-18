SUBPACKAGES := \
        python \
        xhal

SUBPACKAGES.RPM        := $(patsubst %,%.rpm,         $(SUBPACKAGES))
SUBPACKAGES.CLEAN      := $(patsubst %,%.clean,       $(SUBPACKAGES))
SUBPACKAGES.CLEANRPM   := $(patsubst %,%.cleanrpm,    $(SUBPACKAGES))
SUBPACKAGES.CLEANALLRPM:= $(patsubst %,%.cleanallrpm, $(SUBPACKAGES))
SUBPACKAGES.CLEANALL   := $(patsubst %,%.cleanall,    $(SUBPACKAGES))
SUBPACKAGES.CHECKABI   := $(patsubst %,%.checkabi,    $(SUBPACKAGES))
SUBPACKAGES.INSTALL    := $(patsubst %,%.install,     $(SUBPACKAGES))
SUBPACKAGES.UNINSTALL  := $(patsubst %,%.uninstall,   $(SUBPACKAGES))
SUBPACKAGES.RELEASE    := $(patsubst %,%.release,     $(SUBPACKAGES))
SUBPACKAGES.DOC        := $(patsubst %,%.doc,         $(SUBPACKAGES))

.PHONY: $(SUBPACKAGES) \
	$(SUBPACKAGES.RPM) \
	$(SUBPACKAGES.CLEAN) \
	$(SUBPACKAGES.CLEANRPM) \
	$(SUBPACKAGES.CLEANALLRPM) \
	$(SUBPACKAGES.CLEANALL) \
	$(SUBPACKAGES.CHECKABI) \
	$(SUBPACKAGES.INSTALL) \
	$(SUBPACKAGES.UNINSTALL) \
	$(SUBPACKAGES.RELEASE)

all: $(SUBPACKAGES) $(SUBPACKAGES.RPM) $(SUBPACKAGES.DOC)

rpm: $(SUBPACKAGES) $(SUBPACKAGES.RPM)

doc: $(SUBPACKAGES.DOC)

cleanrpm: $(SUBPACKAGES.CLEANRPM)

cleandoc: $(SUBPACKAGES.CLEANDOC)

clean: $(SUBPACKAGES.CLEAN) $(SUBPACKAGES.CLEANRPM) $(SUBPACKAGES.CLEANDOC) 

cleanallrpm: $(SUBPACKAGES.CLEANALLRPM)

cleanall: $(SUBPACKAGES.CLEANALL)

install: $(SUBPACKAGES.INSTALL)

uninstall: $(SUBPACKAGES.UNINSTALL)

release: $(SUBPACKAGES.RELEASE)

$(SUBPACKAGES):
	$(MAKE) -C $@

$(SUBPACKAGES.RPM):
	$(MAKE) -C $(patsubst %.rpm,%, $@) rpm

$(SUBPACKAGES.DOC):
	$(MAKE) -C $(patsubst %.doc,%, $@) doc

$(SUBPACKAGES.CLEAN):
	$(MAKE) -C $(patsubst %.clean,%, $@) clean

$(SUBPACKAGES.CLEANRPM):
	$(MAKE) -C $(patsubst %.cleanrpm,%, $@) cleanrpm

$(SUBPACKAGES.CLEANDOC):
	$(MAKE) -C $(patsubst %.cleandoc,%, $@) cleandoc

$(SUBPACKAGES.CLEANALLRPM):
	$(MAKE) -C $(patsubst %.cleanallrpm,%, $@) cleanallrpm

$(SUBPACKAGES.CLEANALL):
	$(MAKE) -C $(patsubst %.cleanall,%, $@) cleanall

$(SUBPACKAGES.CHECKABI):
	$(MAKE) -C $(patsubst %.checkabi,%, $@) checkabi

$(SUBPACKAGES.INSTALL): $(SUBPACKAGES)
	$(MAKE) -C $(patsubst %.install,%, $@) install

$(SUBPACKAGES.UNINSTALL):
	$(MAKE) -C $(patsubst %.uninstall,%, $@) uninstall

$(SUBPACKAGES.RELEASE):
	$(MAKE) -C $(patsubst %.release,%, $@) release
