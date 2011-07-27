#include<systemc.h>
#include "exmem_stage.h"

int sc_main(int, char * argv [])
{

  // signal declaration
  sc_clock clk("clk", 10, SC_NS);
  sc_signal<sc_logic> rst;

  sc_signal<sc_lv<32> > npc_in;
  sc_signal<sc_lv<32> > ir_1_in;
  sc_signal<sc_lv<32> > ir_0_in;
  sc_signal<sc_lv<32> > A0;
  sc_signal<sc_lv<32> > B0;
  sc_signal<sc_lv<32> > A1;
  sc_signal<sc_lv<32> > B1;
  sc_signal<sc_lv<32> > immediate_1;
  sc_signal<sc_lv<32> > immediate_0;
  sc_signal<sc_logic> alu_src_1;
  sc_signal<sc_logic> alu_src_0;
  sc_signal<sc_logic> reg_dst_1;
  sc_signal<sc_logic> reg_dst_0;
  sc_signal<sc_logic> jal_in;
  sc_signal<sc_logic> mem_read_in;
  sc_signal<sc_logic> mem_write_in;
  sc_signal<sc_logic> wb_enable_1_in;
  sc_signal<sc_logic> wb_enable_0_in;

  sc_signal<sc_logic> alu_1_data_ready;
  sc_signal<sc_logic> alu_0_data_ready;

  sc_signal<sc_lv<32> > npc_out;
  sc_signal<sc_lv<32> > B0_out;
  sc_signal<sc_lv<5> > dst_reg_1;
  sc_signal<sc_lv<5> > dst_reg_0;
  sc_signal<sc_lv<32> > alu_out_1;
  sc_signal<sc_lv<32> > alu_out_0;
  sc_signal<sc_lv<32> > lmd;
  sc_signal<sc_logic> jal_out;
  sc_signal<sc_logic> mem_read_out;
  sc_signal<sc_logic> mem_write_out;
  sc_signal<sc_logic> wb_enable_1_out;
  sc_signal<sc_logic> wb_enable_0_out;

  // device under test instantiation
  exmem_stage dut("execute/memory_stage");

  // port map
  dut.clk(clk);
  dut.rst(rst);
  dut.npc_in(npc_in);
  dut.ir_1_in(ir_1_in);
  dut.ir_0_in(ir_0_in);
  dut.A0(A0);
  dut.B0(B0);
  dut.A1(A1);
  dut.B1(B1);
  dut.immediate_1(immediate_1);
  dut.immediate_0(immediate_0);
  dut.alu_src_1(alu_src_1);
  dut.alu_src_0(alu_src_0);
  dut.reg_dst_1(reg_dst_1);
  dut.reg_dst_0(reg_dst_0);
  dut.jal_in(jal_in);
  dut.mem_read_in(mem_read_in);
  dut.mem_write_in(mem_write_in);
  dut.wb_enable_1_in(wb_enable_1_in);
  dut.wb_enable_0_in(wb_enable_0_in);
  dut.alu_1_data_ready(alu_1_data_ready);
  dut.alu_0_data_ready(alu_0_data_ready);
  dut.npc_out(npc_out);
  dut.B0_out(B0_out);
  dut.dst_reg_1(dst_reg_1);
  dut.dst_reg_0(dst_reg_0);
  dut.alu_out_1(alu_out_1);
  dut.alu_out_0(alu_out_0);
  dut.lmd(lmd);
  dut.jal_out(jal_out);
  dut.mem_read_out(mem_read_out);
  dut.mem_write_out(mem_write_out);
  dut.wb_enable_1_out(wb_enable_1_out);
  dut.wb_enable_0_out(wb_enable_0_out);

//   dut.npc_internal(npc_internal);
//   dut.jal_internal(jal_internal);
//   dut.wb_enable_1_internal(wb_enable_1_internal);
//   dut.wb_enable_0_internal(wb_enable_0_internal);
//   dut.mem_read_internal(mem_read_internal);
//   dut.alu_1_operand_b(alu_1_operand_b);
//   dut.alu_0_operand_b(alu_0_operand_b);
//dut.alu_1_opcode(alu_1_opcode);
//   dut.alu_0_opcode(alu_0_opcode);
//   dut.alu_1_out_internal(alu_1_out_internal);
//   dut.alu_0_out_internal(alu_0_out_internal);


  // tracefile
  sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
  
  if(tcf == NULL)
    {
      cout << "An error has occurred while opening trace file" << endl;
      exit(-1);
    }
  
  tcf->set_time_unit(100 , SC_PS);      // set tracing resolution to ns

  sc_trace(tcf, dut.clk, "clock");
  sc_trace(tcf, dut.rst, "reset");
  sc_trace(tcf, dut.npc_in, "npc_in");
  sc_trace(tcf, dut.ir_1_in, "ir_1_in");
  sc_trace(tcf, dut.ir_0_in, "ir_0_in");
  sc_trace(tcf, dut.A1, "A1");
  sc_trace(tcf, dut.B1, "B1");
  sc_trace(tcf, dut.A0, "A0");
  sc_trace(tcf, dut.B0, "B0");
  sc_trace(tcf, dut.immediate_1, "immediate_1");
  sc_trace(tcf, dut.immediate_0, "immediate_0");
  sc_trace(tcf, dut.npc_out, "npc_out");
  sc_trace(tcf, dut.B0_out, "B0_out");
  sc_trace(tcf, dut.B0_internal, "B0_internal");
  sc_trace(tcf, dut.alu_src_1, "alu_src_1");
  sc_trace(tcf, dut.alu_src_0, "alu_src_0");
  sc_trace(tcf, dut.reg_dst_1, "reg_dst_1");
  sc_trace(tcf, dut.reg_dst_0, "reg_dst_0");
  sc_trace(tcf, dut.jal_in, "jal_in");
  sc_trace(tcf, dut.mem_read_in, "mem_read_in");
  sc_trace(tcf, dut.mem_write_in, "mem_write");
  sc_trace(tcf, dut.wb_enable_1_in, "wb_enable_1_in");
  sc_trace(tcf, dut.wb_enable_0_in, "wb_enable_0_in");
  sc_trace(tcf, dut.alu_1_data_ready, "alu_1_data_ready");
  sc_trace(tcf, dut.alu_0_data_ready, "alu_0_data_ready");
  sc_trace(tcf, dut.dst_reg_1, "dst_reg_1");
  sc_trace(tcf, dut.dst_reg_0, "dst_reg_0");
  sc_trace(tcf, dut.alu_out_1, "alu_out_1");
  sc_trace(tcf, dut.alu_out_0, "alu_out_0");
  sc_trace(tcf, dut.lmd, "lmd");
  sc_trace(tcf, dut.jal_out, "jal_out");
  sc_trace(tcf, dut.mem_read_out, "mem_read_out");
  sc_trace(tcf, dut.mem_write_out, "mem_write_out");
  sc_trace(tcf, dut.wb_enable_1_out, "wb_enable_1_out");
  sc_trace(tcf, dut.wb_enable_0_out, "wb_enable_0_out");

  // tracing internal signals
  sc_trace(tcf, dut.npc_internal, "npc_internal");
  sc_trace(tcf, dut.jal_internal, "jal_internal");
  sc_trace(tcf, dut.wb_enable_1_internal, "wb_enable_1_internal");
  sc_trace(tcf, dut.wb_enable_0_internal, "wb_enable_0_internal");
  sc_trace(tcf, dut.mem_read_internal, "mem_read_internal");
  sc_trace(tcf, dut.alu_1_operand_b, "alu_1_operand_b");
  sc_trace(tcf, dut.alu_0_operand_b, "alu_0_operand_b");
  sc_trace(tcf, dut.alu_1_opcode, "alu_1_opcode");
  sc_trace(tcf, dut.alu_0_opcode, "alu_0_opcode");
  sc_trace(tcf, dut.alu_1_out_internal, "alu_1_out_internal");
  sc_trace(tcf, dut.alu_0_out_internal, "alu_0_out_internal");


  rst = SC_LOGIC_1;
  npc_in = 0;
  ir_1_in = 0b00000000000000000000000000000000; //addi R1,R1,1
  ir_0_in = 0b00000000000000000000000000000000; //add R0,R1,R1
  A0 = 0b00000000000000000000000000000000;
  B0 = 0b00000000000000000000000000000000;
  A1 = 0b00000000000000000000000000000000;
  B1 = 0b00000000000000000000000000000000;
  immediate_1 = 0b00000000000000000000000000000000;
  immediate_0 = 0b00000000000000000000000000000000;
  alu_src_1 = SC_LOGIC_0;
  alu_src_0 = SC_LOGIC_0;
  reg_dst_1 = SC_LOGIC_0;
  reg_dst_0 = SC_LOGIC_0;
  jal_in = SC_LOGIC_0;
  mem_read_in = SC_LOGIC_0;
  mem_write_in = SC_LOGIC_0;
  wb_enable_1_in = SC_LOGIC_0;
  wb_enable_0_in = SC_LOGIC_0;
  sc_start(12, SC_NS);

  rst = SC_LOGIC_0;  
  npc_in = 1;
  ir_1_in = 0b00000100001000100000000000000001; //addi R1,R1,1
  ir_0_in = 0b01000000001000100000000000000001; //add R0,R1,R1
  A0 = 0b00000000000000000000000000000101;
  B0 = 0b00000000000000000000000000000101;
  A1 = 0b00000000000000000000000000000101;
  B1 = 0b00000000000000000000000000000101;
  immediate_1 = 0b00000000000000000000000000000001;
  immediate_0 = 0b00000000000000000000000000000001;
  alu_src_1 = SC_LOGIC_0;
  alu_src_0 = SC_LOGIC_1;
  reg_dst_1 = SC_LOGIC_0;
  reg_dst_0 = SC_LOGIC_1;
  jal_in = SC_LOGIC_0;
  mem_read_in = SC_LOGIC_0;
  mem_write_in = SC_LOGIC_0;
  wb_enable_1_in = SC_LOGIC_1;
  wb_enable_0_in = SC_LOGIC_1;
  sc_start(3, SC_NS);

  sc_start(2, SC_NS);
  sc_start(10, SC_NS);

  rst = SC_LOGIC_0;  
  npc_in = 1;
  ir_1_in = 0b00011100001000100000000000000001; //multi R1,R1,1
  ir_0_in = 0b01000000001000100000000000000001; //add R0,R1,R1
  A0 = 0b00000000000000000000000000000101;
  B0 = 0b00000000000000000000000000000101;
  A1 = 0b00000000000000000000000000000101;
  B1 = 0b00000000000000000000000000000101;
  immediate_1 = 0b00000000000000000000000000000001;
  immediate_0 = 0b00000000000000000000000000000001;
  alu_src_1 = SC_LOGIC_0;
  alu_src_0 = SC_LOGIC_1;
  reg_dst_1 = SC_LOGIC_0;
  reg_dst_0 = SC_LOGIC_1;
  jal_in = SC_LOGIC_0;
  mem_read_in = SC_LOGIC_0;
  mem_write_in = SC_LOGIC_0;
  wb_enable_1_in = SC_LOGIC_1;
  wb_enable_0_in = SC_LOGIC_1;
  sc_start(12, SC_NS);
  sc_start(12, SC_NS);  



  return 0;
}
