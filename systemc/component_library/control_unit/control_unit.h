/*
 * control_unit.h
 * --------------------------------------------------------------------
 *
 * This file is part of SARUWATARI.
 * Copyright (C) 2010 Loris Fichera (loris.fichera@gmail.com)
 *                    Daniele Marletta (danielemar86@gmail.com) 
 *
 * SARUWATARI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SARUWATARI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SARUWATARI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _CONTROL_UNIT
#define _CONTROL_UNIT

#include <systemc.h>

class control_unit : sc_module
{
 public:
  /*   sc_in<bool> clk; */
  /*   sc_in<sc_logic> rst; */
  sc_in<sc_lv<32> > ir_1;
  sc_in<sc_lv<32> > ir_0;
  sc_in<sc_lv<32> > branch_operand;

  sc_out<sc_logic> alu_src_1;
  sc_out<sc_logic> alu_src_0;
  sc_out<sc_logic> reg_dst_1;
  sc_out<sc_logic> reg_dst_0;
  sc_out<sc_logic> jump;
  sc_out<sc_logic> jal;
  sc_out<sc_logic> branch_taken;
  sc_out<sc_logic> mem_read;
  sc_out<sc_logic> mem_write;
  sc_out<sc_logic> wb_enable_1;
  sc_out<sc_logic> wb_enable_0;

  sc_signal<sc_lv<6> > opcode_1;
  sc_signal<sc_lv<6> > opcode_0;

  void opcode_process();
  void process_1();
  void process_0();

  SC_CTOR(control_unit)
  {
    SC_METHOD(opcode_process);
    sensitive << ir_1 << ir_0;
    dont_initialize();
    
    SC_METHOD(process_1);
    sensitive << opcode_1 << branch_operand;
    dont_initialize();

    SC_METHOD(process_0);
    sensitive << opcode_0;
    dont_initialize();

  }
};

void control_unit::opcode_process()
{
  opcode_1.write(ir_1.read().range(31, 26));
  opcode_0.write(ir_0.read().range(31, 26));
}

void control_unit::process_1()
{

  std::cout<<"CU opcode_1 is: "<<opcode_1.read().range(5,4).to_int()<<std::endl;  
  switch(opcode_1.read().range(5, 4).to_uint())
    {
    case 0b00: // I-type
      if ( opcode_1.read().range(3, 0) == 0b0000 ) // nop
	{       
	  alu_src_1.write(SC_LOGIC_0);
	  reg_dst_1.write(SC_LOGIC_0);
	  jump.write(SC_LOGIC_0);
	  jal.write(SC_LOGIC_0);
	  branch_taken.write(SC_LOGIC_0);
	  wb_enable_1.write(SC_LOGIC_0);
	}       
      else if ( opcode_1.read().range(3, 0) == 0b0101 ) //beqz
	{       
	  if ( branch_operand.read().to_uint() == 0 )
	    {
	      alu_src_1.write(SC_LOGIC_0);
	      reg_dst_1.write(SC_LOGIC_0);
	      jump.write(SC_LOGIC_0);
	      jal.write(SC_LOGIC_0);
	      branch_taken.write(SC_LOGIC_1);
	      wb_enable_1.write(SC_LOGIC_0);
	    }
	  else 
	    {
	      alu_src_1.write(SC_LOGIC_0);
	      reg_dst_1.write(SC_LOGIC_0);
	      jump.write(SC_LOGIC_0);
	      jal.write(SC_LOGIC_0);
	      branch_taken.write(SC_LOGIC_0);
	      wb_enable_1.write(SC_LOGIC_0);
	    }
	}       
      else if ( opcode_1.read().range(3, 0) == 0b0110 ) //bqnz
	{       
	  if ( branch_operand.read().to_uint() != 0 )
	    {
	      alu_src_1.write(SC_LOGIC_0);
	      reg_dst_1.write(SC_LOGIC_0);
	      jump.write(SC_LOGIC_0);
	      jal.write(SC_LOGIC_0);
	      branch_taken.write(SC_LOGIC_1);
	      wb_enable_1.write(SC_LOGIC_0);
	    }
	  else 
	    {
	      alu_src_1.write(SC_LOGIC_0);
	      reg_dst_1.write(SC_LOGIC_0);
	      jump.write(SC_LOGIC_0);
	      jal.write(SC_LOGIC_0);
	      branch_taken.write(SC_LOGIC_0);
	      wb_enable_1.write(SC_LOGIC_0);
	    }
	}       
      else //other I-type instructions
	{       
	  alu_src_1.write(SC_LOGIC_0);
	  reg_dst_1.write(SC_LOGIC_0);
	  jump.write(SC_LOGIC_0);
	  jal.write(SC_LOGIC_0);
	  branch_taken.write(SC_LOGIC_0);
	  wb_enable_1.write(SC_LOGIC_1);
	}       
      break;

    case 0b01: // R-type
      alu_src_1.write(SC_LOGIC_1);
      reg_dst_1.write(SC_LOGIC_1);
      jump.write(SC_LOGIC_0);
      jal.write(SC_LOGIC_0);
      branch_taken.write(SC_LOGIC_0);
      wb_enable_1.write(SC_LOGIC_1);
      break;

    case 0b10: //J-type
      //std::cout<<"JUmp!!!"<<std::endl;
      if ( opcode_1.read().range(3, 0) == 0b0000 ) //jump
	{       
	  alu_src_1.write(SC_LOGIC_0);
	  reg_dst_1.write(SC_LOGIC_0);
	  jump.write(SC_LOGIC_1);
	  jal.write(SC_LOGIC_0);
	  branch_taken.write(SC_LOGIC_0);
	  wb_enable_1.write(SC_LOGIC_0);
	}       
      else if ( opcode_1.read().range(3, 0) == 0b0001 ) //jal
	{       
	  alu_src_1.write(SC_LOGIC_0);
	  reg_dst_1.write(SC_LOGIC_0);
	  jump.write(SC_LOGIC_0);
	  jal.write(SC_LOGIC_1);
	  branch_taken.write(SC_LOGIC_0);
	  wb_enable_1.write(SC_LOGIC_1);
	}       
      break;

    default:
      std::cout << "CONTROL UNIT: ERROR - "<<opcode_1<<" is an invalid opcode detected in datapath 1."<< std::endl;
      break;
    }       
}

