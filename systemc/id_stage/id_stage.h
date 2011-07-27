/*
 * id_stage.h    
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

#ifndef ID_STAGE
#define ID_STAGE

#include <systemc.h>

#define W_SIZE 32
#define REGFILE_SIZE 32

enum reg_file_state_t
  {
    clean,
    dirty
  };

class id_stage : sc_module
{
 public:
  /* Clock and Reset */
  sc_in<bool> clk;
  sc_in<sc_logic> rst;

  /* Inputs from IF/ID latch */
  sc_in<sc_lv<W_SIZE> > npc_in;
  sc_in<sc_lv<W_SIZE> > ir_1_in;
  sc_in<sc_lv<W_SIZE> > ir_0_in;
  /* Inputs from wb stage */
  sc_in<sc_logic> reg_file_write_enable_1;
  sc_in<sc_lv<5> > reg_file_write_address_1;
  sc_in<sc_lv<W_SIZE> > reg_file_write_data_1;

  sc_in<sc_logic> reg_file_write_enable_0;
  sc_in<sc_lv<5> > reg_file_write_address_0;
  sc_in<sc_lv<W_SIZE> > reg_file_write_data_0;

  /* Inputs from Control Unit */
  sc_in<sc_logic> branch_taken;
  sc_in<sc_logic> jump;
  sc_in<sc_logic> jal;

  /* Outputs to IF stage */
  sc_out<sc_lv<W_SIZE> > address_after_jump_or_branch;
  sc_out<sc_logic > jump_or_branch;
  /* Datapath outputs */
  sc_out<sc_lv<W_SIZE> > npc_out;
  sc_out<sc_lv<W_SIZE> > ir_1_out;
  sc_out<sc_lv<W_SIZE> > ir_0_out;

  sc_out<sc_lv<W_SIZE> > immediate_1;
  sc_out<sc_lv<W_SIZE> > immediate_0;

  sc_inout<sc_lv<W_SIZE> > A1;
  sc_inout<sc_lv<W_SIZE> > B1;
  sc_inout<sc_lv<W_SIZE> > A0;
  sc_inout<sc_lv<W_SIZE> > B0;
  
  sc_signal<reg_file_state_t> reg_file_pr_state;
  sc_signal<reg_file_state_t> reg_file_nx_state;

  sc_signal<sc_lv<W_SIZE> > A1_internal;  
  sc_signal<sc_lv<W_SIZE> > B1_internal;  
  sc_signal<sc_lv<W_SIZE> > A0_internal;
  sc_signal<sc_lv<W_SIZE> > B0_internal;
  /* Processes */
  void process();

 id_stage(sc_module_name _name, sc_trace_file* tcf, bool trace_enabled) : sc_module(_name), _tcf(tcf)
  {
    SC_METHOD(process);
    sensitive << rst << clk
	      << npc_in << ir_1_in << ir_0_in
	      << branch_taken << jump << jal
	      << reg_file_write_enable_1 
	      << reg_file_write_enable_0 ;
      
    /* Instantiate Register File vector */
    _registers = std::vector<int>(REGFILE_SIZE);

    /* Initialize Register File vector */
    for (int i = 0; i < REGFILE_SIZE; i ++) 
      {
        _registers[i] = 0;
      }
      
    if (trace_enabled)
      {
        sc_trace(_tcf,jump_or_branch,"007_2a_ID_J_or_B"); 
        sc_trace(_tcf,address_after_jump_or_branch,"011_2a_ID_addr_J_or_B");  
	//        sc_trace(_tcf,branch_operand_is_zero,"015_2a_ID_branc_operand_is_zero");  


	sc_trace(_tcf,A1_internal, "REGFILE_A1_internal");
	sc_trace(_tcf, A1, "REGFILE_A1");
        sc_trace(_tcf,_registers[0], "REGFILE_0"); 
        sc_trace(_tcf,_registers[1], "REGFILE_1"); 
        sc_trace(_tcf,_registers[2], "REGFILE_2"); 
        sc_trace(_tcf,_registers[3], "REGFILE_3"); 
        sc_trace(_tcf,_registers[4], "REGFILE_4"); 
        sc_trace(_tcf,_registers[5], "REGFILE_5"); 
        sc_trace(_tcf,_registers[6], "REGFILE_6"); 
        sc_trace(_tcf,_registers[7], "REGFILE_7"); 
        sc_trace(_tcf,_registers[8], "REGFILE_8"); 
        sc_trace(_tcf,_registers[9], "REGFILE_9"); 
        sc_trace(_tcf,_registers[10],"REGFILE_10"); 
        sc_trace(_tcf,_registers[31],"REGFILE_31"); 
        sc_trace(_tcf,reg_file_pr_state,"REGFILE_STATE"); 

      }
  }
  SC_HAS_PROCESS(id_stage);	

 private:
  sc_trace_file *_tcf;

  /* Register File */
  std::vector<int> _registers;
};


