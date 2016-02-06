CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

all: mtglck.cpp mtglck.hpp
	$(CC) -g mtglck.cpp -o mtglck.`uname`
	rm -rf SunWS_cache core* totals.csv data.dat
	./prepare_files.bsh
	chmod go-rwx *

clean:
	rm -f *.o SunWS_cache core* totals.csv data.dat mtglck.[S,L]* 
	chmod go-rwx *
