/*
 * exmem_wb_latch.h
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

#ifndef EXMEM_WB_LATCH
#define EXMEM_WB_LATCH

#define W_SIZE 32

#include <systemc.h>
#include <iostream>

class exmem_wb_latch : sc_module
{
 private:
  unsigned int dst_reg_jal;
	
 public:
  sc_in<bool> clk;
  sc_in<sc_logic> rst;
  sc_in<sc_logic> we; // write enable

  /* Inputs */
  sc_in<sc_lv<W_SIZE> > npc_in;
  sc_in<sc_lv<W_SIZE> > alu_out_1_in;
  sc_in<sc_lv<5> > dst_reg_1_in;
  sc_in<sc_lv<W_SIZE> > alu_out_0_in;
  sc_in<sc_lv<W_SIZE> > lmd_in;
  sc_in<sc_lv<5> > dst_reg_0_in;
  sc_in<sc_logic > jal_in;
  sc_in<sc_logic > mem_read_in;
  sc_in<sc_logic > wb_enable_1_in;
  sc_in<sc_logic > wb_enable_0_in;
	
  /* Outputs */
  sc_out<sc_lv<W_SIZE> > npc_out;
  sc_out<sc_lv<W_SIZE> > alu_out_1_out;
  sc_out<sc_lv<5> > dst_reg_1_out;
  sc_out<sc_lv<5> > dst_reg_jal_out;
  sc_out<sc_lv<W_SIZE> > alu_out_0_out;
  sc_out<sc_lv<W_SIZE> > lmd_out;
  sc_out<sc_lv<5> > dst_reg_0_out;
  sc_out<sc_logic > jal_out;
  sc_out<sc_logic > mem_read_out;
  sc_out<sc_logic > wb_enable_1_out;
  sc_out<sc_logic > wb_enable_0_out;

  /* Process */
  void process();

    exmem_wb_latch(sc_module_name _name, sc_trace_file* tcf, bool trace_enabled) : sc_module(_name), _tcf(tcf)
   {
    SC_METHOD(process);
    sensitive << clk.pos() << rst;
    dst_reg_jal=31;

    if (trace_enabled) 
    {
        sc_trace(_tcf,npc_out,"3b_EXMEM_we");
        sc_trace(_tcf,npc_out,"3b_EXMEM_WB_npc_out");
        sc_trace(_tcf,alu_out_0_out,"3b_EXMEM_WB_alu_out_0_out");
        sc_trace(_tcf,alu_out_1_out,"3b_EXMEM_WB_alu_out_1_out");
        sc_trace(_tcf,dst_reg_0_out,"3b_EXMEM_WB_dst_reg_0_out");
        sc_trace(_tcf,dst_reg_1_out,"3b_EXMEM_WB_dst_reg_1_out");
        sc_trace(_tcf,dst_reg_jal_out,"3b_EXMEM_WB_dst_reg_jal");
        sc_trace(_tcf,lmd_out,"3b_EXMEM_WB_lmd_out");
        sc_trace(_tcf,jal_out,"3b_EXMEM_WB_jal_out");
        sc_trace(_tcf,mem_read_out,"3b_EXMEM_WB_mem_read_out");
        sc_trace(_tcf,wb_enable_1_out,"3b_EXMEM_WB_wb_enable_1_out");
        sc_trace(_tcf,wb_enable_0_out,"3b_EXMEM_WB_wb_enable_0_out");
    }

  }
    SC_HAS_PROCESS(exmem_wb_latch);
 private:
    sc_trace_file *_tcf;
	
};

void exmem_wb_latch::process() 
{
  //std::cout << "EXMEM/WB LATCH PROCESS IS STARTING " << std::endl;
  	
  if (rst.read() == SC_LOGIC_1)
    {
      npc_out.write(0);
      alu_out_1_out.write(0);
      dst_reg_1_out.write(0);
      alu_out_0_out.write(0);
      lmd_out.write(0);
      dst_reg_0_out.write(0);
      dst_reg_jal_out.write(0);
      jal_out.write(SC_LOGIC_0);
      mem_read_out.write(SC_LOGIC_0);
      wb_enable_1_out.write(SC_LOGIC_0);
      wb_enable_0_out.write(SC_LOGIC_0);

    } 

  else if ( clk.event() &&
	    we.read() == SC_LOGIC_1 )
    {
      npc_out.write(npc_in.read());
      alu_out_1_out.write(alu_out_1_in.read());
      dst_reg_1_out.write(dst_reg_1_in.read());
      dst_reg_jal_out.write(dst_reg_jal);
      alu_out_0_out.write(alu_out_0_in.read());
      lmd_out.write(lmd_in.read());
      dst_reg_0_out.write(dst_reg_0_in.read());
      jal_out.write(jal_in.read());
      mem_read_out.write(mem_read_in.read());
      wb_enable_1_out.write(wb_enable_1_in.read());
      wb_enable_0_out.write(wb_enable_0_in.read());
    }

  //std::cout << "EXMEM/WB LATCH PROCESS ENDED " << std::endl;
  
}
#endif
 
