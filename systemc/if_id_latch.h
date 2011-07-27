/*
 * if_id_latch.cc    
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

#ifndef IF_ID_LATCH
#define IF_ID_LATCH

#include <systemc.h>
#include <iostream>

#define W_SIZE 32

class if_id_latch : sc_module 
{
  public:
    /* Clock and Reset */
    sc_in<bool> clk;
    sc_in<sc_logic> rst;
    sc_in<sc_logic> hdu_rst; // reset signal from hdu
    sc_in<sc_logic> we; // write enable

    /* Inputs */
    sc_in<sc_lv<W_SIZE> > npc_in;
    sc_in<sc_lv<W_SIZE> > ir_1_in;
    sc_in<sc_lv<W_SIZE> > ir_0_in;

    /* Outputs */
    sc_out<sc_lv<W_SIZE> > npc_out;
    sc_out<sc_lv<W_SIZE> > ir_1_out;
    sc_out<sc_lv<W_SIZE> > ir_0_out;
    
    /* Processes */
    void process();

    if_id_latch(sc_module_name _name, sc_trace_file* tcf, bool trace_enabled) : sc_module(_name), _tcf(tcf)
     {
        SC_METHOD(process);
            sensitive << clk.pos() << rst
		      << hdu_rst;
        dont_initialize();

        if (trace_enabled)
        {
            sc_trace(_tcf,hdu_rst,"1b_IF_ID_hdu_rst");
            sc_trace(_tcf,we,"1b_IF_ID_we");
            sc_trace(_tcf,npc_out,"012_1b_IF_ID_npc_out");
            sc_trace(_tcf,ir_1_out,"013_1b_IF_ID_ir_1_out");
            sc_trace(_tcf,ir_0_out,"014_1b_IF_ID_ir_0_out");
        }
    }
    SC_HAS_PROCESS(if_id_latch);	
    
  private:
    sc_trace_file *_tcf;
};


void if_id_latch::process() 
{
    //std::cout << "IF/ID LATCH PROCESS STARTING " << std::endl;
    
    if ( rst.read() == SC_LOGIC_1 ||
	 hdu_rst.read() == SC_LOGIC_1 )
    {
        //if ( hdu_rst.read() == SC_LOGIC_1 )
        //{
        //    std::cout<<"Received hdu RST !!"<<std::endl;
        //}
        npc_out.write(0);
        ir_1_out.write(0);
        ir_0_out.write(0);
    }
    
    else if ( clk.event() &&
	      we.read() == SC_LOGIC_1 )
    {
        npc_out.write(npc_in.read());
        ir_1_out.write(ir_1_in.read());
        ir_0_out.write(ir_0_in.read());
    }

    //std::cout << "NPC: " << npc_in.read() << std::endl;
    //std::cout << "IF/ID LATCH PROCESS ENDING " << std::endl;
}

#endif
