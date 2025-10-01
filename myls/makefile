LIBDIR=-L/home/arthurlee/linux_tools/myls
INCLUDEDIR=-I/home/arthurlee/linux_tools/myls

all:libmyls.so main

libmyls.so:myls.cpp myls.h
	g++ -fPIC -shared -o libmyls.so myls.cpp

main: main.cpp  
	g++ -o main main.cpp  $(INCLUDEDIR) $(LIBDIR) -lmyls

clean:
	rm -f libmyls.so main