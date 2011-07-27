
#include <systemc.h>
#include "zero_comparator.h"

int sc_main(int, char * argv[]) 
{

  sc_signal<sc_lv<32> > input;
  sc_signal<sc_logic > output;

  zero_comparator dut("zero_comparator");

  dut.value_in(input);
  dut.is_zero(output);

  sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
  if(tcf == NULL)
    {
      cout << "An error has occurred while opening trace file" << endl;
      exit(-1);
    }

  tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

  sc_trace(tcf, dut.value_in, "value_in");
  sc_trace(tcf, dut.is_zero, "is_zero");

  input.write(0b00000000000000000000000000000000);
  sc_start(10, SC_NS);

  input.write(0b00000000000000100000000000000000);
  sc_start(10, SC_NS);

  input.write(0b11000000000000010000000000000000);
  sc_start(10, SC_NS);

  input.write(0b11000000000000000000000000000000);
  sc_start(10, SC_NS);
	
  sc_close_vcd_trace_file(tcf);
  return 0;
} 
