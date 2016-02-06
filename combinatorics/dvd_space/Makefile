CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

all:comb.cpp Makefile
	$(CC) -g comb.cpp -o comb
	rm -rf SunWS_cache core*
	chmod go-rwx *

clean:
	rm -f comb SunWS_cache core*
	chmod go-rwx *
