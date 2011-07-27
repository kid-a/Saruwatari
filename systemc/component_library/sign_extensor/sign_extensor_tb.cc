#include <systemc.h>
#include "sign_extensor.h"

int sc_main(int, char * argv[]) 
{

  sc_signal<sc_lv<16> > input;
  sc_signal<sc_lv<32> > output;

  sign_extensor dut("sign_extensor");

  dut.data_in(input);
  dut.data_out(output);

  sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
  if(tcf == NULL)
    {
      cout << "An error has occurred while opening trace file" << endl;
      exit(-1);
    }

  tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

  sc_trace(tcf, dut.data_in, "data_in");
  sc_trace(tcf, dut.data_out, "data_out");

  input.write(0b0000000000000000);
  sc_start(10, SC_NS);

  input.write(0b0000000000000010);
  sc_start(10, SC_NS);

  input.write(0b1100000000000001);
  sc_start(10, SC_NS);

  input.write(0b1100000000000000);
  sc_start(10, SC_NS);
	
  sc_close_vcd_trace_file(tcf);
  return 0;
} 
