/*
 * exmem_stage.h    
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

#ifndef EXMEM_STAGE
#define EXMEM_STAGE

#include <systemc.h>
#include <alu.h>

#define W_SIZE 32

class exmem_stage : sc_module
{
  public:
    /* Clock and Reset */
    sc_in<bool> clk;
    sc_in<sc_logic> rst;
    /* Inputs from ID/EXMEM latch */
    sc_in<sc_lv<W_SIZE> > npc_in;
    sc_in<sc_lv<W_SIZE> > ir_1_in;
    sc_in<sc_lv<W_SIZE> > ir_0_in;
    sc_in<sc_lv<W_SIZE> > A1;
    sc_in<sc_lv<W_SIZE> > B1;
    sc_in<sc_lv<W_SIZE> > A0;
    sc_in<sc_lv<W_SIZE> > B0;
    sc_in<sc_lv<W_SIZE> > immediate_1;
    sc_in<sc_lv<W_SIZE> > immediate_0;
    sc_in<sc_logic> alu_src_1;
    sc_in<sc_logic> alu_src_0;
    sc_in<sc_logic> reg_dst_1;
    sc_in<sc_logic> reg_dst_0;
    sc_in<sc_logic> jal_in;
    sc_in<sc_logic> mem_read_in;
    sc_in<sc_logic> mem_write_in;
    sc_in<sc_logic> wb_enable_1_in;
    sc_in<sc_logic> wb_enable_0_in;
    
    /* Outputs to Hazard Detection Unit (HDU) */
    sc_out<sc_logic> alu_1_data_ready;
    sc_out<sc_logic> alu_0_data_ready;
    /* Outputs to Data Memory (DM) */
    sc_out<sc_lv<W_SIZE> > B0_out;
    /* Datapath outputs */
    sc_out<sc_lv<W_SIZE> > npc_out;
    sc_out<sc_lv<5> > dst_reg_1;
    sc_out<sc_lv<5> > dst_reg_0;
    sc_out<sc_lv<W_SIZE> > alu_out_1;  
    sc_out<sc_lv<W_SIZE> > alu_out_0;

    sc_out<sc_logic> jal_out;
    sc_out<sc_logic> mem_read_out;
    sc_out<sc_logic> mem_write_out; 
    sc_out<sc_logic> wb_enable_1_out;
    sc_out<sc_logic> wb_enable_0_out;

    /* Processes */
    void process();
    void mux_alu_src_1();
    void mux_alu_src_0();
    void mux_dst_reg_1();
    void mux_dst_reg_0();
    void alu_out_process();
    void outputs_process();


    exmem_stage(sc_module_name _name, sc_trace_file* tcf, bool trace_enabled) : sc_module(_name), _tcf(tcf)
     {
      SC_METHOD(process);
        sensitive << npc_in << ir_1_in << ir_0_in;
        // dont_initialize();

      SC_METHOD(mux_alu_src_1);
        sensitive << alu_src_1 << B1 << immediate_1;

      SC_METHOD(mux_alu_src_0);
        sensitive << alu_src_0 << B0 << immediate_0;

      SC_METHOD(mux_dst_reg_1);
        sensitive << reg_dst_1 << ir_1_in;
        dont_initialize();

      SC_METHOD(mux_dst_reg_0);
        sensitive << reg_dst_0 << ir_0_in;
        dont_initialize();

      SC_METHOD(alu_out_process);
        sensitive << alu_1_out_internal 
      	<< alu_0_out_internal;

      SC_METHOD(outputs_process);
        sensitive << npc_internal << B0_internal
      	<< jal_internal 
      	<< wb_enable_1_internal
      	<< wb_enable_0_internal
      	<< mem_read_internal
      	<< mem_write_internal;

      alu_1 = new alu("alu_1");
      alu_0 = new alu("alu_0");
      /* port map is incomplete */
      alu_1->clk(clk);
      alu_1->rst(rst);
      alu_1->a(A1);
      alu_1->b(alu_1_operand_b);
      alu_1->op(alu_1_opcode);
      alu_1->data_ready(alu_1_data_ready);
      alu_1->result(alu_1_out_internal);

      alu_0->clk(clk);
      alu_0->rst(rst);
      alu_0->a(A0);
      alu_0->b(alu_0_operand_b);
      alu_0->op(alu_0_opcode);
      alu_0->data_ready(alu_0_data_ready);
      alu_0->result(alu_0_out_internal);

      if ( trace_enabled)
      {
        sc_trace(_tcf,alu_out_1,"3a_EXMEM_alu_out_1");
        sc_trace(_tcf,alu_out_0,"3a_EXMEM_alu_out_0");
        sc_trace(_tcf,alu_src_1,"3a_EXMEM_alu_src_1");
        sc_trace(_tcf,alu_src_0,"3a_EXMEM_alu_src_0");
	
	sc_trace(_tcf,alu_1_data_ready,"AAA_ALU_1_DATA_READY");
	sc_trace(_tcf,alu_0_data_ready,"AAA_ALU_0_DATA_READY");

        sc_trace(_tcf,immediate_1,"3a_EXMEM_immediate_1");
        sc_trace(_tcf,immediate_0,"3a_EXMEM_immediate_0");

	sc_trace(_tcf, alu_1_opcode, "BBB_alu_1_opcode");
	sc_trace(_tcf, alu_0_opcode, "BBB_alu_0_opcode");

        sc_trace(_tcf,alu_1_operand_b,"3a_EXMEM_alu_1_operand_b");
	

      }
    } // End SC_CTOR

    SC_HAS_PROCESS(exmem_stage);
  private:
    sc_trace_file *_tcf;
    /* Internal signals */
    sc_signal<sc_lv<W_SIZE> > npc_internal;
    sc_signal<sc_lv<W_SIZE> > B0_internal;
    sc_signal<sc_logic> jal_internal;
    sc_signal<sc_logic> wb_enable_1_internal;
    sc_signal<sc_logic> wb_enable_0_internal;
    sc_signal<sc_logic> mem_read_internal;
    sc_signal<sc_logic> mem_write_internal;
    
    alu *alu_1;
    alu *alu_0;
    /* alu-related internal signals */
    sc_signal<sc_lv<W_SIZE> > alu_1_operand_b;
    sc_signal<sc_lv<W_SIZE> > alu_0_operand_b;
    sc_signal<sc_lv<4> > alu_1_opcode;
    sc_signal<sc_lv<4> > alu_0_opcode;
    sc_signal<sc_lv<W_SIZE> > alu_1_out_internal;
    sc_signal<sc_lv<W_SIZE> > alu_0_out_internal;
};


