
all:
	g++ -fPIC -I/usr/lib/dart/include -c helloworld.cpp -o helloworld.o
	g++ -shared -Wl,-soname,libhelloworld.so -o libhelloworld.so helloworld.o

clean:
	-rm *.dll *.o *.a *.exe *.so