#ifndef CHDL_LOADER_H
#define CHDL_LOADER_H

#include <chdl/chdl.h>
#include <chdl/ag.h>
#include <chdl/dir.h>
#include <map>
#include <vector>
#include <sstream>
#include <string>

namespace chdl {
  // Turn a string type into a c++ string.
  template <typename T> std::string Stringify(const T &x) { return ""; }

  template <char C, typename NEXT>
    std::string Stringify(const strtype<C, NEXT> &x)
  {
    return std::string(1, C) + Stringify(NEXT());
  }

  typedef std::map<std::string, std::vector<node> > runtime_ag_t;

  // This should probably get a more dignified name than "runtimify".
  template <typename T>
    void Runtimify(runtime_ag_t &r, const T &x, std::string prefix = "",
                   direction_t dir = DIR_ALL)
  { }

  static void Runtimify(runtime_ag_t &r, const node &x, std::string prefix = "",
                        direction_t dir = DIR_ALL)
  {
    r[prefix].push_back(x);
  }

  template <unsigned N>
    void Runtimify(runtime_ag_t &r, const bvec<N> &x, std::string prefix = "",
                   direction_t dir = DIR_ALL)
  {
    for (unsigned i = 0; i < N; ++i) r[prefix].push_back(x[i]);
  }

  template <unsigned N>
    void Runtimify(runtime_ag_t &r, const bus<N> &x, std::string prefix = "",
                   direction_t dir = DIR_ALL)
  {
    for (unsigned i = 0; i < N; ++i) r[prefix].push_back(x[i]);
  }

  template <unsigned N, typename T>
    void Runtimify(runtime_ag_t &r, const vec<N, T> &x, std::string prefix = "",
                   direction_t dir = DIR_ALL)
  {
    for (unsigned i = 0; i < N; ++i) {
      std::ostringstream oss;
      oss << i;
      Runtimify(r, x[i], prefix + "_" + oss.str());
    }
  }

  template <typename T>
    void Runtimify(runtime_ag_t &r, const in<T> &x, std::string prefix = "",
                   direction_t dir = DIR_ALL)
  {
    if (dir == DIR_IN || dir == DIR_ALL)
      Runtimify(r, T(x), prefix, dir);
  }

  template <typename T>
    void Runtimify(runtime_ag_t &r, const out<T> &x, std::string prefix = "",
                   direction_t dir = DIR_ALL)
  {
    if (dir == DIR_OUT || dir == DIR_ALL)
      Runtimify(r, T(x), prefix, dir);
  }

  template <typename T>
    void Runtimify(runtime_ag_t &r, const inout<T> &x, std::string prefix = "",
                   direction_t dir = DIR_ALL)
  {
    if (dir == DIR_INOUT || dir == DIR_ALL)
      Runtimify(r, T(x), prefix, dir);
  }

  template <typename NAME, typename T, typename NEXT>
    void Runtimify(runtime_ag_t &r, const ag<NAME,T,NEXT> &x,
                   std::string prefix = "", direction_t dir = DIR_ALL)
  {
    Runtimify(r, x.contents, prefix + "_" + Stringify(NAME()), dir);
    Runtimify(r, x.next, prefix, dir);
  }

  template <typename NAME, typename T, typename NEXT>
    runtime_ag_t Runtimify(const ag<NAME, T, NEXT> &x, std::string prefix = "",
                           direction_t dir = DIR_ALL)
  {
    runtime_ag_t r;
    Runtimify(r, x, prefix, dir);
    return r;
  }

  // Expose a module interface.
  template <typename T> void Expose(std::string name, const T &iface) {
    using namespace std;

    runtime_ag_t inputs(Runtimify(iface, name, DIR_IN)),
                 outputs(Runtimify(iface, name, DIR_OUT)),
                 inouts(Runtimify(iface, name, DIR_INOUT));

    // Use the internal, vector-based function for inputs.
    for (auto &p : inputs) {
      for (auto &n : p.second) {
        vector<node> in(input_internal(p.first, p.second.size()));
        for (unsigned i = 0; i < p.second.size(); ++i)
          p.second[i] = in[i];
      }
    }

    for (auto &p : outputs)
      for (auto &n : p.second)
        tap(p.first, n, true);

    for (auto &p : inouts)
      for (auto &n : p.second)
        tap(p.first, tristatenode(nodeid_t(n)), true);
  }

  #define EXPOSE(x) do { Expose(#x, x); } while(0)

  template <typename T>
    T Bind(std::string name,
           std::map<std::string, std::vector<node> > &in,
           std::map<std::string, std::vector<node> > &out,
           std::map<std::string, std::vector<tristatenode> > &inout)
  {
    using namespace std;

    // The value we'll return, default constructed; all signals independent.
    T r;

    // The runtime version of the return value, ready for assignment.
    runtime_ag_t in_module(Runtimify(r, name, DIR_IN)),
                 out_module(Runtimify(r, name, DIR_OUT)),
                 inout_module(Runtimify(r, name, DIR_INOUT));

    // TODO: make sure vector sizes match

    // Assign nodes from the arguments to our return value's fields.
    for (auto &x : in_module)
      for (unsigned i = 0; i < x.second.size(); ++i)
        x.second[i] = in[x.first][i];

    for (auto &x : out_module)
      for (unsigned i = 0; i < x.second.size(); ++i)
        x.second[i] = out[x.first][i];

    for (auto &x : inout_module)
      for (unsigned i = 0; i < x.second.size(); ++i)
        x.second[i] = inout[x.first][i];

    return r;
  }

  struct iface_t {
    std::map<std::string, std::vector<node> > in, out;
    std::map<std::string, std::vector<tristatenode> > inout;

    template <typename T> iface_t operator()(std::string name, T &x) {
      x = Bind<T>(name, in, out, inout);
      return *this;
    }
  };

  iface_t Load(std::string filename);
};

#endif
