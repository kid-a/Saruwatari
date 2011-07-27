/*
 * wb_stage.h    
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

#ifndef WB_STAGE
#define WB_STAGE

#define W_SIZE 32
#include <systemc.h>
#include <iostream>

class wb_stage : sc_module
{
 private:
	
 public:
  sc_in<bool> clk;
  sc_in<sc_logic> rst;

  /* inputs from exmem/wb latch */
  sc_in<sc_lv<W_SIZE> > npc;
  sc_in<sc_lv<W_SIZE> > alu_out_1;
  sc_in<sc_lv<5> > dst_reg_1;
  sc_in<sc_lv<5> > dst_reg_jal;
  sc_in<sc_lv<W_SIZE> > alu_out_0;
  sc_in<sc_lv<W_SIZE> > lmd;
  sc_in<sc_lv<5> > dst_reg_0;
  sc_in<sc_logic > jal;
  sc_in<sc_logic > mem_read;
  sc_in<sc_logic > wb_enable_1;
  sc_in<sc_logic > wb_enable_0;

  /* outputs to ID stage */
  sc_out<sc_logic> reg_file_write_enable_1;
  sc_out<sc_lv<5> > reg_file_write_address_1;
  sc_out<sc_lv<W_SIZE> > reg_file_write_data_1;
  sc_out<sc_logic> reg_file_write_enable_0;
  sc_out<sc_lv<5> > reg_file_write_address_0;
  sc_out<sc_lv<W_SIZE> > reg_file_write_data_0;

  /* Process */
  void process();

 wb_stage(sc_module_name _name, sc_trace_file* tcf, bool trace_enabled) : sc_module(_name), _tcf(tcf)
  {
    SC_METHOD(process);
    sensitive << rst 
	      << npc 
	      << alu_out_1
	      << dst_reg_1
	      << dst_reg_jal
	      << alu_out_0
	      << lmd
	      << dst_reg_0
	      << jal
	      << mem_read
	      << wb_enable_1
	      << wb_enable_0;

    if (trace_enabled) 
      {
	sc_trace(_tcf,reg_file_write_data_0,"4_WB_write_data_0");
	sc_trace(_tcf,reg_file_write_enable_0,"4_WB_write_enable_0");
	sc_trace(_tcf,reg_file_write_address_0,"4_WB_write_address_0");
            
	sc_trace(_tcf,reg_file_write_data_1,"4_WB_write_data_1");
	sc_trace(_tcf,reg_file_write_enable_1,"4_WB_write_enable_1");
	sc_trace(_tcf,reg_file_write_address_1,"4_WB_write_address_1");
      } 
  }
  SC_HAS_PROCESS(wb_stage);
 private:
  sc_trace_file *_tcf;
};

void wb_stage::process()
{
  if ( rst.read() == SC_LOGIC_1 ) {
    reg_file_write_enable_1.write(SC_LOGIC_0);
    reg_file_write_address_1.write(0);
    reg_file_write_data_1.write(0);
    reg_file_write_enable_0.write(SC_LOGIC_0);
    reg_file_write_address_0.write(0);
    reg_file_write_data_0.write(0);
  } else {
		
      if ( jal.read() == SC_LOGIC_1 ) {
	reg_file_write_data_1.write(npc.read());
	reg_file_write_address_1.write( dst_reg_jal.read() );
      } else {
	reg_file_write_data_1.write(alu_out_1.read());
	reg_file_write_address_1.write( dst_reg_1.read() );
      }
      
      if ( mem_read.read() == SC_LOGIC_1 ) {
	reg_file_write_data_0.write(lmd.read());
      } else {
	reg_file_write_data_0.write(alu_out_0.read());
      }
      
      reg_file_write_address_0.write( dst_reg_0.read() );
      reg_file_write_enable_1.write(wb_enable_1.read());
      reg_file_write_enable_0.write(wb_enable_0.read());
    }
}

#endif
