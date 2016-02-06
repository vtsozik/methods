CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 #CC = /opt/gcc/3.2.2/bin/g++
 CC = /usr/lib/ccache/g++
endif

all: matrix.hpp cqr.cpp cqr.hpp lpl.cpp lpl.hpp cegvl.cpp cegvl.hpp sleq.cpp sleq.hpp egvt.cpp egvt.hpp svd.cpp svd.hpp Makefile
	$(CC) -g cqr.cpp lpl.cpp cegvl.cpp sleq.cpp egvt.cpp svd.cpp -o svd.`uname`
	rm -rf SunWS_cache core*

clean:
	rm -rf svd.`uname` *.o SunWS_cache core*
