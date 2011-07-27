#include <systemc.h>
#include "ram.h"

// Testbench
int sc_main(int, char * argv[]) 
{
  const int WORDS = 256;
  const int WORD_SIZE = 32;
  // Declare Signals
  //sc_signal ...
  sc_clock clk("clk",10,SC_NS);
  sc_signal<sc_logic > write_enable;
  sc_signal<sc_lv<WORD_SIZE> > addr;
  sc_signal<sc_lv<WORD_SIZE> > data_in;
  sc_signal<sc_lv<WORD_SIZE> > data_out;


  // Instantiate DUT
  ram<WORDS, WORD_SIZE> dut("ram");


  // Bind Ports
  dut.clk(clk);
  dut.write_enable(write_enable);
  dut.addr(addr);
  dut.data_in(data_in);
  dut.data_out(data_out);


  // Open Tracefile
  sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
  if(tcf == NULL)
    cout << "An error has occurred while opening trace file" << endl;
  tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

	
  // Add signals to Tracefile
  sc_trace(tcf, dut.clk, "clk");
  sc_trace(tcf, dut.write_enable, "write_enable");
  sc_trace(tcf, dut.addr, "addr");
  sc_trace(tcf, dut.data_in, "data_in");
  sc_trace(tcf, dut.data_out, "data_out");


  // Assign test values to signals
  write_enable = SC_LOGIC_1;
  addr = 		"00000000000000000000000000000000";
  data_in = 	"00000000000000000000000000000001";
  sc_start(10, SC_NS);                // Run simulation
  addr = 		"00000000000000000000000000001000";
  data_in = 	"00000000000000000000000000000010";
    
  sc_start(10, SC_NS);                // Run simulation
  addr = 		"00000000000000000000000000000000";
  // ....
  sc_start(10, SC_NS);                // Run simulation
  addr = 		"00000000000000000000000000001000";
  // ....
   			
	
  // Close Tracefile
  sc_close_vcd_trace_file(tcf);
  return 0;
} 
