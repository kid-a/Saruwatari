//------------------------------------------------------------------
// Simple Testbench for dff flipflop file
// 
// SystemC for VHDL engineers
// (c)www.ht-lab.com
//------------------------------------------------------------------

#include <systemc.h>
#include "adder.h"

int sc_main(int argc, char* argv[])
{
    sc_signal<sc_lv<32> > a,b,sum;      // Local signals

	//sc_clock clk("clk",10,SC_NS,0.5);   // Create a clock signal

    adder<32> DUT("adder");                     // Instantiate Device Under Test

    DUT.operand_A(a);    // Connect ports
    DUT.operand_B(b);
    DUT.sum(sum);

    sc_trace_file *fp;                  // Create VCD file
    fp=sc_create_vcd_trace_file("wave");
    fp->set_time_unit(100, SC_PS);      // set tracing resolution to ns
    sc_trace(fp,a,"operand_A");             // Add signals to trace file
    sc_trace(fp,b,"operand_B");
    sc_trace(fp,sum,"sum");
    
    a="00000000000000000000000000000001";
    b="00000000000000000000000000000001";
    sc_start(5, SC_NS);                // Run simulation
    a="00000000000000000000000000000010";
    b="00000000000000000000000000000010";
    sc_start(5, SC_NS);                // Run simulation
    a="00000000000000000000000000000010";
    b="00000000000000000000000000000100";
    sc_start(5, SC_NS);                // Run simulation
    a="00000000000000000000000000000110";
    b="00000000000000000000000000001000";
    sc_start(5, SC_NS);                // Run simulation
 

    sc_close_vcd_trace_file(fp);        // close(fp)

    return 0;                           // Return no errors
}
