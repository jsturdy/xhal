SUBPACKAGES := \
        python \
        xhal

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
	$(SUBPACKAGES.CHECKABI) \
	$(SUBPACKAGES.CLEANALL)

.PHONY: all build checkabi doc install uninstall rpm release
.PHONY: clean cleanall cleandoc cleanrpm cleanrelease
build: $(SUBPACKAGES)

all: $(SUBPACKAGES) $(SUBPACKAGES.DOC)

doc: $(SUBPACKAGES.DOC)
	@echo "Generating project doxygen"
	@echo "TO DO"
#	@mkdir ./doc/build
#	@rm -fr ./doc/build/* 2> /dev/null
#	@doxygen -s ./doc/doxygen_conf
# copy generated docs to common location for release?

rpm: $(SUBPACKAGES) $(SUBPACKAGES.RPM)

clean: $(SUBPACKAGES.CLEAN)

cleanrpm: $(SUBPACKAGES.CLEANRPM)

cleandoc: $(SUBPACKAGES.CLEANDOC)
# remove generated common docs

install: $(SUBPACKAGES) $(SUBPACKAGES.INSTALL)

uninstall: $(SUBPACKAGES.UNINSTALL)

release: $(SUBPACKAGES.RELEASE)
# put generated files into release tree

cleanrelease:
	-rm -rf release

cleanall: $(SUBPACKAGES.CLEANALL) cleanrelease

checkabi: $(SUBPACKAGES.CHECKABI)

$(SUBPACKAGES):
	$(MAKE) -C $@

$(SUBPACKAGES.RPM): $(SUBPACKAGES)
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
	$(MAKE) -C $(patsubst %.checkabi,%, $@) checkabi

$(SUBPACKAGES.DOC):
	$(MAKE) -C $(patsubst %.doc,%, $@) doc

$(SUBPACKAGES.INSTALL): $(SUBPACKAGES)
	$(MAKE) -C $(patsubst %.install,%, $@) install

$(SUBPACKAGES.UNINSTALL): $(SUBPACKAGES)
	$(MAKE) -C $(patsubst %.uninstall,%, $@) uninstall

$(SUBPACKAGES.RELEASE): $(SUBPACKAGES)
	$(MAKE) -C $(patsubst %.release,%, $@) release

python:
