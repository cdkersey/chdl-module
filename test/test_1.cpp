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
  bvec<8> next_ctr, ctr(Reg(next_ctr)); next_ctr = ctr + Lit<8>(1);

  z80_bus_t io;

  _(io, "d").connect(ctr, Lit(1));

  EXPOSE(io);

  optimize();

  ofstream netlist("test_1.nand");
  print_netlist(netlist);

  return 0;
}
