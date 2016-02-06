CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

all: cn.cpp cn.hpp Makefile permit 
	$(CC) -g cn.cpp -o cn
	rm -rf SunWS_cache core*

clean: permit
	rm -f cn SunWS_cache core*

permit:
	 chmod -R go-rwx *
