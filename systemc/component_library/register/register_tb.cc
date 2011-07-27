#include <systemc.h>
#include "register.h"

// Testbench
int sc_main(int, char * argv[]) 
{
	const int SIZE = 32;
	// Declare Signals
	sc_clock clk("clk",10,SC_NS);
	sc_signal<sc_logic > rst;
	sc_signal<sc_logic > we;
	sc_signal<sc_lv<SIZE> > data_in;
	sc_signal<sc_lv<SIZE> > data_out;
	
    // Instantiate DUT
	reg<SIZE> dut("reg");


    // Bind Ports
    dut.clk(clk);
    dut.rst(rst);
    dut.we(we);
    dut.data_in(data_in);
    dut.data_out(data_out);


    // Open Tracefile
    sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
    if(tcf == NULL)
        cout << "An error has occurred while opening trace file" << endl;
    tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

	
	// Add signals to Tracefile
    sc_trace(tcf, dut.clk, "clk");
    sc_trace(tcf, dut.rst, "rst");
    sc_trace(tcf, dut.we, "we");
    sc_trace(tcf, dut.data_in, "data_in");
    sc_trace(tcf, dut.data_out, "data_out");


	// Assign test values to signals
	// signal = "...";
	rst = SC_LOGIC_0;
	we = SC_LOGIC_1;
	data_in = "00000000000000000000000000000111";
    sc_start(10, SC_NS);                // Run simulation
	data_in = "00000000000000000000000000001111";
    sc_start(10, SC_NS);                // Run simulation
	data_in = "00000000000000000000000000000001";
    sc_start(10, SC_NS);                // Run simulation
    we = SC_LOGIC_0;
	data_in = "00000000000000000000000001111111";
    
    sc_start(10, SC_NS);                // Run simulation
    rst = SC_LOGIC_1;
    sc_start(10, SC_NS);                // Run simulation
   	// ....
   		
	
	// Close Tracefile
	sc_close_vcd_trace_file(tcf);
	return 0;
} 
