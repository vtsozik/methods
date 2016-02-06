all:	millerrabin.hpp testmillerrabin.cpp rsa.hpp testrsa.cpp eratosthenes.hpp testeratosthenes.cpp instcal.cpp rsawrapper.hpp rsamain.cpp rsaverify.java Makefile
	#g++ -O3 -g -Wall testmillerrabin.cpp -o testmillerrabin.`uname`
	#g++ -O3 -g -Wall testrsa.cpp -o testrsa.st.`uname`
	#g++ -O3 -pg -g -Wall testeratosthenes.cpp -o testeratosthenes.`uname`
	#g++ -g -pg -Wall testrsa.cpp -o testrsa.st.`uname`
	g++ -O3 -Wall testrsa.cpp -o testrsa.st.`uname`
	#g++ -g -Wall testrsa.cpp -DPARALLEL_GEN -o testrsa.mt.`uname` -lpthread
	g++ -fauto-inc-dec -fcprop-registers -fdce -fdefer-pop -fdelayed-branch -fdse -fguess-branch-probability -fif-conversion2 -fif-conversion -finline-small-functions -fipa-pure-const -fipa-reference -fmerge-constants -fsplit-wide-types -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-copyrename -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-fre -ftree-sra -ftree-ter -funit-at-a-time -falign-functions  -falign-jumps -falign-loops  -falign-labels -fcaller-saves -fcrossjumping -fcse-follow-jumps  -fcse-skip-blocks -fdelete-null-pointer-checks -fexpensive-optimizations -fgcse  -fgcse-lm -findirect-inlining -foptimize-sibling-calls -fpeephole2 -freorder-blocks  -freorder-functions -frerun-cse-after-loop -fsched-interblock  -fsched-spec -fschedule-insns2 -fstrict-aliasing -fstrict-overflow -ftree-switch-conversion -ftree-pre -ftree-vrp -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize -pthread -Wall testrsa.cpp -DPARALLEL_GEN -o testrsa.mt.`uname`
	#g++ -g -Wall testrsa.cpp -o testrsa.`uname` -pthread
	#g++ -fopenmp -Wall testrsa.cpp -o testrsa.omp.`uname`
	#g++ -O3 -Wall instcal.cpp -o instcal.`uname`
	#g++ -O3 -Wall rsamain.cpp -o rsamain.`uname`
	#javac rsaverify.java

clean:
	rm -f *.`uname` core.* dump *.class *.out *.enc *.dec 
