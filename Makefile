PREFIX ?= /usr/local
CHDL_INCLUDE ?= $(PREFIX)/include/chdl
CHDL_LIB ?= $(PREFIX)/lib
CXXFLAGS += -std=c++11 -fPIC -I$(PREFIX)/include
LDFLAGS += -shared -L$(PREFIX)/lib
LDLIBS += -lchdl

libchdl-module.so: load_netlist.cpp loader.cpp loader.h
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) load_netlist.cpp loader.cpp $(LDLIBS)

install: libchdl-module.so
	mkdir -p $(CHDL_INCLUDE)
	mkdir -p $(CHDL_LIB)
	cp loader.h $(CHDL_INCLUDE)
	cp libchdl-module.so $(CHDL_LIB)

clean:
	rm -f *.so *.o
