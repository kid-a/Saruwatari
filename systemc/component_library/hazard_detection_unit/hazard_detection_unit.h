/*
 * hazard_detection_unit.h    
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

#ifndef HDU_H
#define HDU_H

#include <systemc.h>

#define W_SIZE 32

enum hazard_state_t
  {
    idle_hazard,
    structural_hazard,
    control_hazard,
    data_control_hazard,
    data_hazard_0,
    data_hazard_1
  };

class hazard_detection_unit : sc_module
{
 public:
  sc_in<bool> clk;
  sc_in<sc_logic> rst;
    
  sc_in<sc_logic> jump_or_branch_taken;
  sc_in<sc_lv<W_SIZE> > if_id_ir_1;
  sc_in<sc_lv<W_SIZE> > if_id_ir_0;
  sc_in<sc_lv<W_SIZE> > id_exmem_ir_1;
  sc_in<sc_lv<W_SIZE> > id_exmem_ir_0;
  sc_in<sc_logic> alu_1_data_ready;
  sc_in<sc_logic> alu_0_data_ready;
    
  sc_out<sc_logic> pc_write_enable;
  sc_out<sc_logic> if_id_we;
  sc_out<sc_logic> if_id_rst;
  sc_out<sc_logic> id_exmem_we;
  sc_out<sc_logic> id_exmem_rst;
  sc_out<sc_logic> exmem_wb_we;

  sc_signal<hazard_state_t> curr_state;
  sc_signal<hazard_state_t> nx_state;

  void update_state();
  void ns_op_logic();

 hazard_detection_unit(sc_module_name _name, 
		       sc_trace_file* tcf, 
		       bool trace_enabled) : 
  sc_module(_name), _tcf(tcf)
  {
    SC_METHOD(update_state);
    sensitive << clk.pos() << rst;

    SC_METHOD(ns_op_logic);
    sensitive << jump_or_branch_taken
	      << if_id_ir_1 << if_id_ir_0
	      << id_exmem_ir_1 << id_exmem_ir_0
	      << alu_1_data_ready << alu_0_data_ready
	      << rst << curr_state;
    dont_initialize();
            
    /* Trace Signals */
    if (trace_enabled) 
      {
	sc_trace(_tcf, if_id_ir_1,"HDU_if_id_ir_1");
	sc_trace(_tcf, if_id_ir_0,"HDU_if_id_ir_0");
	sc_trace(_tcf, id_exmem_ir_1, "HDU_id_exmem_ir_1");
	sc_trace(_tcf, id_exmem_ir_0, "HDU_id_exmem_ir_0");
	sc_trace(_tcf, jump_or_branch_taken, "HDU_jump_or_branch_taken");
	sc_trace(_tcf, alu_1_data_ready, "HDU_alu_1_data_ready");
	sc_trace(_tcf, alu_0_data_ready, "HDU_alu_0_data_ready");
	sc_trace(_tcf, pc_write_enable, "HDU_pc_write_enable");
	sc_trace(_tcf, id_exmem_rst, "HDU_id_exmem_rst");
	sc_trace(_tcf, id_exmem_we, "HDU_id_exmem_we");
	sc_trace(_tcf, if_id_rst, "HDU_if_id_rst");
	sc_trace(_tcf, if_id_we, "HDU_if_id_we");
	sc_trace(_tcf, exmem_wb_we, "HDU_exmem_wb_we");
      }
  }

  SC_HAS_PROCESS(hazard_detection_unit);

 private:
  sc_trace_file * _tcf;
};

void hazard_detection_unit::update_state()
{
  if (rst == SC_LOGIC_1)
    {
      curr_state.write(idle_hazard);
    }
  else
    {
      curr_state.write(nx_state.read());
    }
}

