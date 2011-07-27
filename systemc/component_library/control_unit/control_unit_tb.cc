#include <systemc.h>
#include "control_unit.h"

int sc_main(int, char * argv[]) 
{
  sc_signal<sc_lv<6> > opcode_1;
  sc_signal<sc_lv<6> > opcode_0;
  sc_signal<sc_logic> branch_operand_is_zero;

  sc_signal<sc_logic> alu_src_1;
  sc_signal<sc_logic> alu_src_0;
  sc_signal<sc_logic> reg_dst_1;
  sc_signal<sc_logic> reg_dst_0;
  sc_signal<sc_logic> jump;
  sc_signal<sc_logic> jal;
  sc_signal<sc_logic> branch_taken;
  sc_signal<sc_logic> mem_read;
  sc_signal<sc_logic> mem_write;
  sc_signal<sc_logic> wb_enable_1;
  sc_signal<sc_logic> wb_enable_0;

  control_unit dut("control_unit");
  
  dut.opcode_1(opcode_1);
  dut.opcode_0(opcode_0);
  dut.branch_operand_is_zero(branch_operand_is_zero);
  dut.alu_src_1(alu_src_1);
  dut.alu_src_0(alu_src_0);
  dut.reg_dst_1(reg_dst_1);
  dut.reg_dst_0(reg_dst_0);
  dut.jump(jump);
  dut.jal(jal);
  dut.branch_taken(branch_taken);
  dut.mem_read(mem_read);
  dut.mem_write(mem_write);
  dut.wb_enable_1(wb_enable_1);
  dut.wb_enable_0(wb_enable_0);

  sc_trace_file * tcf = sc_create_vcd_trace_file("wave");
  if(tcf == NULL)
    {
      cout << "An error has occurred while opening trace file" << endl;
      exit(-1);
      }
  
  tcf->set_time_unit(100, SC_PS);      // set tracing resolution to ns
  
  sc_trace(tcf, dut.opcode_1, "opcode_1");
  sc_trace(tcf, dut.opcode_0, "opcode_0");
  sc_trace(tcf, dut.branch_operand_is_zero, 
	   "branch_operand_is_zero");
  sc_trace(tcf, dut.alu_src_1, "alu_src_1");
  sc_trace(tcf, dut.alu_src_0, "alu_src_0");
  sc_trace(tcf, dut.reg_dst_1, "reg_dst_1");
  sc_trace(tcf, dut.reg_dst_0, "reg_dst_0");
  sc_trace(tcf, dut.jump, "jump");
  sc_trace(tcf, dut.jal, "jal");
  sc_trace(tcf, dut.branch_taken, "branch_taken");
  sc_trace(tcf, dut.mem_read, "mem_read");
  sc_trace(tcf, dut.mem_write, "mem_write");
  sc_trace(tcf, dut.wb_enable_1, "wb_enable_1");
  sc_trace(tcf, dut.wb_enable_0, "wb_enable_0");

  opcode_1.write(0);
  opcode_0.write(0);
  branch_operand_is_zero.write(SC_LOGIC_0);
  sc_start(10, SC_NS);                
  opcode_1.write(0b000101);
  opcode_0.write(0b000011);
  branch_operand_is_zero.write(SC_LOGIC_1);
  sc_start(10, SC_NS);                
  opcode_1.write(0b000001);
  opcode_0.write(0b000100);
  branch_operand_is_zero.write(SC_LOGIC_0);
  sc_start(10, SC_NS);                
  opcode_1.write(0b000001);
  opcode_0.write(0b000100);
  branch_operand_is_zero.write(SC_LOGIC_0);
  sc_start(10, SC_NS);                
  opcode_1.write(0b000000);
  opcode_0.write(0b000000);
  branch_operand_is_zero.write(SC_LOGIC_0);
  sc_start(10, SC_NS);                
  sc_start(10, SC_NS);
   		
	
	// Close Tracefile
	sc_close_vcd_trace_file(tcf);
	return 0;
} 