void exmem_stage::outputs_process()
{
        npc_out.write(npc_internal.read());
        B0_out.write(B0_internal.read());
        jal_out.write(jal_internal.read());
        wb_enable_1_out.write(wb_enable_1_internal.read());
        wb_enable_0_out.write(wb_enable_0_internal.read());
        mem_read_out.write(mem_read_internal.read());
        mem_write_out.write(mem_write_internal.read());
}

void exmem_stage::alu_out_process()
{
        alu_out_1.write(alu_1_out_internal.read());
        alu_out_0.write(alu_0_out_internal.read());
}

void exmem_stage::mux_alu_src_1()
{
        if ( alu_src_1.read() == SC_LOGIC_0 )
        {
	        alu_1_operand_b.write(immediate_1.read());
        }
        else
        {
           //std::cout<<"Passing: "<<B1.read()<<std::endl; 
	        alu_1_operand_b.write(B1.read());
        }
}

void exmem_stage::mux_alu_src_0()
{
        if ( alu_src_0.read() == SC_LOGIC_0 ) 
        {
	        alu_0_operand_b.write(immediate_0.read());
        } 
        else
        {
	        alu_0_operand_b.write(B0.read());
        }
}

void exmem_stage::mux_dst_reg_1()
{
        if ( reg_dst_1.read() == SC_LOGIC_0 )
        {
	        dst_reg_1.write(ir_1_in.read().range(20, 16));
        }
        else
        {
	        dst_reg_1.write(ir_1_in.read().range(15, 11));
        }
}

void exmem_stage::mux_dst_reg_0()
{
        if ( reg_dst_0.read() == SC_LOGIC_0 )
        {
	        dst_reg_0.write(ir_0_in.read().range(20, 16));
        }
        else
        {
	        dst_reg_0.write(ir_0_in.read().range(15, 11));
        }
}

