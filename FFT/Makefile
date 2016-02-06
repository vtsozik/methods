CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 #CC = /opt/gcc/3.2.2/bin/g++
 CC = g++
endif

all: fft.cpp Makefile
	$(CC) -g fft.cpp -o fft
	rm -rf SunWS_cache core*
	chmod go-rwx *

clean:
	rm -f fft SunWS_cache core* out.csv data.dat
	chmod go-rwx *
