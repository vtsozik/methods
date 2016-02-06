CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

all: gbm.cpp
	$(CC) -g gbm.cpp normal.c -o gbm.`uname`
	rm -rf SunWS_cache core*
	chmod go-rwx *

clean:
	rm -f *.o SunWS_cache core* gbm.[S,L]* 
	chmod go-rwx *
