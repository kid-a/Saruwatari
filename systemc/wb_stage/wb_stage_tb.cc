#include <systemc.h>
#include "wb_stage.h"

/* Testbench */
int sc_main(int, char * argv[]) 
{
	/* Signals */
	sc_clock clk("clk",10,SC_NS);
    //sc_signal ...

    /* Instantiate DUT */
	wb_stage dut("wb_stage");


    // Bind Ports
    //dut.portname(signal);


    // Open Tracefile
    sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
    if(tcf == NULL)
      {
        cout << "An error has occurred while opening trace file" << endl;
		exit(-1);
      }

    tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

	
	// Add signals to Tracefile
    //sc_trace(tcf, dut.signal, "name");


	// Assign test values to signals
	// signal = "...";
	// word = "00000000000000000000000000000000";
    sc_start(10, SC_NS);                
	// signal = "...";
    sc_start(10, SC_NS);                // Run simulation
   	// ....
   		
	
	// Close Tracefile
	sc_close_vcd_trace_file(tcf);
	return 0;
} 
