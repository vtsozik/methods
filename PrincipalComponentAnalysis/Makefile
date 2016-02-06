CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

all: matrix.hpp cqr.cpp cqr.hpp lpl.cpp lpl.hpp cegvl.cpp cegvl.hpp sleq.cpp sleq.hpp egvt.cpp egvt.hpp Makefile
	$(CC) -g cqr.cpp lpl.cpp cegvl.cpp sleq.cpp egvt.cpp -o pcam.`uname`
	rm -rf SunWS_cache core*
	chmod go-rwx *

clean:
	rm -f pcam* *.o SunWS_cache core*
	chmod go-rwx *
