COMPILE = g++ -std=c++11

# make use of 'root-config' to get the root stuff that we need
ROOTINCLUDE = $(shell root-config --incdir)
ROOTLIBS    = $(shell root-config --libs) -lRooFit -lRooFitCore

# string substitution and wildcard magic to get a list of object targets
OBJECTS     = $(subst src/, obj/, $(subst .cpp,.o, $(wildcard src/*.cpp)))

# build all objects then the main executable
all: $(OBJECTS)
	$(COMPILE) -Isrc -I$(ROOTINCLUDE) $(OBJECTS) main.cpp $(ROOTLIBS) -o bin/main

# build all objects
obj/%.o: src/%.cpp
	$(COMPILE) -Isrc -I$(ROOTINCLUDE) -c $^ $(ROOTLIBS) -o $@

# delete stuff
clean:
	rm -rf bin/* obj/*

info:
	echo $(OBJS)
