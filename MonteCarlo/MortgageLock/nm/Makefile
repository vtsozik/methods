CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 #CC = /opt/gcc/3.2.2/bin/g++
 CC = /usr/lib/ccache/g++
endif

all: Calibrator.cpp Calibrator.hpp Formula.cpp Formula.hpp ZBrent.hpp Integral.hpp Integral.cpp Legendre.hpp Makefile normal.c pricer 
	$(CC) -O3 Integral.cpp Formula.cpp Calibrator.cpp normal.c -o calibrator.`uname`
	rm -rf SunWS_cache dump core*
	chmod go-rwx *

pricer: Pricer.hpp Pricer.cpp normal.c
	$(CC) -O3 -D"DAYS_IN_SERIES=`cat data.dat | wc -l`" Pricer.cpp normal.c -o pricer.`uname`
	rm -rf SunWS_cache dump core*
	chmod go-rwx *

clean:
	rm -f *.o SunWS_cache dump core* calibrator.[S,L]* pricer.[S,L]* *.o calib_results.txt
	chmod go-rwx *
