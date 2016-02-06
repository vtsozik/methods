CC = /opt/forte/6.2_patch4/bin/CC
ARCH = $(shell uname)
ifeq ($(ARCH),Linux) 
 CC = /opt/gcc/3.2.2/bin/g++
endif

TARGET=legendre.`uname`

all : $(TARGET)

$(TARGET) : cegvl.o cqr.o legendre.cpp legendre.hpp Makefile
	$(CC) -g legendre.cpp cegvl.o cqr.o -o $(TARGET)
	rm -rf SunWS_cache core*
	ls -l|grep -v '^total '|grep -v '\-\-\-\-\-\-'|awk '{print $$9}'|xargs chmod -R go-rwx .

cegvl.o : cegvl.cpp cegvl.hpp
	$(CC) -g -c cegvl.cpp -o cegvl.o

cqr.o : cqr.cpp cqr.hpp
	$(CC) -g -c cqr.cpp -o cqr.o

clean:
	rm -f $(TARGET) dump* *.o SunWS_cache core*
	ls -l|grep -v '^total '|grep -v '\-\-\-\-\-\-'|awk '{print $$9}'|xargs chmod -R go-rwx .
