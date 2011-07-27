#include <systemc.h>
#include "if_stage.h"

// Testbench
int sc_main(int, char * argv[]) 
{
		// Signals
		sc_clock clk("clk",10,SC_NS,0.5);
		sc_signal<sc_logic > rst;
		sc_signal<sc_logic > pc_we;
		sc_signal<sc_logic > jump_or_branch;
		sc_signal<sc_lv<W_SIZE> > address_after_jump_or_branch;
		sc_signal<sc_lv<W_SIZE> > npc;
		sc_signal<sc_lv<W_SIZE> > ir_1;
		sc_signal<sc_lv<W_SIZE> > ir_0;

		// Open Tracefile
    sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
    if( tcf == NULL) {
        cout << "An error has occurred while opening trace file" << endl;
		exit(-1);
	}
    tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

	// Instantiate DUT
    if_stage dut("if_stage",tcf, true);


    // Bind Ports
    //dut.portname(signal);
	dut.clk(clk);
	dut.rst(rst);
	dut.pc_we(pc_we);
	dut.jump_or_branch(jump_or_branch);
	dut.address_after_jump_or_branch(address_after_jump_or_branch);
	dut.npc(npc);
	dut.instruction_1(ir_1);
	dut.instruction_0(ir_0);

	
	// Add signals to Tracefile
    sc_trace(tcf, dut.clk, "clk");
    sc_trace(tcf, dut.rst, "rst");
    sc_trace(tcf, dut.pc_we, "pc_write_enable");
    sc_trace(tcf, dut.jump_or_branch, "jump_or_branch");
    sc_trace(tcf, dut.address_after_jump_or_branch, "address_after_jump_or_branch");
    sc_trace(tcf, dut.npc, "new_program_counter");
    sc_trace(tcf, dut.instruction_1, "ir_1");
    sc_trace(tcf, dut.instruction_0, "ir_0");

	rst.write(SC_LOGIC_1);
	pc_we.write(SC_LOGIC_1);
	jump_or_branch.write(SC_LOGIC_0);
	address_after_jump_or_branch.write("000000000011111110000000000000"
);
    sc_start(13, SC_NS);                // Run simulation
    rst.write(SC_LOGIC_0);

	// signal = "...";
    sc_start(10, SC_NS);                
    sc_start(10, SC_NS);                
    sc_start(10, SC_NS);                
   	// ....
   		
	
	// Close Tracefile
	sc_close_vcd_trace_file(tcf);
	return 0;
} 
