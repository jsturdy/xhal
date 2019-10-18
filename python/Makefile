SUBPACKAGES := \
        reg_interface_gem

SUBPACKAGES.CLEAN    := $(patsubst %,%.clean,    $(SUBPACKAGES))
SUBPACKAGES.DEBUG    := $(patsubst %,%.debug,    $(SUBPACKAGES))
SUBPACKAGES.INSTALL  := $(patsubst %,%.install,  $(SUBPACKAGES))
SUBPACKAGES.RELEASE  := $(patsubst %,%.release,  $(SUBPACKAGES))
SUBPACKAGES.UNINSTALL:= $(patsubst %,%.uninstall,$(SUBPACKAGES))
SUBPACKAGES.RPM      := $(patsubst %,%.rpm,      $(SUBPACKAGES))
SUBPACKAGES.CLEANRPM := $(patsubst %,%.cleanrpm, $(SUBPACKAGES))
SUBPACKAGES.DOC      := $(patsubst %,%.doc,      $(SUBPACKAGES))
SUBPACKAGES.CLEANDOC := $(patsubst %,%.cleandoc, $(SUBPACKAGES))
SUBPACKAGES.CLEANALL := $(patsubst %,%.cleanall, $(SUBPACKAGES))
SUBPACKAGES.CHECKABI := $(patsubst %,%.checkabi, $(SUBPACKAGES))

.PHONY: $(SUBPACKAGES) \
	$(SUBPACKAGES.CLEAN) \
	$(SUBPACKAGES.INSTALL) \
	$(SUBPACKAGES.UNINSTALL) \
	$(SUBPACKAGES.RELEASE) \
	$(SUBPACKAGES.RPM) \
	$(SUBPACKAGES.CLEANRPM) \
	$(SUBPACKAGES.DOC) \
	$(SUBPACKAGES.CLEANDOC) \
	$(SUBPACKAGES.CLEANALL) \
	$(SUBPACKAGES.CHECKABI)

.PHONY: all build checkabi doc install uninstall rpm release
.PHONY: clean cleanall cleandoc cleanrpm
build: $(SUBPACKAGES)

all: $(SUBPACKAGES) $(SUBPACKAGES.DOC)

rpm: $(SUBPACKAGES) $(SUBPACKAGES.RPM)

doc: $(SUBPACKAGES.DOC)

clean: $(SUBPACKAGES.CLEAN)

cleandoc: $(SUBPACKAGES.CLEANDOC)

cleanrpm: $(SUBPACKAGES.CLEANRPM)

install: $(SUBPACKAGES.INSTALL)

uninstall: $(SUBPACKAGES.UNINSTALL)

release: $(SUBPACKAGES.RELEASE)

cleanall: $(SUBPACKAGES.CLEANALL)

checkabi: $(SUBPACKAGES.CHECKABI)

$(SUBPACKAGES):
	$(MAKE) -C $@

$(SUBPACKAGES.RPM):
	$(MAKE) -C $(patsubst %.rpm,%, $@) rpm

$(SUBPACKAGES.CLEAN):
	$(MAKE) -C $(patsubst %.clean,%, $@) clean

$(SUBPACKAGES.CLEANDOC):
	$(MAKE) -C $(patsubst %.cleandoc,%, $@) cleandoc

$(SUBPACKAGES.CLEANRPM):
	$(MAKE) -C $(patsubst %.cleanrpm,%, $@) cleanrpm

$(SUBPACKAGES.CLEANALL):
	$(MAKE) -C $(patsubst %.cleanall,%, $@) cleanall

$(SUBPACKAGES.CHECKABI):


$(SUBPACKAGES.DOC):
	$(MAKE) -C $(patsubst %.doc,%, $@) html

$(SUBPACKAGES.INSTALL):
	$(MAKE) -C $(patsubst %.install,%, $@) install-site

$(SUBPACKAGES.UNINSTALL):
	$(MAKE) -C $(patsubst %.uninstall,%, $@) uninstall-site

$(SUBPACKAGES.RELEASE): $(SUBPACKAGES)
	$(MAKE) -C $(patsubst %.release,%, $@) release