void exmem_stage::process()
{
    if (rst.read() == SC_LOGIC_1)
    {
        //std::cout << "Reset in EXMEM_STAGE" << std::endl;
        npc_internal.write(0);
        B0_internal.write(0);
        jal_internal.write(SC_LOGIC_0);
        wb_enable_1_internal.write(SC_LOGIC_0);
        wb_enable_0_internal.write(SC_LOGIC_0);
        mem_read_internal.write(SC_LOGIC_0);
        mem_write_internal.write(SC_LOGIC_0);
        alu_0_opcode.write(0);
        alu_1_opcode.write(0);
    }
    else
    {
        //std::cout << "Not reset in EXMEM_STAGE" << std::endl;
        npc_internal.write(npc_in.read());
        B0_internal.write(B0.read());
        jal_internal.write(jal_in.read());
        mem_read_internal.write(mem_read_in.read());
        mem_write_internal.write(mem_write_in.read());
        wb_enable_1_internal.write(wb_enable_1_in.read());
        wb_enable_0_internal.write(wb_enable_0_in.read());
             
        /* alu controller 1 */
        switch (ir_1_in.read().range(31, 30).to_int())
        {
            /* I-type */
            case 0b00:
                switch (ir_1_in.read().range(29, 26).to_int())
                {
                     case 0b0000:  //nop
                        alu_1_opcode.write(0b0000);
                        break;

                     case 0b0001: // addi
                        alu_1_opcode.write(0b0001);
                        break;
            
                    case 0b0010: // subi
                        alu_1_opcode.write(0b0010);
                        break;
            
                    case 0b0101: // beqz
                        alu_1_opcode.write(0b0001);
                        break;

                    case 0b0110: // bnqz
                        alu_1_opcode.write(0b0001);
                        break;	      

                    case 0b0111: //multi
                        alu_1_opcode.write(0b0011);
                        break;

                    default: //not valid
                      alu_1_opcode.write(0b0000);
                      std::cout << "(EXMEM stage) Warning: Invalid I-type " 
      	              << "instruction in alu_controller_1." 
      	              << std::endl;
                      break;
                } // end switch I-type
                break; // end I-type

            /* R-type */
            case 0b01:
                switch (ir_1_in.read().range(10, 0).to_int())
                {
                    case 0b00000000001: // add
                        alu_1_opcode.write(0b0001);
                        break;

                    case 0b00000000010: // sub
                        alu_1_opcode.write(0b0010);
                        break;

                    case 0b00000000011: // mult
                        alu_1_opcode.write(0b0011);
                        break;

                    case 0b00000000100: // slt
                        alu_1_opcode.write(0b0101);
                        break;

                    case 0b00000000101: // sgt
                        alu_1_opcode.write(0b0110);
                        break;

                    case 0b00000000110: // div
                        alu_1_opcode.write(0b0100);
                        break;

                    default: //not valid
                        alu_1_opcode.write(0b0000);
                        std::cout << "(EXMEM stage) Warning: Invalid R-type " 
            	    	  << "instruction in alu_controller_1." 
            	    	  << std::endl;	      
                    } // end switch R-type
                    break; // end R-type

            /* J-type */      
            case 0b10:
                switch (ir_1_in.read().range(29, 26).to_int())
                {
                    case 0b0000: // jump
                        alu_1_opcode.write(0b0000);
                        break;
                      
                    case 0b0001: // jal
                        alu_1_opcode.write(0b0001);
                        break;

                    default: //not valid
                        std::cout << "(EXMEM stage) Warning: Invalid J-type " 
      	                  << "instruction in alu_controller_1." 
      	                  << std::endl;
                        break;
                } // end switch J-type
                break; // end J-type

            /* Invalid Instruction */    
            default:
                std::cout << "(EXMEM stage) ERROR: Invalid instruction " 
      	          << "in alu_controller_1." 
      	          << std::endl;	      	  
                break;
        } // end alu controller 1


        /* alu controller 0 */
        switch (ir_0_in.read().range(31, 30).to_int())
        {
            /* I-type */
            case 0b00:
                switch (ir_0_in.read().range(29, 26).to_int())
                {
                    case 0b0000:  //nop
                        alu_0_opcode.write(0b0000);
                        break;
                        
                    case 0b0001: // addi
                        alu_0_opcode.write(0b0001);
                        break;
                    
                    case 0b0010: // subi
                        alu_0_opcode.write(0b0010);
                        break;

                    case 0b0011: // lw
                        alu_0_opcode.write(0b0111);
                        break;

                    case 0b100: // sw
                        alu_0_opcode.write(0b0111);
                        break;
                      
                    case 0b0111: //multi
                        alu_0_opcode.write(0b0011);
                        break;

                    default:
                        alu_0_opcode.write(0b0000);
                        std::cout << "(EXMEM stage) Warning: Invalid I-type "
                    	  << "instruction in alu_controller_0." 
                    	  << std::endl;
                        break;
                } // end switch I-type
                break; //end I-type

            /* R-type */
            case 0b01:
                switch (ir_0_in.read().range(10, 0).to_int())
                {
                    case 0b00000000001: // add
                        alu_0_opcode.write(0b0001);
                        break;

                    case 0b00000000010: // sub
                        alu_0_opcode.write(0b0010);
                        break;

                    case 0b00000000011: // mult
                        alu_0_opcode.write(0b0011);
                        break;

                    case 0b00000000100: // slt
                        alu_0_opcode.write(0b0101);
                        break;

                    case 0b00000000101: // sgt
                        alu_0_opcode.write(0b0110);
                        break;

                    case 0b00000000110: // div
                        alu_0_opcode.write(0b0100);
                        break;

                    default:
                        alu_0_opcode.write(0b0000);
                        std::cout << "(EXMEM stage) Warning: Invalid R-type "
            	    	  << "instruction in alu_controller_0." 
            	    	  << std::endl;	      
                        break;
                } // end switch R-type
                break; // end R-type

                /* Invalid Instruction */
                default:
                    std::cout << "(EXMEM stage) ERROR: Invalid instruction "
            	      << "in alu_controller_0."
            	      << std::endl;
                    break;
            } // end switch alu controller 0
      } // end alu controller 0
}

#endif