void hazard_detection_unit::ns_op_logic()
{
  /* Reset */
  if (rst == SC_LOGIC_1)
    {
      pc_write_enable.write(SC_LOGIC_1);
      if_id_rst.write(SC_LOGIC_1); 
      if_id_we.write(SC_LOGIC_1);  
      id_exmem_rst.write(SC_LOGIC_1);
      id_exmem_we.write(SC_LOGIC_1);
      exmem_wb_we.write(SC_LOGIC_1);
      /* state */
      nx_state.write(idle_hazard);
    }
  else 
    {
      switch (curr_state)
        {
	  /* Currently, there are no hazard, and the hdu is "monitoring" the pipeline. */
	case idle_hazard:
	            
	  std::cout << sc_time_stamp() << "HDU: NOTE - IDLE_HAZARD state." << std::endl;

	  /* A nop is entering the EXMEM stage */
	  if ( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000000 &&
	       id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000000)
	    {
	      /* (Re-)runs the pipeline */
	      pc_write_enable.write(SC_LOGIC_1);
	      if_id_we.write(SC_LOGIC_1);
	      id_exmem_we.write(SC_LOGIC_1);
	      exmem_wb_we.write(SC_LOGIC_1);
	      /* Do not activate reset */
	      if_id_rst.write(SC_LOGIC_0);
	      id_exmem_rst.write(SC_LOGIC_0);
	      /* Next state is idle again. */
	      nx_state.write(idle_hazard);
	    }
	  /* Structural Hazard (at least one ALU is busy computing) */
	  else if ( alu_0_data_ready == SC_LOGIC_0 ||
		    alu_1_data_ready == SC_LOGIC_0 )
	    {
	      /* Stalls the pipeline */
	      pc_write_enable.write(SC_LOGIC_0);
	      if_id_we.write(SC_LOGIC_0);
	      id_exmem_we.write(SC_LOGIC_0);
	      exmem_wb_we.write(SC_LOGIC_0);
	      /* Do not activate reset */
	      if_id_rst.write(SC_LOGIC_0);
	      id_exmem_rst.write(SC_LOGIC_0);
	      /* Next state is Structural Hazard */
	      nx_state.write(structural_hazard);
	    }
	  /* Data Hazard  */
	  else
	    {
	      /* Initialize enabling all signals */
	      pc_write_enable.write(SC_LOGIC_1);
	      id_exmem_we.write(SC_LOGIC_1);
	      if_id_we.write(SC_LOGIC_1);
	      exmem_wb_we.write(SC_LOGIC_1);
                              
	      /* IR_0 in EXMEM belongs to {addi, subi, multi, lw}
	       * and
	       * if IR_0 in ID is a I-type
	       */
	      if (( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
		    id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
		    id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
		    id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		  (( if_id_ir_0.read().range(31, 30).to_uint() == 0b00 ) &&  
		   ( id_exmem_ir_0.read().range(20, 16).to_uint() == 
		     if_id_ir_0.read().range(25, 21).to_uint() ) &&
		   ( id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000)))
	              
		{
		  cout << sc_time_stamp() << id_exmem_ir_0.read() << endl;
		  cout << sc_time_stamp() << if_id_ir_0.read() << endl;
		  cout << sc_time_stamp() << id_exmem_ir_0.read().range(20,16) << endl;
		  cout << sc_time_stamp() << if_id_ir_0.read().range(25,21) << endl;
		  cout << sc_time_stamp() << id_exmem_ir_0.read().range(20,16).to_uint() << endl;
		  cout << sc_time_stamp() << if_id_ir_0.read().range(25,21).to_uint() << endl;

		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "c" << std::endl;
		}
                     
	      /* IR_0 in EXMEM belongs to {addi, subi, multi, lw}
	       * and
	       * if IR_1 in ID is a I-type
	       */
	      else if (( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		       (( if_id_ir_1.read().range(31, 30).to_uint() == 0b00 ) && 
			( id_exmem_ir_0.read().range(20, 16).to_uint() == 
			  if_id_ir_1.read().range(25, 21).to_uint() ) &&
			( id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 )))
		{
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  if ( if_id_ir_1.read().range(31, 30).to_uint()==0b000101 ||
		       if_id_ir_0.read().range(31, 30).to_uint()==0b000110 )
		    {
		      nx_state.write(data_control_hazard);
		    }

		  else
		    {
		      nx_state.write(data_hazard_1);
		    }
		  std::cout << sc_time_stamp() << "d" << std::endl;
		}

	      // if IR_0 in ID is a R-type
	      else if ((  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		       ( if_id_ir_0.read().range(31, 30).to_uint() == 0b01 &&
			 ( id_exmem_ir_0.read().range(20, 16).to_uint() ==
			   if_id_ir_0.read().range(25, 21).to_uint() &&
			   id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 ) ||
			 ( id_exmem_ir_0.read().range(20, 16).to_uint() == 
			   if_id_ir_0.read().range(20, 16).to_uint() &&
			   id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "e" << std::endl;
		}
	              
	      // if IR_1 in ID is a R-type
	      else if ((  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		       ( ( if_id_ir_1.read().range(31, 30).to_uint() == 0b01 &&
			   ( id_exmem_ir_0.read().range(20, 16).to_uint() == 
			     if_id_ir_1.read().range(25, 21).to_uint() &&
			     id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 ) ||
			   ( id_exmem_ir_0.read().range(20, 16).to_uint() == 
			     if_id_ir_1.read().range(20, 16).to_uint() &&
			     id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 ))
			 )
		       )
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "f" << std::endl;
		}

	      // check for control hazard
	      else if (( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		       ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
	              	 if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ||
	              	 if_id_ir_1.read().range(31, 30).to_uint() == 0b10 ))
		{
		  nx_state.write(control_hazard);
		  std::cout << sc_time_stamp() << "g" << std::endl;
		}

	                			    
	      // if IR_0 in EXMEM is a R-type
	      // and if IR_0 in ID is a I-type
	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_0.read().range(31, 30).to_uint() == 0b00 &&
	              	  id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	  if_id_ir_0.read().range(25, 21).to_uint() &&
	              	  id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 ))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "i" << std::endl;
		}

	      // if IR_0 in EXMEM is a R-type
	      // and IR_1 in ID is a I-type
	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_1.read().range(31, 30).to_uint() == 0b00 &&
	              	  id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	  if_id_ir_1.read().range(25, 21).to_uint() &&
	              	  id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 ))
		{
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
	                
		  if ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
		       if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 )
		    {
		      nx_state.write(data_control_hazard);
		      std::cout << sc_time_stamp() << "j" << std::endl;
		    }
	                
		  else
		    {
		      nx_state.write(data_hazard_1);
		      std::cout << sc_time_stamp() << "k" << std::endl;
		    }
		}

	      // if IR_0 in EXMEM is a R-type
	      // and IR_0 in ID is a R-type
	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_0.read().range(31, 30).to_uint() == 0b01 &&
	              	  ( id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	    if_id_ir_0.read().range(25, 21).to_uint() &&
	              	    id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 ) ||
	              	  ( id_exmem_ir_0.read().range(15, 11).to_uint() ==
	              	    if_id_ir_0.read().range(20, 16).to_uint() &&
	              	    id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "l" << std::endl;
		}

	      // if IR_0 in EXMEM is a R-type
	      // and IR_1 in ID is a R-type
	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_1.read().range(31, 30).to_uint() == 0b01 &&
	              	  ( id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	    if_id_ir_1.read().range(25, 21).to_uint() &&
	              	    id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 ) ||
	              	  ( id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	    if_id_ir_1.read().range(20, 16).to_uint() &&
	              	    id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "m" << std::endl;
		}

	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
	              	  if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ||
	              	  if_id_ir_1.read().range(31, 30).to_uint() == 0b10 ))
		{
		  nx_state.write(control_hazard);
		  std::cout << sc_time_stamp() << "n" << std::endl;

		}
			
	      // if IR_1 in EXMEM is a I-type
	      // and IR_0 in ID is a I-type
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_0.read().range(31, 30).to_uint() == 0b00 &&
	              	 id_exmem_ir_1.read().range(20, 16).to_uint() == 
	              	 if_id_ir_0.read().range(25, 21).to_uint() &&
	              	 id_exmem_ir_1.read().range(20,16).to_uint() != 0b00000 ))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "p" << std::endl;
		}
	              
	      // if IR_1 in EXMEM is a I-type
	      // and IR_0 in ID is a I-type
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_1.read().range(31, 30).to_uint() == 0b00 &&
	              	 id_exmem_ir_1.read().range(20, 16).to_uint() ==
	              	 if_id_ir_1.read().range(25, 21).to_uint() &&
	              	 id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 ))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "q" << std::endl;

		}
	              
	      // if IR_1 in EXMEM is a I-type
	      // and IR_0 in ID is a R-type
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_0.read().range(31, 30).to_uint() == 0b01 &&
	              	 ( id_exmem_ir_1.read().range(20, 16).to_uint() == 
	              	   if_id_ir_0.read().range(25, 21).to_uint() &&
	              	   id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 ) ||
	              	 ( id_exmem_ir_1.read().range(20, 16).to_uint() == 
	              	   if_id_ir_0.read().range(20, 16).to_uint() &&
	              	   id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "r" << std::endl;
		}
	              
	      // if IR_1 in EXMEM is a I-type
	      // and IR_1 in ID is a R-type
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_1.read().range(31, 30).to_uint() == 0b01 &&
	              	 ( id_exmem_ir_1.read().range(20, 16).to_uint() ==
	              	   if_id_ir_1.read().range(25, 21).to_uint() &&
	              	   id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 ) ||
	              	 ( id_exmem_ir_1.read().range(20, 16).to_uint() == 
	              	   if_id_ir_1.read().range(20, 16).to_uint() &&
	              	   id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "r" << std::endl;
		}

	      // if IR_1 in EXMEM is a I-type
	      // check for control hazard
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
	              	 if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ||
	              	 if_id_ir_1.read().range(31, 30).to_uint() == 0b10 ))
		{
		  nx_state.write(control_hazard);
		  std::cout << sc_time_stamp() << "s" << std::endl;
		}
	              
	      // if IR_1 in EXMEM is a R-type
	      // and IR0 in ID is a I-type
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_0.read().range(31, 30).to_uint() == 0b00 &&
	              	  id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	  if_id_ir_0.read().range(25, 21).to_uint() &&
	              	  id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 ))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "u" << std::endl;
		}

	      // if IR_1 in EXMEM is a R-type
	      // if IR_1 in ID is a I-type
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_1.read().range(31, 30).to_uint() == 0b00 &&
	              	  id_exmem_ir_1.read().range(15, 11).to_uint() ==
	              	  if_id_ir_1.read().range(25, 21).to_uint() &&
	              	  id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 ))
		{
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
	                  
		  if ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
		       if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 )
		    {
		      nx_state.write(data_control_hazard);
		      std::cout << sc_time_stamp() << "v" << std::endl;
		    }
	                
		  else
		    {
		      nx_state.write(data_hazard_1);
		      std::cout << sc_time_stamp() << "v" << std::endl;
		    }

		}

	      // if IR_1 in EXMEM is a R-type
	      // and IR_0 in ID is a R-type
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_0.read().range(31, 30).to_uint() == 0b01 &&
	              	  ( id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	    if_id_ir_0.read().range(25, 21).to_uint() &&
	              	    id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 ) ||
	              	  ( id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	    if_id_ir_0.read().range(20, 16).to_uint() &&
	              	    id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "w" << std::endl;
		}
	              
	      // if IR_1 in EXMEM is a R-type
	      // if IR_1 in ID is a R-type
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_1.read().range(31, 30).to_uint() == 0b01 &&
	              	  ( id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	    if_id_ir_1.read().range(25, 21).to_uint() &&
	              	    id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 ) ||
	              	  ( id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	    if_id_ir_1.read().range(20, 16).to_uint() &&
	              	    id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "x" << std::endl;
		}

	      // if IR_1 in EXMEM is a R-type
	      // check for control hazard
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
	              	  if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ||
	              	  if_id_ir_1.read().range(31, 30).to_uint() == 0b10 ))
		{
		  nx_state.write(control_hazard);
		  std::cout << sc_time_stamp() << "y" << std::endl;
		}

	      else
		{
		  if_id_we.write(SC_LOGIC_1);
		  id_exmem_we.write(SC_LOGIC_1);
		  id_exmem_rst.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_1);
		  exmem_wb_we.write(SC_LOGIC_1);
		  nx_state.write(idle_hazard);
		  std::cout << sc_time_stamp() << "z" << std::endl;
		}
	    }

	  break;

	case structural_hazard:

	  std::cout << sc_time_stamp() << "HDU: NOTE - STRUCTURAL_HAZARD state." << std::endl;

	  /* A nop is entering the EXMEM stage */
	  if ( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000000 &&
	       id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000000 )
	    {
	      /* Enables write on PC and Latches */
	      pc_write_enable.write(SC_LOGIC_1);
	      if_id_we.write(SC_LOGIC_1);
	      id_exmem_we.write(SC_LOGIC_1);
	      exmem_wb_we.write(SC_LOGIC_1);
	      /* Do not activate reset */
	      if_id_rst.write(SC_LOGIC_0);
	      id_exmem_rst.write(SC_LOGIC_0);
	      /* Next state is idle again. */
	      nx_state.write(idle_hazard);
	    }
	  /* Structural Hazard (at least one ALU is busy computing) */
	  else if ( alu_0_data_ready == SC_LOGIC_0 ||
		    alu_1_data_ready == SC_LOGIC_0 )
	    {
	      /* Stalls the pipeline */
	      pc_write_enable.write(SC_LOGIC_0);
	      if_id_we.write(SC_LOGIC_0);
	      id_exmem_we.write(SC_LOGIC_0);
	      exmem_wb_we.write(SC_LOGIC_0);
	      /* Do not activate reset */
	      if_id_rst.write(SC_LOGIC_0);
	      id_exmem_rst.write(SC_LOGIC_0);
	      /* Next state is Structural Hazard */
	      nx_state.write(structural_hazard);
	    }
	  /* Data Hazard  */
	  else
	    {
	      /* Initialize enabling all signals */
	      pc_write_enable.write(SC_LOGIC_1);
	      id_exmem_we.write(SC_LOGIC_1);
	      if_id_we.write(SC_LOGIC_1);
	      exmem_wb_we.write(SC_LOGIC_1);
                
	      // IR_0 in EXMEM belongs to {addi, subi, multi, lw}
	      // IR_0 in ID is a I-type
	      if ( (id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
		    id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
		    id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
		    id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) 
		   &&
		   (if_id_ir_0.read().range(31, 30).to_uint() == 0b00 &&  
		    (id_exmem_ir_0.read().range(20, 16).to_uint() == if_id_ir_0.read().range(25, 21).to_uint() ) &&
		    id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000))
		{
		  cout << sc_time_stamp() << id_exmem_ir_0.read() << endl;
		  cout << sc_time_stamp() << if_id_ir_0.read() << endl;
		  cout << sc_time_stamp() << id_exmem_ir_0.read().range(20,16) << endl;
		  cout << sc_time_stamp() << if_id_ir_0.read().range(25,21) << endl;
		  cout << sc_time_stamp() << id_exmem_ir_0.read().range(20,16).to_uint() << endl;
		  cout << sc_time_stamp() << if_id_ir_0.read().range(25,21).to_uint() << endl;

		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "c" << std::endl;
		}
                     
	      /* IR_0 in EXMEM belongs to {addi, subi, multi, lw}
	       * and
	       * if IR_1 in ID is a I-type
	       */
	      else if (( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		       (( if_id_ir_1.read().range(31, 30).to_uint() == 0b00 ) && 
			( id_exmem_ir_0.read().range(20, 16).to_uint() == 
			  if_id_ir_1.read().range(25, 21).to_uint() ) &&
			( id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 )))
		{
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  if ( if_id_ir_1.read().range(31, 30).to_uint()==0b000101 ||
		       if_id_ir_0.read().range(31, 30).to_uint()==0b000110 )
		    {
		      nx_state.write(data_control_hazard);
		    }

		  else
		    {
		      nx_state.write(data_hazard_1);
		    }
		  std::cout << sc_time_stamp() << "d" << std::endl;
		}

	      // if IR_0 in ID is a R-type
	      else if ((  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		       ( if_id_ir_0.read().range(31, 30).to_uint() == 0b01 &&
			 ( id_exmem_ir_0.read().range(20, 16).to_uint() ==
			   if_id_ir_0.read().range(25, 21).to_uint() &&
			   id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 ) ||
			 ( id_exmem_ir_0.read().range(20, 16).to_uint() == 
			   if_id_ir_0.read().range(20, 16).to_uint() &&
			   id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "e" << std::endl;
		}
	              
	      // if IR_1 in ID is a R-type
	      else if ((  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
	              	  id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		       ( ( if_id_ir_1.read().range(31, 30).to_uint() == 0b01 &&
			   ( id_exmem_ir_0.read().range(20, 16).to_uint() == 
			     if_id_ir_1.read().range(25, 21).to_uint() &&
			     id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 ) ||
			   ( id_exmem_ir_0.read().range(20, 16).to_uint() == 
			     if_id_ir_1.read().range(20, 16).to_uint() &&
			     id_exmem_ir_0.read().range(20, 16).to_uint() != 0b00000 ))
			 )
		       )
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "f" << std::endl;
		}

	      // check for control hazard
	      else if (( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000001 || 
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000111 || 
	              	 id_exmem_ir_0.read().range(31, 26).to_uint() == 0b000011 ) &&
		       ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
	              	 if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ||
	              	 if_id_ir_1.read().range(31, 30).to_uint() == 0b10 ))
		{
		  nx_state.write(control_hazard);
		  std::cout << sc_time_stamp() << "g" << std::endl;
		}

	                			    
	      // if IR_0 in EXMEM is a R-type
	      // and if IR_0 in ID is a I-type
	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_0.read().range(31, 30).to_uint() == 0b00 &&
	              	  id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	  if_id_ir_0.read().range(25, 21).to_uint() &&
	              	  id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 ))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "i" << std::endl;
		}

	      // if IR_0 in EXMEM is a R-type
	      // and IR_1 in ID is a I-type
	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_1.read().range(31, 30).to_uint() == 0b00 &&
	              	  id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	  if_id_ir_1.read().range(25, 21).to_uint() &&
	              	  id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 ))
		{
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
	                
		  if ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
		       if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 )
		    {
		      nx_state.write(data_control_hazard);
		      std::cout << sc_time_stamp() << "j" << std::endl;
		    }
	                
		  else
		    {
		      nx_state.write(data_hazard_1);
		      std::cout << sc_time_stamp() << "k" << std::endl;
		    }
		}

	      // if IR_0 in EXMEM is a R-type
	      // and IR_0 in ID is a R-type
	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_0.read().range(31, 30).to_uint() == 0b01 &&
	              	  ( id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	    if_id_ir_0.read().range(25, 21).to_uint() &&
	              	    id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 ) ||
	              	  ( id_exmem_ir_0.read().range(15, 11).to_uint() ==
	              	    if_id_ir_0.read().range(20, 16).to_uint() &&
	              	    id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "l" << std::endl;
		}

	      // if IR_0 in EXMEM is a R-type
	      // and IR_1 in ID is a R-type
	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_1.read().range(31, 30).to_uint() == 0b01 &&
	              	  ( id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	    if_id_ir_1.read().range(25, 21).to_uint() &&
	              	    id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 ) ||
	              	  ( id_exmem_ir_0.read().range(15, 11).to_uint() == 
	              	    if_id_ir_1.read().range(20, 16).to_uint() &&
	              	    id_exmem_ir_0.read().range(15, 11).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "m" << std::endl;
		}

	      else if ( id_exmem_ir_0.read().range(31, 26).to_uint() == 0b010000  &&
	              	( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
	              	  if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ||
	              	  if_id_ir_1.read().range(31, 30).to_uint() == 0b10 ))
		{
		  nx_state.write(control_hazard);
		  std::cout << sc_time_stamp() << "n" << std::endl;

		}
	      // if IR_1 in EXMEM is a I-type
	      // and IR_0 in ID is a I-type
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_0.read().range(31, 30).to_uint() == 0b00 &&
	              	 id_exmem_ir_1.read().range(20, 16).to_uint() == 
	              	 if_id_ir_0.read().range(25, 21).to_uint() &&
	              	 id_exmem_ir_1.read().range(20,16).to_uint() != 0b00000 ))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "p" << std::endl;
		}
	              
	      // if IR_1 in EXMEM is a I-type
	      // and IR_0 in ID is a I-type
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_1.read().range(31, 30).to_uint() == 0b00 &&
	              	 id_exmem_ir_1.read().range(20, 16).to_uint() ==
	              	 if_id_ir_1.read().range(25, 21).to_uint() &&
	              	 id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 ))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "q" << std::endl;

		}
	              
	      // if IR_1 in EXMEM is a I-type
	      // and IR_0 in ID is a R-type
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_0.read().range(31, 30).to_uint() == 0b01 &&
	              	 ( id_exmem_ir_1.read().range(20, 16).to_uint() == 
	              	   if_id_ir_0.read().range(25, 21).to_uint() &&
	              	   id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 ) ||
	              	 ( id_exmem_ir_1.read().range(20, 16).to_uint() == 
	              	   if_id_ir_0.read().range(20, 16).to_uint() &&
	              	   id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "r" << std::endl;
		}
	              
	      // if IR_1 in EXMEM is a I-type
	      // and IR_1 in ID is a R-type
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_1.read().range(31, 30).to_uint() == 0b01 &&
	              	 ( id_exmem_ir_1.read().range(20, 16).to_uint() ==
	              	   if_id_ir_1.read().range(25, 21).to_uint() &&
	              	   id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 ) ||
	              	 ( id_exmem_ir_1.read().range(20, 16).to_uint() == 
	              	   if_id_ir_1.read().range(20, 16).to_uint() &&
	              	   id_exmem_ir_1.read().range(20, 16).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "r" << std::endl;
		}

	      // if IR_1 in EXMEM is a I-type
	      // check for control hazard
	      else if (( id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000001 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000010 ||
	              	 id_exmem_ir_1.read().range(31, 26).to_uint() == 0b000111 ) &&
		       ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
	              	 if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ||
	              	 if_id_ir_1.read().range(31, 30).to_uint() == 0b10 ))
		{
		  nx_state.write(control_hazard);
		  std::cout << sc_time_stamp() << "s" << std::endl;
		}
	              
	      // if IR_1 in EXMEM is a R-type
	      // and IR0 in ID is a I-type
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_0.read().range(31, 30).to_uint() == 0b00 &&
	              	  id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	  if_id_ir_0.read().range(25, 21).to_uint() &&
	              	  id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 ))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "u" << std::endl;
		}

	      // if IR_1 in EXMEM is a R-type
	      // if IR_1 in ID is a I-type
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_1.read().range(31, 30).to_uint() == 0b00 &&
	              	  id_exmem_ir_1.read().range(15, 11).to_uint() ==
	              	  if_id_ir_1.read().range(25, 21).to_uint() &&
	              	  id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 ))
		{
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
	                  
		  if ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
		       if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 )
		    {
		      nx_state.write(data_control_hazard);
		      std::cout << sc_time_stamp() << "v" << std::endl;
		    }
	                
		  else
		    {
		      nx_state.write(data_hazard_1);
		      std::cout << sc_time_stamp() << "v" << std::endl;
		    }

		}

	      // if IR_1 in EXMEM is a R-type
	      // and IR_0 in ID is a R-type
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_0.read().range(31, 30).to_uint() == 0b01 &&
	              	  ( id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	    if_id_ir_0.read().range(25, 21).to_uint() &&
	              	    id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 ) ||
	              	  ( id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	    if_id_ir_0.read().range(20, 16).to_uint() &&
	              	    id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "w" << std::endl;
		}
	              
	      // if IR_1 in EXMEM is a R-type
	      // if IR_1 in ID is a R-type
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_1.read().range(31, 30).to_uint() == 0b01 &&
	              	  ( id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	    if_id_ir_1.read().range(25, 21).to_uint() &&
	              	    id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 ) ||
	              	  ( id_exmem_ir_1.read().range(15, 11).to_uint() == 
	              	    if_id_ir_1.read().range(20, 16).to_uint() &&
	              	    id_exmem_ir_1.read().range(15, 11).to_uint() != 0b00000 )))
		{
		  nx_state.write(data_hazard_1);
		  id_exmem_we.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_0);
		  if_id_we.write(SC_LOGIC_0);
		  std::cout << sc_time_stamp() << "x" << std::endl;
		}

	      // if IR_1 in EXMEM is a R-type
	      // check for control hazard
	      else if ( id_exmem_ir_1.read().range(31, 26) == 0b010000 &&
	              	( if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 ||
	              	  if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ||
	              	  if_id_ir_1.read().range(31, 30).to_uint() == 0b10 ))
		{
		  nx_state.write(control_hazard);
		  std::cout << sc_time_stamp() << "y" << std::endl;
		}

	      else
		{
		  if_id_we.write(SC_LOGIC_1);
		  id_exmem_we.write(SC_LOGIC_1);
		  id_exmem_rst.write(SC_LOGIC_0);
		  pc_write_enable.write(SC_LOGIC_1);
		  exmem_wb_we.write(SC_LOGIC_1);
		  nx_state.write(idle_hazard);
		  std::cout << sc_time_stamp() << "z" << std::endl;
		}
	    }

	  break;

	case control_hazard:
	
	  std::cout << sc_time_stamp() << "HDU: NOTE - CONTROL_HAZARD state." << std::endl;
	  cout << if_id_ir_1.read() << endl;       

	  if ( if_id_ir_1.read().range(31, 26).to_uint() == 0b000000 )
	    {
	      if_id_rst.write(SC_LOGIC_0);
	      std::cout << sc_time_stamp() << " HDU: if_id_rst => 0 " << std::endl;
	    }
	  
	  else if ( jump_or_branch_taken.read() == SC_LOGIC_1 ||
		    if_id_ir_1.read().range(31, 30).to_uint() == 0b10 )
	    {
	      std::cout << sc_time_stamp() << " I'm entering here " << std::endl;

	      if_id_rst.write(SC_LOGIC_1);
	      std::cout << sc_time_stamp() << " HDU: if_id_rst => 1 " << std::endl;
	      std::cout << sc_time_stamp() << "  "<<if_id_rst.read()<< std::endl;
	    }
	  
	  else
	    {
	      if_id_rst.write(SC_LOGIC_0);
	      std::cout << sc_time_stamp() << " HDU: if_id_rst => 0 " << std::endl;
	    }
          
	  nx_state.write(idle_hazard);
	  
	  break;
	  
	case data_control_hazard:
	  
	  std::cout << sc_time_stamp() << "HDU: NOTE - DATA_CONTROL_HAZARD state." << std::endl;
	  cout << if_id_ir_1.read() << endl;
	  
	  nx_state.write(data_hazard_1);
	  pc_write_enable.write(SC_LOGIC_1);
	  if_id_we.write(SC_LOGIC_1);
	  cout << "End of DATA_CONTROL_HAZARD" << endl;
	  cout << jump_or_branch_taken << endl;
	  break;
	  
	case data_hazard_1:
	  
	  std::cout << sc_time_stamp() << "HDU: NOTE - DATA_HAZARD_1 state." << std::endl;
	  
	  /* 	  if ( (if_id_ir_1.read().range(31, 26).to_uint() == 0b000101 || */
	  /* 		if_id_ir_1.read().range(31, 26).to_uint() == 0b000110 ) && */
	  /* 	       jump_or_branch_taken == SC_LOGIC_1 ) */
	  if ( jump_or_branch_taken == SC_LOGIC_1 )
	    {
	      cout << if_id_ir_1.read() << endl;
	      if_id_rst.write(SC_LOGIC_1);
	      std::cout << sc_time_stamp() << "zz" << std::endl;
	    }
	  else
	    {
	      nx_state.write(idle_hazard);
	      pc_write_enable.write(SC_LOGIC_1);
	      if_id_we.write(SC_LOGIC_1);
	      id_exmem_we.write(SC_LOGIC_1);
	      id_exmem_rst.write(SC_LOGIC_0);
	      if_id_rst.write(SC_LOGIC_0);
	      exmem_wb_we.write(SC_LOGIC_1);	  
	      std::cout << sc_time_stamp() << "zzz" << std::endl;
	    }
	  
	  break;
	  
	default:
	  std::cout << sc_time_stamp() << "HDU: ERROR - Invalid state encountered"
		    << std::endl;
	}
    }
}
#endif
