
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include <chdl/chdl.h>

#include <sys/time.h>

using namespace std;
using namespace chdl;

// The ability to read pre-flattened and pre-optimized netlists is important for
// the future of CHDL.

void die(const char* msg) {
  cerr << msg << endl;
  exit(1);
}

unsigned long time_us() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

namespace chdl {

typedef map<string, vector<node> > input_t;

void read_netlist(const char* filename, input_t inputs) {
  std::vector<node> memory_internal(
    std::vector<node> &qa, std::vector<node> &d, std::vector<node> &da,
    node w, std::string filename, bool sync, size_t &id
  );
  std::vector<node> memory_add_read_port(size_t id, std::vector<node> &qa);

  ifstream in(filename);

  map<nodeid_t, node> nodes;

  string eat;
  in >> eat;
  if (eat != "inputs") die("Malformed netlist (at top).");
  while (in.get() == '\n' && in.get() == ' ') {
    in >> eat;
    unsigned idx = 0;
    while (in.peek() != '\n') {
      unsigned node;
      in >> node;
      nodes[node] = inputs[eat][idx];
      ++idx;
    }
  }

  in >> eat;

  if (eat != "utputs") die("Malformed netlist (after inputs).");

  while (in.get() == '\n' && in.get() == ' ') {
    in >> eat;
    unsigned idx = 0;
    while (in.peek() != '\n') {
      unsigned node;
      in >> node;
      tap(eat, nodes[node]);
      ++idx;
    }
  }

  in >> eat;
  if (eat != "esign") die("Malformed netlist (at end of taps).");

  while (in) {
    in >> eat;
    nodeid_t in0, in1, out;
    if (eat == "lit0") {
      in >> out;
      nodes[out] = Lit(0);
    } else if (eat == "lit1") {
      in >> out;
      nodes[out] = Lit(1);
    } else if (eat == "inv") {
      in >> in0 >> out;
      nodes[out] = Inv(nodes[in0]);
    } else if (eat == "nand") {
      in >> in0 >> in1 >> out;
      nodes[out] = Nand(nodes[in0], nodes[in1]);
    } else if (eat == "reg") {
      in >> in0 >> out;
      nodes[out] = Reg(nodes[in0]);
    } else if (eat == "syncram") {
      while (in.get() != '<');
      unsigned abits, dbits; in >> abits; in >> dbits;
      if (in.get() != '>') die("Malformed memory.");
      vector<node> da, d;
      vector<vector<node>> qa, q;

      for (unsigned i = 0; i < abits; ++i) {
        nodeid_t x;
        in >> x;
        da.push_back(nodes[x]);
      }

      for (unsigned i = 0; i < dbits; ++i) {
        nodeid_t x;
        in >> x;
        d.push_back(nodes[x]);
      }

      nodeid_t w_nid; in >> w_nid;
      node w(nodes[w_nid]);

      unsigned port(0);
      while (in.peek() != '\n') {
        qa.push_back(vector<node>());
        q.push_back(vector<node>());

        for (unsigned i = 0; i < abits; ++i) {
          nodeid_t x;
          in >> x;
          qa[port].push_back(nodes[x]);
        }

        for (unsigned i = 0; i < dbits; ++i) {
          nodeid_t x;
          in >> x;
          q[port].push_back(nodes[x]);
        }

        ++port;
      }

      size_t id;
      
      vector<node> q0(memory_internal(qa[0], d, da, w, "", true, id));
      for (unsigned i = 0; i < q0.size(); ++i) q[0][i] = q0[i];

      for (unsigned i = 1; i < q.size(); ++i) {
        vector<node> qi = memory_add_read_port(id, qa[i]);
        for (unsigned j = 0; j < qi.size(); ++j) q[i][j] = qi[j];
      }
    } else {
      die(("Unrecognized node type: " + eat).c_str());
    }
  }
}

}

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage:" << endl << "  " << argv[0] << " <nand file>" << endl;
    return 1;
  }

  input_t inputs;

  read_netlist(argv[1], inputs);
  optimize();

  // opt_dead_node_elimination();

  // ofstream wave_file("waveform.vcd");
  // run(wave_file, 10000);

  return 0;
}
