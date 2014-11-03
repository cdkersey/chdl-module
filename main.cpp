#include <chdl/chdl.h>

#include "loader.h"
#include <chdl/ag.h>
#include <chdl/memreq.h>

using namespace chdl;
using namespace std;

// Z80-like interface for testing
typedef ag<STP("rd"), out<node>,
        ag<STP("wr"), out<node>,
        ag<STP("mreq"), out<node>,
        ag<STP("ioreq"), out<node>,
        ag<STP("wait"), in<node>,
        ag<STP("busreq"), in<node>,
        ag<STP("busack"), out<node>,
        ag<STP("int"), in<node>,
        ag<STP("nmi"), in<node>,
        ag<STP("m1"), out<node>,
        ag<STP("halt"), out<node>,
        ag<STP("refsh"), out<node>,
        ag<STP("a"), inout<bvec<16> >, // (can be tri-stated)
        ag<STP("d"), inout<bvec<8> > > > > > > > > > > > > > > > z80_bus_t;

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
