default:	
	rm -rf *.o
	rm -rf *.so
	gcc -fPIC -shared signals.c signals.so
	gcc -fPIC -shared jobs.c jobs.so
	gcc -fPIC -shared builtincmd.c builtincmd.so
	gcc -fPIC -shared string.c strings.so
	gcc main.c -o shell \/.signals.so \/.jobs.so \/.builtincms.so \/.strings.so
	rm -rf *~
