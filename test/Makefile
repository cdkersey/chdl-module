CXXFLAGS=-std=c++11 -I../
LDLIBS=-lchdl -lchdl-module

all: test_1 load_z80

load_z80: load_z80.cpp iface.h

test_1: test_1.cpp iface.h

clean:
	rm -f test_1 load_z80 *.nand *.vcd
