all: drv.cpp
	#g++ drv.cpp -o drv
	g++ stpst.cpp drv.cpp -o stpst
	chmod -R go-rwx *

clean:	
	rm -rf core* dump drv stpst
	chmod -R go-rwx *
