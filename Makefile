
COMPILE = g++ -std=c++11

ROOTINCLUDE = $(shell root-config --incdir)
ROOTLIBS    = $(shell root-config --libs) -lRooFit -lRooFitCore

all:
	$(COMPILE) -I$(ROOTINCLUDE) main.cpp $(ROOTLIBS) -o bin/main

clean:
	rm -rf bin/* obj/*
