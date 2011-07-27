/*
 * if_stage.h    
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

#ifndef IF_STAGE
#define IF_STAGE

#include <systemc.h>

#define W_SIZE 32

class if_stage : sc_module 
{
 public:
  sc_in<bool > clk;
  sc_in<sc_logic > rst;
  		
  /* Inputs from ID stage.	*/
  sc_in<sc_logic > pc_we;
  sc_in<sc_logic > jump_or_branch;
  sc_in<sc_lv<W_SIZE> > address_after_jump_or_branch;
  		
  /* Outputs to IF/ID latch	*/
  sc_out<sc_lv<W_SIZE> > npc;
  sc_out<sc_lv<W_SIZE> > instruction_1;
  sc_out<sc_lv<W_SIZE> > instruction_0;
  	
  /* Processes */
  void process();
  void instructions_output();
  
  /* Methods */
  void load_memory(const char* filename);
  		
 if_stage(sc_module_name _name, sc_trace_file* tcf, bool trace_enabled) : sc_module(_name), _tcf(tcf)
  {
    SC_METHOD(process);
    sensitive << clk.pos() << rst;
    //dont_initialize();

    SC_METHOD(instructions_output);
    sensitive << _pc;
    //dont_initialize();
	    

    _memory = std::vector<sc_lv<W_SIZE> >(256);
    load_memory("mem_dump.dat");
    _pc.write(0);
    /*         _index_pc=0; */
    if (trace_enabled) 
      {
	sc_trace(_tcf,pc_we,"002_1a_IF_pc_we");
	sc_trace(_tcf,jump_or_branch,"005_1a_IF_J_or_B");
	sc_trace(_tcf,address_after_jump_or_branch,"007_1a_IF_addr_J_or_B");
	sc_trace(_tcf,_pc,"003_1a_IF_pc");
	sc_trace(_tcf,npc,"004_1a_IF_npc");
	sc_trace(_tcf,instruction_1,"008_1a_IF_ir_1");
	sc_trace(_tcf,instruction_0,"009_1a_IF_ir_0");
      }
  }	
  SC_HAS_PROCESS(if_stage);	

 private:
  sc_signal<int> _pc;
  /* Trace File */	
  sc_trace_file *_tcf;

  /* Instruction Memory (IM) */
  std::vector<sc_lv<W_SIZE> > _memory;
};

void if_stage::instructions_output()
{
  int _index_pc = 0;
  _index_pc = _pc.read() / 4;
  instruction_0.write( _memory[_index_pc] );
  instruction_1.write( _memory[_index_pc + 1] );
  npc.write(_pc.read() + 8);
}

void if_stage::process() 
{
  cout << "Entering the IF Process" << endl;
  
  if ( rst.read() == SC_LOGIC_1 ) 
    {
      /*         _pc = 0; */
      _pc.write(0);
    } 
  else 
    {
      if ( (pc_we == SC_LOGIC_1) && (jump_or_branch == SC_LOGIC_1) ) 
        {
	  _pc.write(address_after_jump_or_branch.read().to_int());
        }	

      else if ( clk.event() )
	{	  
	  if ( pc_we == SC_LOGIC_1) 
	    {
	      _pc.write(_pc.read() + 8);
	    }
	}
    }
  cout << "Exiting the IF Process" << endl;
}

void if_stage::load_memory(const char* filename) 
{
  std::string instruction;
  ifstream memory(filename);
  int address = 0;
  if(memory.is_open()) 
    {
      while(!memory.eof()) 
        {
	  getline(memory,instruction );
	  if(instruction.size() > 0) 
            {
	      _memory[address++] = instruction.c_str();
            }
        }
      /* Fill the remains of the memory with zeros */
      for ( address; address < _memory.size(); address++ ) 
        {
	  //std::cout<<"Mem addr: "<<address<<std::endl;
	  _memory[address] = 0;
        } 

    } 
  else 
    {
      std::cerr << "Cannot open memory file: " << filename << std::endl;		  
      exit(-1);
    }
}

#endif
