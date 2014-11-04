#include <iostream>

#include <chdl/chdl.h>

#include <loader.h>
#include <chdl/ag.h>
#include <chdl/memreq.h>

#include "iface.h"

using namespace chdl;
using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage:" << endl << "  " << argv[0] << " <netlist>" << endl;
    return 1; 
  }

  z80_bus_t io;
  Load(argv[1])("io", io);
  TAP(io);

  optimize();

  ofstream netlist("load_z80.nand");
  print_netlist(netlist);

  ofstream vcd("load_z80.vcd");
  run(vcd, 1000);

  return 0;
}
