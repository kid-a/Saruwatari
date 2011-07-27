#include <systemc.h>
#include "if_stage.h"
#include "if_id_latch.h"



// Testbench
int sc_main(int, char * argv[]) 
{
	
	/* Open Tracefile */
    sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
    if(tcf == NULL)
      {
        cout << "An error has occurred while opening trace file" << endl;
		exit(-1);
      }
	/* Set tracing resolution to ns */
    tcf->set_time_unit(100, SC_PS);      

	/* Signals  */
	sc_clock clok("clk",10,SC_NS);
	sc_signal<sc_logic > reset;

    /* Instantiate DUT */
	if_stage dut("IF Stage",tcf);
	if_id_latch dut("IF/ID Latch");

    /* Bind Ports */
    //dut.portname(signal);


	
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
