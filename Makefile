CC = g++

CPPFLAGS = -std=c++11 -g -Wall

LDFLAGS = -lm

all: unit-tests

unit-tests.o:    unit-tests.cpp
CCDataSet.o:     CCDataSet.cc
CCImageReader.o: CCImageReader.cc

unit-tests: unit-tests.o CCDataSet.o CCImageReader.o

clean:
	rm -f *.o
	rm -f unit-tests
