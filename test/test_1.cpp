#include <chdl/chdl.h>

#include <loader.h>
#include <chdl/ag.h>
#include <chdl/memreq.h>

#include "iface.h"

using namespace chdl;
using namespace std;

void show(const runtime_ag_t &r) {
  for (auto &p : r) {
    cout << p.first << ':';
    for (auto n : p.second)
      cout << ' ' << n;
    cout << endl;
  }
}

int main() {
  z80_bus_t io;

  EXPOSE(io);

  optimize();

  ofstream netlist("main.nand");
  print_netlist(netlist);

  return 0;
}
