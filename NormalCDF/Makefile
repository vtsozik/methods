CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

all: cdf.cpp Makefile
	$(CC) -g cdf.cpp -o cdf
	rm -rf SunWS_cache core*
	chmod go-rwx *

clean:
	rm -f cdf SunWS_cache core*
	chmod go-rwx *
