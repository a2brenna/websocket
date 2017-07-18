INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr
GITREF=\"$(shell git log | head -n 1 | awk '{ print $$2; }')\"
GITSTATUS=\"$(shell echo 'if [ "x$$(git status -s)" == "x" ]; then echo "clean"; else echo "dirty"; fi' | bash)\"

CXX=g++
CXXFLAGS=-DGITREF=${GITREF} -DGITSTATUS=${GITSTATUS} -L${LIBRARY_DIR} -I${INCLUDE_DIR} -march=native -O3 -flto -std=c++14 -fPIC -Wall -Wextra -fopenmp -fno-omit-frame-pointer -g

all: test

test: test.o websocket.o
	${CXX} ${CXXFLAGS} -o test test.o websocket.o

websocket.o: src/websocket.cc src/websocket.h
	${CXX} ${CXXFLAGS} -c src/websocket.cc -o websocket.o

test.o: src/test.cc
	${CXX} ${CXXFLAGS} -c src/test.cc -o test.o

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
	rm -f test