void id_stage::process()
{
  int branch_destination_address = 0;

  if (rst.read() == SC_LOGIC_1)
    {
      A1.write(0);
      B1.write(0);
      A0.write(0);
      B0.write(0);
      npc_out.write(0);
      npc_out.write(0);
      ir_1_out.write(0);
      ir_0_out.write(0);
      immediate_1.write(0);
      immediate_0.write(0);
      address_after_jump_or_branch.write(0);
      jump_or_branch.write(SC_LOGIC_0);

      for (int i = 0; i < REGFILE_SIZE; i ++) 
	{
	  _registers[i] = 0;
	}
    }

  else
    {
      if ( clk.read() == false )
	{
	  if ( reg_file_write_enable_1.read() == SC_LOGIC_1 &&
	       reg_file_write_address_1.read().to_uint() != 0 )
	    {
	      _registers[ reg_file_write_address_1.read().to_uint() ] =
		reg_file_write_data_1.read().to_int();
	      
/* 	      std::cout << sc_time_stamp() << "(ID stage) Writing: " <<  */
/* 		reg_file_write_data_1.read().to_int()  */
/* 			<< " into register: " << */
/* 		reg_file_write_address_1.read().to_uint()  */
/* 			<< endl; */
	    }
      
	  if ( reg_file_write_enable_0.read() == SC_LOGIC_1 &&
	       reg_file_write_address_0.read().to_uint() != 0 )
     
	    {
	      _registers[ reg_file_write_address_0.read().to_uint() ] =
		reg_file_write_data_0.read().to_int();
	  
/* 	      std::cout << sc_time_stamp() << "(ID stage) Writing: " <<  */
/* 		reg_file_write_data_0.read().to_int()  */
/* 			<< " into register: " << */
/* 		reg_file_write_address_0.read().to_uint()  */
/* 			<< endl; */
	    }
	}

      npc_out.write(npc_in.read());
      ir_1_out.write(ir_1_in.read());
      ir_0_out.write(ir_0_in.read());

      immediate_1.write( ir_1_in.read().range(15, 0) );
      immediate_0.write( ir_0_in.read().range(15, 0) );
      
      /* branch target calculation */      
      branch_destination_address = 
	npc_in.read().to_int() + 
	ir_1_in.read().range(15, 0).to_int();


      cout << "                                  BDA: " << branch_destination_address << endl;

      if ( branch_taken.read() == SC_LOGIC_1 )
	{
	  address_after_jump_or_branch.write( branch_destination_address );
	} 
      else
	{
	  address_after_jump_or_branch.write(_registers[ ir_1_in.read().range(25, 21).to_int() ] );
	}
	  
      /* Jump or Branch */
      if ( branch_taken.read() == SC_LOGIC_1 ||
	   jump.read() == SC_LOGIC_1 ||
	   jal.read() == SC_LOGIC_1)
	{
	  jump_or_branch.write(SC_LOGIC_1);
	}
      else
	{
	  jump_or_branch.write(SC_LOGIC_0);
	}

      A0.write( _registers[ ir_0_in.read().range(25, 21).to_int() ] );
      B0.write( _registers[ ir_0_in.read().range(20, 16).to_int() ] );
      A1.write( _registers[ ir_1_in.read().range(25, 21).to_int() ] );
      B1.write( _registers[ ir_1_in.read().range(20, 16).to_int() ] );
    } // End if 
} // End process
#endif
