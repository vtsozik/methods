CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

all: qr.cpp qr.hpp lpl.cpp lpl.hpp egvl.cpp egvl.hpp sleq.cpp sleq.hpp egvt.cpp egvt.hpp Makefile
	$(CC) -g qr.cpp lpl.cpp egvl.cpp sleq.cpp egvt.cpp -o qrm.`uname`
	rm -rf SunWS_cache core*
	chmod go-rwx *

clean:
	rm -f qrm* core* *.o SunWS_cache
	chmod go-rwx *
