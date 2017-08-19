INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr
GITREF=\"$(shell git log | head -n 1 | awk '{ print $$2; }')\"
GITSTATUS=\"$(shell echo 'if [ "x$$(git status -s)" == "x" ]; then echo "clean"; else echo "dirty"; fi' | bash)\"

CXX=g++
CXXFLAGS=-DGITREF=${GITREF} -DGITSTATUS=${GITSTATUS} -L${LIBRARY_DIR} -I${INCLUDE_DIR} -march=native -O3 -flto -std=c++14 -fPIC -Wall -Wextra -fopenmp -fno-omit-frame-pointer -g

all: test libwebsocket.so libwebsocket.a

test: test.o websocket.o encode.o transport.o
	${CXX} ${CXXFLAGS} -o test test.o websocket.o transport.o encode.o -lsodium -lgnutls

libwebsocket.so: websocket.o transport.o encode.o
	${CXX} ${CXXFLAGS} -shared -Wl,-soname,libwebsocket.so -o libwebsocket.so websocket.o transport.o encode.o

libwebsocket.a: websocket.o transport.o encode.o
	 ar rcs libwebsocket.a websocket.o transport.o encode.o

websocket.o: src/websocket.cc src/websocket.h
	${CXX} ${CXXFLAGS} -c src/websocket.cc -o websocket.o

transport.o: src/transport.cc src/transport.h
	${CXX} ${CXXFLAGS} -c src/transport.cc -o transport.o

test.o: src/test.cc
	${CXX} ${CXXFLAGS} -c src/test.cc -o test.o

encode.o: src/encode.cc
	${CXX} ${CXXFLAGS} -c src/encode.cc -o encode.o

install: libwebsocket.so libwebsocket.a src/websocket.h src/transport.h
	mkdir -p ${DESTDIR}/${PREFIX}/lib
	cp *.a ${DESTDIR}/${PREFIX}/lib
	cp *.so ${DESTDIR}/${PREFIX}/lib
	mkdir -p ${DESTDIR}/${PREFIX}/include/websocket/
	cp src/websocket.h ${DESTDIR}/${PREFIX}/include/websocket/
	cp src/transport.h ${DESTDIR}/${PREFIX}/include/websocket/

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
	rm -f test
