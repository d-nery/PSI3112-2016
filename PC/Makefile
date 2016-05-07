CXX      = g++ -std=c++11
DEBUG    = -g -DDEBUG
CXXFLAGS = -Wall -Wextra -lncurses -lpthread

OBJECTS  = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.PHONY: all debug clean

all: interface

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

interface: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o interface $(OBJECTS)

debug: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(DEBUG) -o interface $(OBJECTS)

clean:
	rm -rf $(OBJECTS)
