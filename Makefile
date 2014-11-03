CXXFLAGS = -std=c++11 -g
LDLIBS = -lchdl

all: main load_netlist

main: main.cpp loader.cpp loader.h
load_netlist: load_netlist.cpp

clean:
	rm -f main
