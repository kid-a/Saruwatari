/*
 * alu.h    
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

#ifndef ALU_H
#define ALU_H

#include <systemc.h>

#define W_SIZE 32

enum alu_state_t 
  {
    idle,
    computing
  };

class alu : sc_module
{
 public:
  sc_in<bool> clk;
  sc_in<sc_logic> rst;
    
  sc_in<sc_lv<W_SIZE> > a;
  sc_in<sc_lv<W_SIZE> > b;
  sc_in<sc_lv<4> > op;

  sc_out<sc_logic> data_ready;
  sc_out<sc_lv<W_SIZE> > result;

  sc_signal<alu_state_t> pr_state;
  sc_signal<alu_state_t> nx_state;

  void update_state();
  void ns_op_logic();

  SC_CTOR(alu)
  {
    SC_METHOD(update_state);
    sensitive << clk.pos() << rst;
      
    SC_METHOD(ns_op_logic);
    sensitive << pr_state << a << b << op;
    dont_initialize();

    computing_trigger_activated = 0;
  }

 private:
  int computing_trigger_activated;
};


void alu::update_state()
{
  if (rst == SC_LOGIC_1) 
    {
      pr_state.write(idle);
    }
  else 
    {
      pr_state.write(nx_state.read());
    }
}

void alu::ns_op_logic()
{
  int opcode = op.read().to_int();

  if (rst == SC_LOGIC_1)
    {
      data_ready.write(SC_LOGIC_1);
      result.write(0);
      nx_state.write(idle);
    }
    
  else
    {
      switch (pr_state)
        {
	case idle:
	  switch(opcode)
	    {
	    case 0b0000: //nop
	      std::cout << "ALU: nop - idle " << std::endl; 
	      result.write(0);
	      data_ready.write(SC_LOGIC_1);
	      nx_state.write(idle);
	      break;

	    case 0b0001: //sum
	      std::cout << "ALU: sum - idle " << std::endl; 
	      result.write( a.read().to_int() + b.read().to_int() );
	      data_ready.write(SC_LOGIC_1);
	      nx_state.write(idle);	  
	      break;
	                
	    case 0b0010: //sub
	      std::cout << "ALU: sub - idle " << std::endl; 
	      result.write( a.read().to_int() - b.read().to_int() );
	      data_ready.write(SC_LOGIC_1);
	      nx_state.write(idle);
	      break;

	    case 0b0011: //mult - 2 cycles
	      if ( a.event() || b.event() || op.event() )
		{
		  std::cout << "ALU: mult - idle " << std::endl; 
		  data_ready.write(SC_LOGIC_0);
		  nx_state.write(computing);
		}
	      break;

	    case 0b0100: //div - 2 cycles
	      if ( a.event() || b.event() || op.event() )
		{
		  std::cout << "ALU: div - idle " << std::endl; 
		  data_ready.write(SC_LOGIC_0);
		  nx_state.write(computing);
		}
	      break;

	    case 0b0101: //slt
	      std::cout << "ALU: slt - idle " << std::endl; 
	      data_ready.write(SC_LOGIC_1);
	      std::cout<<"Testing if: "<<a.read().to_int()<<" is less than "<<b.read().to_int()<<std::endl;
	      if ( a.read().to_int() < b.read().to_int() )
		{
		  result.write(1);
		}
	      else
		{
		  result.write(0);
		}
	      nx_state.write(idle);
	      break;

	    case 0b0110: //sgt
	      std::cout << "ALU: sgt - idle " << std::endl; 
	      data_ready.write(SC_LOGIC_1);
	      if ( a.read().to_int() > b.read().to_int() )
		{
		  result.write(1);
		}
	      else
		{
		  result.write(0);
		}
	      nx_state.write(idle);
	      break;

	    case 0b0111: //addres
	      if ( a.event() || b.event() || op.event() )
		{
		  std::cout << "ALU: lw/sw - idle " << std::endl; 
		  data_ready.write(SC_LOGIC_0);
		  nx_state.write(computing);
		}
	      break;

	    default:
	      std::cout <<  "ALU: ERROR - Invalid opcode." << std::endl;
	      break;
	    } // end opcode
	  break;

	case computing:
	  switch (opcode)
	    {
	    case 0b0011: //mult
	      std::cout << "ALU: Mult - computing " << std::endl; 
	      if (computing_trigger_activated == 0)
		{
		  computing_trigger_activated = 1;
		  next_trigger(6, SC_NS);
		}
	      else
		{
		  computing_trigger_activated = 0;
		  data_ready.write(SC_LOGIC_1);
		  result.write( a.read().to_int() * b.read().to_int() );
		  std::cout<<"Mult: "<<a.read().to_int()<<"*"<<b.read().to_int()<<"= "<<result.read().to_int()<<std::endl;
		  nx_state.write(idle);
		}
	      break;

	    case 0b0100: //div
	      std::cout << "ALU: div - computing " << std::endl; 
	      if (computing_trigger_activated == 0)
		{
		  computing_trigger_activated = 1;
		  next_trigger(6, SC_NS);
		}
	      else
		{
		  computing_trigger_activated = 0;
		  data_ready.write(SC_LOGIC_1);
		  result.write( a.read().to_int() / b.read().to_int() );
		  std::cout<<"Div: "<<a.read().to_int()<<"/"<<b.read().to_int()<<"= "<<result.read().to_int()<<std::endl;
		  nx_state.write(idle);
		}
	      break;

	    case 0b0111: //address_calculation
	      std::cout << "ALU: lw/sw - computing " << std::endl; 
	      if (computing_trigger_activated == 0)
		{
		  computing_trigger_activated = 1;
		  next_trigger(6, SC_NS);
		}
	      else
		{
		  computing_trigger_activated = 0;
		  data_ready.write(SC_LOGIC_1);
		  result.write( a.read().to_int() +  b.read().to_int() );
		  nx_state.write(idle);
		}
	      break;

	    default:
	      std::cout << "ALU: " << opcode <<" - computing " << std::endl; 
	      computing_trigger_activated = 0;

	    } // end opcode
	  break; // end computing
      
	  /* Invalid State */
	default:
	  std::cout << "ALU: ERROR - Invalid state encountered." << std::endl;
	  break;
        } //end opcode
    }//end state
}

#endif
