CXXFLAGS = -std=c++11 -fPIC
LDFLAGS = -shared
LDLIBS = -lchdl

libchdl-module.so: load_netlist.cpp loader.cpp loader.h
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -f main