void control_unit::process_0()
{
  switch(opcode_0.read().range(5, 4).to_int())
    {
    case 0b00: // I-type
      if ( opcode_0.read().range(3, 0) == 0b0000 ) // nop
	{
	  alu_src_0.write(SC_LOGIC_0);
	  reg_dst_0.write(SC_LOGIC_0);
	  mem_read.write(SC_LOGIC_0);
	  mem_write.write(SC_LOGIC_0);
	  wb_enable_0.write(SC_LOGIC_0);
	}

      else if  ( opcode_0.read().range(3, 0) == 0b0011 ) // lw
	{
	  alu_src_0.write(SC_LOGIC_0);
	  reg_dst_0.write(SC_LOGIC_0);
	  mem_read.write(SC_LOGIC_1);
	  mem_write.write(SC_LOGIC_0);
	  wb_enable_0.write(SC_LOGIC_1);
	}

      else if  ( opcode_0.read().range(3, 0) == 0b0100 ) // sw
	{
	  alu_src_0.write(SC_LOGIC_0);
	  reg_dst_0.write(SC_LOGIC_0);
	  mem_read.write(SC_LOGIC_0);
	  mem_write.write(SC_LOGIC_1);
	  wb_enable_0.write(SC_LOGIC_0);
	}

      else //other I-type instructions
	{
	  alu_src_0.write(SC_LOGIC_0);
	  reg_dst_0.write(SC_LOGIC_0);
	  mem_read.write(SC_LOGIC_0);
	  mem_write.write(SC_LOGIC_0);
	  wb_enable_0.write(SC_LOGIC_1);
	}
      break;

    case 0b01: // I-type
      alu_src_0.write(SC_LOGIC_1);
      reg_dst_0.write(SC_LOGIC_1);
      mem_read.write(SC_LOGIC_0);
      mem_write.write(SC_LOGIC_0);
      wb_enable_0.write(SC_LOGIC_1);
      break;

    default:
      std::cout << "CONTROL UNIT: ERROR - Invalid opcode detected in datapath 0." 
		<< std::endl;
    }
}
#endif
