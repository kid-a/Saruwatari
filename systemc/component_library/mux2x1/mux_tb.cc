#include <systemc.h>
#include "mux.h"

// Testbench
int sc_main(int, char * argv[]) 
{
	const int SIZE = 32;
	// Declare Signals
		sc_signal<sc_lv<SIZE> > a;
		sc_signal<sc_lv<SIZE> > b;
		sc_signal<sc_logic > selector;
		sc_signal<sc_lv<SIZE> > output;


    // Instantiate DUT
	mux<SIZE> dut("mux");


    // Bind Ports
    dut.a(a);
    dut.b(b);
    dut.selector(selector);
    dut.output(output);


    // Open Tracefile
    sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
    if(tcf == NULL)
      {
        cout << "An error has occurred while opening trace file" << endl;
	exit(-1);
      }

    tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

	
	// Add signals to Tracefile
    sc_trace(tcf, dut.a, "a");
    sc_trace(tcf, dut.b, "b");
    sc_trace(tcf, dut.selector, "selector");
    sc_trace(tcf, dut.output, "output");


	// Assign test values to signals
	// signal = "...";
	// word = "00000000000000000000000000000000"
	 a = "00000000000000000000000000000001";
	 b = "00000000000000000000000000000010";
	 selector = SC_LOGIC_0;
    sc_start(10, SC_NS);                // Run simulation
	 a = "00000000000000000000000000000001";
	 b = "00000000000000000000000000000010";
	 selector = SC_LOGIC_1;
    sc_start(10, SC_NS);                // Run simulation
   	 a = "00000000000000000000000000000001";
	 b = "00000000000000000000000000000010";
	 selector = SC_LOGIC_0;
    sc_start(10, SC_NS);                // Run simulation
    sc_start(10, SC_NS);                // Run simulation
   		
	
	// Close Tracefile
	sc_close_vcd_trace_file(tcf);
	return 0;
} 
