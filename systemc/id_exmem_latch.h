/*
 * id_exmem_latch.h 
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

#ifndef _ID_EXMEM_LATCH
#define _ID_EXMEM_LATCH

#include <systemc.h>
#include <iostream>

#define W_SIZE 32

class id_exmem_latch : sc_module
{
  public:
    /* Clock an Reset */
    sc_in<bool> clk;
    sc_in<sc_logic> rst;
    sc_in<sc_logic> hdu_rst; // reset signal from hdu
    sc_in<sc_logic> we; // write enable
    
    /* Inputs from ID */
    sc_in<sc_lv<W_SIZE> > npc_in;
    sc_in<sc_lv<W_SIZE> > ir_1_in;
    sc_in<sc_lv<W_SIZE> > ir_0_in;
    sc_in<sc_lv<W_SIZE> > A0_in;
    sc_in<sc_lv<W_SIZE> > B0_in;
    sc_in<sc_lv<W_SIZE> > A1_in;
    sc_in<sc_lv<W_SIZE> > B1_in;
    sc_in<sc_lv<W_SIZE> > immediate_1_in;
    sc_in<sc_lv<W_SIZE> > immediate_0_in;
    /* Inputs from CU */
    sc_in<sc_logic> alu_src_1_in;
    sc_in<sc_logic> alu_src_0_in;
    sc_in<sc_logic> reg_dst_1_in;
    sc_in<sc_logic> reg_dst_0_in;
    sc_in<sc_logic> jal_in;
    sc_in<sc_logic> mem_read_in;
    sc_in<sc_logic> mem_write_in;
    sc_in<sc_logic> wb_enable_1_in;
    sc_in<sc_logic> wb_enable_0_in;
    
    /* Outputs to EXMEM */
    sc_out<sc_lv<W_SIZE> > npc_out;
    sc_out<sc_lv<W_SIZE> > ir_1_out;
    sc_out<sc_lv<W_SIZE> > ir_0_out;
    sc_out<sc_lv<W_SIZE> > A0_out;
    sc_out<sc_lv<W_SIZE> > B0_out;
    sc_out<sc_lv<W_SIZE> > A1_out;
    sc_out<sc_lv<W_SIZE> > B1_out;
    sc_out<sc_lv<W_SIZE> > immediate_1_out;
    sc_out<sc_lv<W_SIZE> > immediate_0_out;

    sc_out<sc_logic> alu_src_1_out;
    sc_out<sc_logic> alu_src_0_out;
    sc_out<sc_logic> reg_dst_1_out;
    sc_out<sc_logic> reg_dst_0_out;
    sc_out<sc_logic> jal_out;
    sc_out<sc_logic> mem_read_out;
    sc_out<sc_logic> mem_write_out;
    sc_out<sc_logic> wb_enable_1_out;
    sc_out<sc_logic> wb_enable_0_out;

    /* Processes */
    void process();

    id_exmem_latch(sc_module_name _name, sc_trace_file* tcf, bool trace_enabled) : sc_module(_name), _tcf(tcf)
    {
        SC_METHOD(process);
            sensitive << clk.pos() << rst
		      << hdu_rst;
        if (trace_enabled)
        {
	  sc_trace(_tcf,hdu_rst,"2b_ID_EXMEM_hdu_rst");
	  sc_trace(_tcf,we,"2b_ID_EXMEM_we");
	  sc_trace(_tcf,npc_out,"2b_ID_EXMEM_npc_out");  
	  sc_trace(_tcf,ir_1_out,"2b_ID_EXMEM_ir_1_out");  
	  sc_trace(_tcf,ir_0_out,"2b_ID_EXMEM_ir_0_out");
	  sc_trace(_tcf,A0_out,"2b_ID_EXMEM_A0_out");  
	  sc_trace(_tcf,B0_out,"2b_ID_EXMEM_B0_out");  
	  sc_trace(_tcf,A1_out,"2b_ID_EXMEM_A1_out");  
	  sc_trace(_tcf,B1_out,"2b_ID_EXMEM_B1_out");  
	  sc_trace(_tcf,immediate_1_out,"2b_ID_EXMEM_IMM_1_out");  
	  sc_trace(_tcf,immediate_0_out,"2b_ID_EXMEM_IMM_0_out");  
	  sc_trace(_tcf,alu_src_1_out,"2b_ID_EXMEM_alu_src_1_out");  
	  sc_trace(_tcf,alu_src_0_out,"2b_ID_EXMEM_alu_src_0_out");  
	  sc_trace(_tcf,reg_dst_0_out,"2b_ID_EXMEM_reg_dst_0_out");  
	  sc_trace(_tcf,reg_dst_1_out,"2b_ID_EXMEM_reg_dst_1_out");  
	  sc_trace(_tcf,jal_out,"2b_ID_EXMEM_jal_out");  
	  sc_trace(_tcf,mem_read_out,"2b_ID_EXMEM_mem_read_out");  
	  sc_trace(_tcf,mem_write_out,"2b_ID_EXMEM_mem_write_out");
	  sc_trace(_tcf,wb_enable_1_out,"2b_ID_EXMEM_wb_enable_1_out");  
	  sc_trace(_tcf,wb_enable_0_out,"2b_ID_EXMEM_wb_enable_0_out");  
        }
    }
    SC_HAS_PROCESS(id_exmem_latch);
  private:
    sc_trace_file *_tcf;
};


void id_exmem_latch::process() 
{
  //std::cout << "ID/EXMEM LATCH PROCESS STARTING " << std::endl;
  
    if ( rst.read() == SC_LOGIC_1 or 
	 hdu_rst.read() == SC_LOGIC_1 )
    {
        npc_out.write(0);
        ir_1_out.write(0);
        ir_0_out.write(0);
        A0_out.write(0);
        B0_out.write(0);
        A1_out.write(0);
        B1_out.write(0);
        immediate_1_out.write(0);
        immediate_0_out.write(0);
        alu_src_1_out.write(SC_LOGIC_0);
        alu_src_0_out.write(SC_LOGIC_0);
        reg_dst_1_out.write(SC_LOGIC_0);
        reg_dst_0_out.write(SC_LOGIC_0);
        jal_out.write(SC_LOGIC_0);
        mem_read_out.write(SC_LOGIC_0);
        mem_write_out.write(SC_LOGIC_0);
        wb_enable_1_out.write(SC_LOGIC_0);
        wb_enable_0_out.write(SC_LOGIC_0);
    }
    else if ( clk.event() &&
	      we.read() == SC_LOGIC_1 )
    {
        npc_out.write(npc_in.read());
        ir_1_out.write(ir_1_in.read());
        ir_0_out.write(ir_0_in.read());
        A0_out.write(A0_in.read());
        B0_out.write(B0_in.read());
        A1_out.write(A1_in.read());
        B1_out.write(B1_in.read());
        immediate_1_out.write(immediate_1_in.read());
        immediate_0_out.write(immediate_0_in.read());
        alu_src_1_out.write(alu_src_1_in.read());
        alu_src_0_out.write(alu_src_0_in.read());
        reg_dst_1_out.write(reg_dst_1_in.read());
        reg_dst_0_out.write(reg_dst_0_in.read());
        jal_out.write(jal_in.read());
        mem_read_out.write(mem_read_in.read());
        mem_write_out.write(mem_write_in.read());
        wb_enable_1_out.write(wb_enable_1_in.read());
        wb_enable_0_out.write(wb_enable_0_in.read());
    }

    //std::cout << "ID/EXMEM LATCH PROCESS ENDING " << std::endl;
}

#endif
