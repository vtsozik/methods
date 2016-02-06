CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

all: rp.cpp fpt.cpp efpt.cpp normal.c
	#$(CC) -g rp.cpp -o rp.`uname`
	#$(CC) -g fpt.cpp -o fpt.`uname`
	$(CC) -g efpt.cpp normal.c -o efpt.`uname`
	rm -rf SunWS_cache core*
	chmod go-rwx *

clean:
	rm -f *.o SunWS_cache core* rp.[S,L]* fpt.[S,L]* efpt.[S,L]*
	chmod go-rwx *
