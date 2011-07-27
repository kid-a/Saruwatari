#include <systemc.h>
#include "alu.h"

int sc_main(int, char * argv[]) 
{
  sc_clock clk("clk", 10, SC_NS);
  sc_signal<sc_logic> rst;

  sc_signal<sc_lv<32> > a;
  sc_signal<sc_lv<32> > b;
  sc_signal<sc_lv<4> > op;

  sc_signal<sc_logic> data_ready;
  sc_signal<sc_lv<32> > result;

  alu dut("alu");

  dut.clk(clk);
  dut.rst(rst);
  dut.a(a);
  dut.b(b);
  dut.op(op);
  dut.data_ready(data_ready);
  dut.result(result);

  sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
  if(tcf == NULL)
    {
      cout << "An error has occurred while opening trace file" << endl;
      exit(-1);
    }

  tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

  sc_trace(tcf, dut.clk, "clk");
  sc_trace(tcf, dut.rst, "rst");
  sc_trace(tcf, dut.a, "a");
  sc_trace(tcf, dut.b, "b");
  sc_trace(tcf, dut.op, "op");
  sc_trace(tcf, dut.data_ready, "data_ready");
  sc_trace(tcf, dut.result, "result");

  a.write(10);
  b.write(20);
  op.write(1);
  rst.write(SC_LOGIC_1);
  sc_start(13, SC_NS);
  rst.write(SC_LOGIC_0);
  sc_start(7, SC_NS);
  a.write(10);
  b.write(20);
  op.write(3);
  sc_start(40, SC_NS);
  a.write(10);
  b.write(20);
  op.write(2);
  sc_start(10, SC_NS);
  a.write(10);
  b.write(20);
  op.write(1);
  sc_start(10, SC_NS);

  sc_close_vcd_trace_file(tcf);
  return 0;
}
