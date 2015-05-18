CXXFLAGS = -std=c++11 -fPIC
LDFLAGS = -shared
LDLIBS = -lchdl
PREFIX ?= /usr/local
CHDL_INCLUDE ?= $(PREFIX)/include/chdl
CHDL_LIB ?= $(PREFIX)/lib

libchdl-module.so: load_netlist.cpp loader.cpp loader.h
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS)

install:
	cp loader.h $(CHDL_INCLUDE)
	cp libchdl-module.so $(CHDL_LIB)
	ldconfig

clean:
	rm -f *.so *.o
