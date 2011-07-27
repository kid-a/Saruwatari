#include <systemc.h>

SC_MODULE(test_bench) 
{
  sc_in<bool> clock;
  sc_out<int> a;
  sc_out<int> b;

  void generate_signals();

  SC_CTOR(test_bench)
  {
    SC_CTHREAD(generate_signals, clock.pos());
  }
};
