#include <systemc.h>
#include "rom.h"

int sc_main(int, char * argv[]) 
{
	const int WORDS = 256, WORD_SIZE = 32;
    
    sc_signal<sc_lv<WORD_SIZE> > addr;
	sc_signal<bool> ready;
    sc_signal<sc_lv<WORD_SIZE*2> > data;

    // DUT
    rom<WORDS, WORD_SIZE> dut("rom");

    // Port Binding
    dut.addr(addr);
    dut.ready(ready);
    dut.data(data);

    // Tracefile
    sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
    if(tcf == NULL)
        cout << "An error has occurred while opening trace file" << endl;
    tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

    sc_trace(tcf, dut.addr, "addr");
    sc_trace(tcf, dut.data, "data");
    sc_trace(tcf, dut.ready, "ready");

    dut.load_memory_from_file("mem_dump.dat");


    addr="00000000000000000000000000000000";
	ready = 1;
    sc_start(5, SC_NS);                // Run simulation
    addr="00000000000000000000000000001000";
    sc_start(5, SC_NS);                // Run simulation
    addr="00000000000000000000000000000000";
    sc_start(5, SC_NS);                // Run simulation
    addr="00000000000000000000000000001000";
    sc_start(5, SC_NS);                // Run simulation
 
   sc_close_vcd_trace_file(tcf);
	return 0;
}
