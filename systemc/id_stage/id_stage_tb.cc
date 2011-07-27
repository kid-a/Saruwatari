#include <systemc.h>
#include "id_stage.h"

int sc_main(int, char * argv[]) 
{

  // signal declaration
  sc_clock clk("clk", 10, SC_NS);
  sc_signal<sc_logic > rst;

  sc_signal<sc_lv<32> > npc_in;
  sc_signal<sc_lv<32> > ir_1_in;
  sc_signal<sc_lv<32> > ir_0_in;

  sc_signal<sc_logic> reg_file_write_enable_1;
  sc_signal<sc_lv<5> > reg_file_write_address_1;
  sc_signal<sc_lv<32> > reg_file_write_data_1;

  sc_signal<sc_logic> reg_file_write_enable_0;
  sc_signal<sc_lv<5> > reg_file_write_address_0;
  sc_signal<sc_lv<32> > reg_file_write_data_0;

  sc_signal<sc_logic> branch_taken;
  sc_signal<sc_logic> jump;
  sc_signal<sc_logic> jal;
  
  sc_signal<sc_lv<32> > address_after_jump_or_branch;
  sc_signal<sc_logic> jump_or_branch;

  sc_signal<sc_lv<32> > npc_out;
  sc_signal<sc_lv<32> > ir_1_out;
  sc_signal<sc_lv<32> > ir_0_out;

  sc_signal<sc_lv<32> > immediate_1;
  sc_signal<sc_lv<32> > immediate_0;

  sc_signal<sc_lv<32> > A1;
  sc_signal<sc_lv<32> > B1;
  sc_signal<sc_lv<32> > A0;
  sc_signal<sc_lv<32> > B0;

  // device under test instantiation
  id_stage dut("instruction_decode_stage");

  // port map
  dut.clk(clk);
  dut.rst(rst);
  dut.npc_in(npc_in);
  dut.ir_1_in(ir_1_in);
  dut.ir_0_in(ir_0_in);
  dut.reg_file_write_enable_1(reg_file_write_enable_1);
  dut.reg_file_write_address_1(reg_file_write_address_1);
  dut.reg_file_write_data_1(reg_file_write_data_1);
  dut.reg_file_write_enable_0(reg_file_write_enable_0);
  dut.reg_file_write_address_0(reg_file_write_address_0);
  dut.reg_file_write_data_0(reg_file_write_data_0);
  dut.address_after_jump_or_branch(address_after_jump_or_branch);
  dut.jump_or_branch(jump_or_branch);
  dut.npc_out(npc_out);
  dut.ir_1_out(ir_1_out);
  dut.ir_0_out(ir_0_out);
  dut.immediate_1(immediate_1);
  dut.immediate_0(immediate_0);
  dut.A1(A1);
  dut.B1(B1);
  dut.A0(A0);
  dut.B0(B0);
//   dut.if_id_latch_reset(if_id_latch_reset);
//   dut.pc_write_enable(pc_write_enable);
//   dut.alu_src_1(alu_src_1);
//   dut.alu_src_0(alu_src_0);
//   dut.reg_dst_1(reg_dst_1);
//   dut.reg_dst_0(reg_dst_0);
//   dut.jal(jal);
//   dut.mem_read(mem_read);
//   dut.mem_write(mem_write);
//   dut.wb_enable_1(wb_enable_1);
//   dut.wb_enable_0(wb_enable_0);
  
  // tracefile
  sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
  
  if(tcf == NULL)
    {
      cout << "An error has occurred while opening trace file" << endl;
      exit(-1);
    }

  tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns

  sc_trace(tcf, dut.clk, "clock");
  sc_trace(tcf, dut.rst, "reset");
  sc_trace(tcf, dut.npc_in, "npc_in");
  sc_trace(tcf, dut.ir_1_in, "ir_1_in");
  sc_trace(tcf, dut.ir_0_in, "ir_0_in");

  sc_trace(tcf, dut.npc_out, "npc_out");
  sc_trace(tcf, dut.ir_1_out, "ir_1_out");
  sc_trace(tcf, dut.ir_0_out, "ir_0_out");
  sc_trace(tcf, dut.A1, "A1");
  sc_trace(tcf, dut.B1, "B1");
  sc_trace(tcf, dut.A0, "A0");
  sc_trace(tcf, dut.B0, "B0");
  sc_trace(tcf, dut.immediate_1, "immediate_1");
  sc_trace(tcf, dut.immediate_0, "immediate_0");
  sc_trace(tcf, dut.address_after_jump_or_branch, 
	   "address_after_jump_or_branch");

  sc_trace(tcf, reg_file_write_enable_1, "reg_file_write_enable_1");
  sc_trace(tcf, reg_file_write_address_1, "reg_file_write_address_1");
  sc_trace(tcf, reg_file_write_data_1, "reg_file_write_data_1");

  sc_trace(tcf, reg_file_write_enable_0, "reg_file_write_enable_0");
  sc_trace(tcf, reg_file_write_address_0, "reg_file_write_address_0");
  sc_trace(tcf, reg_file_write_data_0, "reg_file_write_data_0");
  
  sc_trace(tcf, dut.jump_or_branch, "jump_or_branch");
  
  // assign test values to signals
  

  branch_taken.write(SC_LOGIC_0);
  jal.write(SC_LOGIC_0);
  jump.write(SC_LOGIC_0);
  rst.write(SC_LOGIC_1);
  sc_start(4, SC_NS);
  rst.write(SC_LOGIC_0);
  npc_in.write(0b00000000000000000000000000000000);
  ir_1_in.write(0b00000100000000000000000000000001);
  ir_0_in.write(0b00000100000000000000000000000010);
  sc_start(10, SC_NS);                
  npc_in.write(0b00000000000000000000000000000001);
  ir_1_in.write(0b00000000000000000000000000001010);
  ir_0_in.write(0b00000000000000000000000000000101);
  sc_start(10, SC_NS);         
  reg_file_write_enable_1.write(SC_LOGIC_1);
  reg_file_write_address_1.write(0);
  reg_file_write_data_1.write(10);
  reg_file_write_enable_0.write(SC_LOGIC_1);
  reg_file_write_address_0.write(4);
  reg_file_write_data_0.write(11);
  npc_in.write(0b00000000000000000000000000000010);
  ir_1_in.write(0b00000000000000000000000000000010);
  ir_0_in.write(0b01000000000000000000000000000100);
  sc_start(10, SC_NS);         
  npc_in.write(0b00000000000001000000000000000011);
  ir_1_in.write(0b00000000000000100000000000000011);
  ir_0_in.write(0b01000000000001000000000000000100);
  npc_in.write(0b00000000000001000000000000100011);
  ir_1_in.write(0b00000000000000100000000010000011);
  ir_0_in.write(0b01000000000001000000000000100100);
  sc_start(10, SC_NS);         
    
  // close tracefile
  sc_close_vcd_trace_file(tcf);
  return 0;
} 
