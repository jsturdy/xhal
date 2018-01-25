ifndef PETA_STAGE
$(error "Error: PETA_STAGE environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif
include ../apps.common.mk

APP_OBJS=libmemsvc.o rwreg.o
LIB_HEADERS=CTP7.hh
APP=../../lib/arm/librwreg

all: build 

build: $(APP)

$(APP):$(APP_OBJS)
	$(CXX) -std=c++0x $(LDFLAGS) -fnon-call-exceptions -shared -o $@.so $(APP_OBJS) 

%.o: %.c
	$(CC) -std=gnu99 -c $(CFLAGS) -o $@ $<
%.o: %.cc
	$(CXX) -std=c++0x -c $(CFLAGS) -o $@ $<

clean:
	rm -rf *.so *.so.* *.o
