#include <systemc.h>
#include "adder.h"
#include "tb_adder.h"

int sc_main(int, char * argv[]) 
{

  sc_signal<int> a;
  sc_signal<int> b;
  sc_signal<int> sum;
  sc_clock clk("Clock", 10, SC_NS);

  adder add("Adder");
  add(a, b, sum);

  test_bench tb("TB");
  tb(clk, a, b);

  sc_trace_file * tf = sc_create_vcd_trace_file("adder_trace");




  sc_trace(tf, clk, "clk");
  sc_trace(tf, a, "a");
  sc_trace(tf, b, "b");
  sc_trace(tf, sum, "sum");

  sc_start(sc_time(300,SC_NS));
  sc_close_vcd_trace_file(tf);
  return 0;
}
