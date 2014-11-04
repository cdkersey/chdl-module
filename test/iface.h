#ifndef CHDL_MODULE_TEST_IFACE_H
#define CHDL_MODULE_TEST_IFACE_H

#include <chdl/chdl.h>
#include <chdl/ag.h>
#include <chdl/dir.h>

namespace chdl {
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
          ag<STP("a"), inout<bus<16> >, // (can be tri-stated)
          ag<STP("d"), inout<bus<8> > > > > > > > > > > > > > > > z80_bus_t;
}

#endif
