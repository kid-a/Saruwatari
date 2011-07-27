/*
 * pipeline.cc    
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

#include <systemc.h>

/* Pipeline */
#include <if_stage.h>
#include <if_id_latch.h>
#include <id_stage.h>
#include <id_exmem_latch.h>
#include <exmem_stage.h>
#include <exmem_wb_latch.h>
#include <wb_stage.h>

/* Components */
#include <control_unit.h>
#include <hazard_detection_unit.h>
#include <ram.h>

#define W_SIZE 32

int sc_main(int, char * argv[]) 
{
    sc_trace_file *tcf = sc_create_vcd_trace_file("wave");
    if(tcf == NULL)
    {
        cout << "An error has occurred while opening trace file" << endl;
        exit(-1);
    }

    tcf->set_time_unit(100, SC_PS);
   

    /**
     *
     *  Signals
     *
     */

    /* Global Clock & Reset */ 
    sc_clock clk("clk",10,SC_NS);
    sc_signal<sc_logic> rst;

    /* Inputs IF/ID Latch */
    sc_signal<sc_logic> if_id_hdu_rst;
    sc_signal<sc_logic> if_id_we;
    sc_signal<sc_lv<W_SIZE> > if_id_npc_in;
    sc_signal<sc_lv<W_SIZE> > if_id_ir_1_in;
    sc_signal<sc_lv<W_SIZE> > if_id_ir_0_in;
    /* Outputs  IF/ID Latch */
    sc_signal<sc_lv<W_SIZE> > if_id_npc_out;
    sc_signal<sc_lv<W_SIZE> > if_id_ir_1_out;
    sc_signal<sc_lv<W_SIZE> > if_id_ir_0_out;

    /** Inputs ID */
    /* From CU */
    sc_signal<sc_logic> id_branch_taken_in;
    sc_signal<sc_logic> id_jump_in;
    sc_signal<sc_logic> id_jal_in;
    /* From WB */
    sc_signal<sc_logic> reg_file_write_enable_1;
    sc_signal<sc_lv<5> > reg_file_write_address_1;
    sc_signal<sc_lv<W_SIZE> > reg_file_write_data_1;
    sc_signal<sc_logic> reg_file_write_enable_0;
    sc_signal<sc_lv<5> > reg_file_write_address_0;
    sc_signal<sc_lv<W_SIZE> > reg_file_write_data_0;
    /** Outputs ID */
    /* To IF */
    sc_signal<sc_logic> jump_or_branch;
    sc_signal<sc_lv<W_SIZE> > address_after_jump_or_branch;

    /** Inputs ID/EXMEM */
    /* From ID */
    sc_signal<sc_logic> id_exmem_hdu_rst;
    sc_signal<sc_logic> id_exmem_we;
    sc_signal<sc_lv<W_SIZE> > id_exmem_npc_in;
    sc_signal<sc_lv<W_SIZE> > id_exmem_ir_1_in;
    sc_signal<sc_lv<W_SIZE> > id_exmem_ir_0_in;
    sc_signal<sc_lv<W_SIZE> > id_exmem_A0_in;
    sc_signal<sc_lv<W_SIZE> > id_exmem_B0_in;
    sc_signal<sc_lv<W_SIZE> > id_exmem_A1_in;
    sc_signal<sc_lv<W_SIZE> > id_exmem_B1_in;
    sc_signal<sc_lv<W_SIZE> > id_exmem_immediate_1_in;
    sc_signal<sc_lv<W_SIZE> > id_exmem_immediate_0_in;
    /* From CU */
    sc_signal<sc_logic> id_exmem_alu_src_1_in;
    sc_signal<sc_logic> id_exmem_alu_src_0_in;
    sc_signal<sc_logic> id_exmem_reg_dst_1_in;
    sc_signal<sc_logic> id_exmem_reg_dst_0_in;
    sc_signal<sc_logic> id_exmem_jal_in;
    sc_signal<sc_logic> id_exmem_mem_read_in;
    sc_signal<sc_logic> id_exmem_mem_write_in;
    sc_signal<sc_logic> id_exmem_wb_enable_1_in;
    sc_signal<sc_logic> id_exmem_wb_enable_0_in;
    /* Outputs ID/EXMEM */
    sc_signal<sc_lv<W_SIZE> > id_exmem_npc_out;
    sc_signal<sc_lv<W_SIZE> > id_exmem_ir_1_out;
    sc_signal<sc_lv<W_SIZE> > id_exmem_ir_0_out;
    sc_signal<sc_lv<W_SIZE> > id_exmem_A0_out;
    sc_signal<sc_lv<W_SIZE> > id_exmem_B0_out;
    sc_signal<sc_lv<W_SIZE> > id_exmem_A1_out;
    sc_signal<sc_lv<W_SIZE> > id_exmem_B1_out;
    sc_signal<sc_lv<W_SIZE> > id_exmem_immediate_1_out;
    sc_signal<sc_lv<W_SIZE> > id_exmem_immediate_0_out;
    sc_signal<sc_logic> id_exmem_alu_src_1_out;
    sc_signal<sc_logic> id_exmem_alu_src_0_out;
    sc_signal<sc_logic> id_exmem_reg_dst_1_out;
    sc_signal<sc_logic> id_exmem_reg_dst_0_out;
    sc_signal<sc_logic> id_exmem_jal_out;
    sc_signal<sc_logic> id_exmem_mem_read_out;
    sc_signal<sc_logic> id_exmem_mem_write_out;
    sc_signal<sc_logic> id_exmem_wb_enable_1_out;
    sc_signal<sc_logic> id_exmem_wb_enable_0_out;

    /* Inputs EXMEM/WB */
    sc_signal<sc_logic> exmem_wb_we;
    sc_signal<sc_lv<W_SIZE> > exmem_wb_npc_in;
    sc_signal<sc_lv<W_SIZE> > exmem_wb_alu_out_1_in;
    sc_signal<sc_lv<5> > exmem_wb_dst_reg_1_in;
    sc_signal<sc_lv<W_SIZE> > exmem_wb_alu_out_0_in;
    sc_signal<sc_lv<W_SIZE> > exmem_wb_lmd_in;
    sc_signal<sc_lv<5> > exmem_wb_dst_reg_0_in;
    sc_signal<sc_logic > exmem_wb_jal_in;
    sc_signal<sc_logic > exmem_wb_mem_read_in;
    sc_signal<sc_logic > exmem_wb_wb_enable_1_in;
    sc_signal<sc_logic > exmem_wb_wb_enable_0_in;
    /* Outputs EXMEM/WB */
    sc_signal<sc_lv<W_SIZE> > exmem_wb_npc_out;
    sc_signal<sc_lv<W_SIZE> > exmem_wb_alu_out_1_out;
    sc_signal<sc_lv<5> > exmem_wb_dst_reg_1_out;
    sc_signal<sc_lv<5> > exmem_wb_dst_reg_jal_out;
    sc_signal<sc_lv<W_SIZE> > exmem_wb_alu_out_0_out;
    sc_signal<sc_lv<W_SIZE> > exmem_wb_lmd_out;
    sc_signal<sc_lv<5> > exmem_wb_dst_reg_0_out;
    sc_signal<sc_logic > exmem_wb_jal_out;
    sc_signal<sc_logic > exmem_wb_mem_read_out;
    sc_signal<sc_logic > exmem_wb_wb_enable_1_out;
    sc_signal<sc_logic > exmem_wb_wb_enable_0_out;
  

    /* data memory input signals */
    sc_signal<sc_lv<W_SIZE> > dm_data_in;
    sc_signal<sc_logic> dm_we;
    /* other signals */
    /* temporary declaration */
    sc_signal<sc_logic> pc_we; // program counter write enable
    //    sc_signal<sc_lv<32> > id_branch_operand;
    sc_signal<sc_logic> cu_branch_taken;
    sc_signal<sc_logic> cu_jump;
    sc_signal<sc_logic> cu_jal;
    sc_signal<sc_logic> hdu_alu_1_data_ready;
    sc_signal<sc_logic> hdu_alu_0_data_ready;
    
    // components instantiation
    if_stage if_s("if_stage", tcf,true);
    if_id_latch if_id_l("if_id_latch",tcf,true);
    id_stage id_s("id_stage",tcf,true);
    id_exmem_latch id_exmem_l("id_exmem_latch",tcf,true);
    exmem_stage em_s("exmem_stage",tcf,true);
    exmem_wb_latch exmem_wb_l("exmem_wb_latch",tcf,true);
    wb_stage wb_s("wb_stage",tcf,true);

    ram<> data_memory("data_memory",tcf,true);
    control_unit cu("control_unit");
    hazard_detection_unit hdu("hazard_detection_unit", tcf, true);

    /**
     *
     * PORT MAP
     *
     */
    
    /* IF Stage */
    if_s.clk(clk);
    if_s.rst(rst);
    if_s.pc_we(pc_we);
    if_s.jump_or_branch(jump_or_branch);
    if_s.address_after_jump_or_branch(address_after_jump_or_branch);
    if_s.npc(if_id_npc_in);
    if_s.instruction_1(if_id_ir_1_in);
    if_s.instruction_0(if_id_ir_0_in);

    /* IF/ID Latch */
    if_id_l.clk(clk);
    if_id_l.rst(rst);
    if_id_l.hdu_rst(if_id_hdu_rst);
    if_id_l.we(if_id_we);
    if_id_l.npc_in(if_id_npc_in);
    if_id_l.ir_1_in(if_id_ir_1_in);
    if_id_l.ir_0_in(if_id_ir_0_in);
    if_id_l.npc_out(if_id_npc_out);
    if_id_l.ir_1_out(if_id_ir_1_out);
    if_id_l.ir_0_out(if_id_ir_0_out);

    /* ID Stage */
    id_s.clk(clk);
    id_s.rst(rst);
    id_s.npc_in(if_id_npc_out);
    id_s.ir_1_in(if_id_ir_1_out);
    id_s.ir_0_in(if_id_ir_0_out);
    id_s.reg_file_write_enable_1(reg_file_write_enable_1);
    id_s.reg_file_write_address_1(reg_file_write_address_1);
    id_s.reg_file_write_data_1(reg_file_write_data_1);
    id_s.reg_file_write_enable_0(reg_file_write_enable_0);
    id_s.reg_file_write_address_0(reg_file_write_address_0);
    id_s.reg_file_write_data_0(reg_file_write_data_0);
    id_s.branch_taken(cu_branch_taken);
    id_s.jump(cu_jump);
    id_s.jal(cu_jal);
    //    id_s.branch_operand(id_branch_operand);
    id_s.address_after_jump_or_branch(address_after_jump_or_branch);
    id_s.jump_or_branch(jump_or_branch);
    id_s.npc_out(id_exmem_npc_in);
    id_s.ir_1_out(id_exmem_ir_1_in);
    id_s.ir_0_out(id_exmem_ir_0_in);
    id_s.immediate_1(id_exmem_immediate_1_in);
    id_s.immediate_0(id_exmem_immediate_0_in);
    id_s.A1(id_exmem_A1_in);
    id_s.B1(id_exmem_B1_in);
    id_s.A0(id_exmem_A0_in);
    id_s.B0(id_exmem_B0_in);

    /* CONTROL UNIT */
    cu.ir_1(if_id_ir_1_out);
    cu.ir_0(if_id_ir_0_out);
    cu.branch_operand(id_exmem_A1_in);
    cu.alu_src_1(id_exmem_alu_src_1_in);
    cu.alu_src_0(id_exmem_alu_src_0_in);
    cu.reg_dst_1(id_exmem_reg_dst_1_in);
    cu.reg_dst_0(id_exmem_reg_dst_0_in);
    cu.jump(cu_jump);
    cu.jal(cu_jal);
    cu.branch_taken(cu_branch_taken);
    cu.mem_read(id_exmem_mem_read_in);
    cu.mem_write(id_exmem_mem_write_in);
    cu.wb_enable_1(id_exmem_wb_enable_1_in);
    cu.wb_enable_0(id_exmem_wb_enable_0_in);

    /* ID/EXMEM Latch */
    id_exmem_l.clk(clk);
    id_exmem_l.rst(rst);
    id_exmem_l.hdu_rst(id_exmem_hdu_rst);
    id_exmem_l.we(id_exmem_we);
    id_exmem_l.npc_in(id_exmem_npc_in);
    id_exmem_l.ir_1_in(id_exmem_ir_1_in);
    id_exmem_l.ir_0_in(id_exmem_ir_0_in);
    id_exmem_l.A0_in(id_exmem_A0_in);
    id_exmem_l.B0_in(id_exmem_B0_in);
    id_exmem_l.A1_in(id_exmem_A1_in);
    id_exmem_l.B1_in(id_exmem_B1_in);
    id_exmem_l.immediate_1_in(id_exmem_immediate_1_in);
    id_exmem_l.immediate_0_in(id_exmem_immediate_0_in);
    id_exmem_l.alu_src_1_in(id_exmem_alu_src_1_in);
    id_exmem_l.alu_src_0_in(id_exmem_alu_src_0_in);
    id_exmem_l.reg_dst_1_in(id_exmem_reg_dst_1_in);
    id_exmem_l.reg_dst_0_in(id_exmem_reg_dst_0_in);
    //  id_exmem_l.jal_in(id_exmem_jal_in);
    id_exmem_l.jal_in(cu_jal);
    id_exmem_l.mem_read_in(id_exmem_mem_read_in);
    id_exmem_l.mem_write_in(id_exmem_mem_write_in);
    id_exmem_l.wb_enable_1_in(id_exmem_wb_enable_1_in);
    id_exmem_l.wb_enable_0_in(id_exmem_wb_enable_0_in);
    id_exmem_l.npc_out(id_exmem_npc_out);
    id_exmem_l.ir_1_out(id_exmem_ir_1_out);
    id_exmem_l.ir_0_out(id_exmem_ir_0_out);
    id_exmem_l.A0_out(id_exmem_A0_out);
    id_exmem_l.B0_out(id_exmem_B0_out);
    id_exmem_l.A1_out(id_exmem_A1_out);
    id_exmem_l.B1_out(id_exmem_B1_out);
    id_exmem_l.immediate_1_out(id_exmem_immediate_1_out);
    id_exmem_l.immediate_0_out(id_exmem_immediate_0_out);
    id_exmem_l.alu_src_1_out(id_exmem_alu_src_1_out);
    id_exmem_l.alu_src_0_out(id_exmem_alu_src_0_out);
    id_exmem_l.reg_dst_1_out(id_exmem_reg_dst_1_out);
    id_exmem_l.reg_dst_0_out(id_exmem_reg_dst_0_out);
    id_exmem_l.jal_out(id_exmem_jal_out);
    id_exmem_l.mem_read_out(id_exmem_mem_read_out);
    id_exmem_l.mem_write_out(id_exmem_mem_write_out);
    id_exmem_l.wb_enable_1_out(id_exmem_wb_enable_1_out);
    id_exmem_l.wb_enable_0_out(id_exmem_wb_enable_0_out);

    /* EXMEM Stage */
    em_s.clk(clk);
    em_s.rst(rst);
    em_s.npc_in(id_exmem_npc_out);
    em_s.ir_1_in(id_exmem_ir_1_out);
    em_s.ir_0_in(id_exmem_ir_0_out);
    em_s.A1(id_exmem_A1_out);
    em_s.B1(id_exmem_B1_out);
    em_s.A0(id_exmem_A0_out);
    em_s.B0(id_exmem_B0_out);
    em_s.immediate_1(id_exmem_immediate_1_out);
    em_s.immediate_0(id_exmem_immediate_0_out);
    em_s.alu_src_1(id_exmem_alu_src_1_out);
    em_s.alu_src_0(id_exmem_alu_src_0_out);
    em_s.reg_dst_1(id_exmem_reg_dst_1_out);
    em_s.reg_dst_0(id_exmem_reg_dst_0_out);
    em_s.jal_in(id_exmem_jal_out);
    em_s.mem_read_in(id_exmem_mem_read_out);
    em_s.mem_write_in(id_exmem_mem_write_out);
    em_s.wb_enable_1_in(id_exmem_wb_enable_1_out);
    em_s.wb_enable_0_in(id_exmem_wb_enable_0_out);
    em_s.alu_1_data_ready(hdu_alu_1_data_ready);
    em_s.alu_0_data_ready(hdu_alu_0_data_ready);
    em_s.B0_out(dm_data_in);
    em_s.npc_out(exmem_wb_npc_in);
    em_s.dst_reg_1(exmem_wb_dst_reg_1_in);
    em_s.dst_reg_0(exmem_wb_dst_reg_0_in);
    em_s.alu_out_1(exmem_wb_alu_out_1_in);
    em_s.alu_out_0(exmem_wb_alu_out_0_in);
    em_s.jal_out(exmem_wb_jal_in);
    em_s.mem_read_out(exmem_wb_mem_read_in);
    em_s.mem_write_out(dm_we);
    em_s.wb_enable_1_out(exmem_wb_wb_enable_1_in);
    em_s.wb_enable_0_out(exmem_wb_wb_enable_0_in);

    data_memory.clk(clk);
    data_memory.write_enable(dm_we);
    data_memory.addr(exmem_wb_alu_out_0_in);
    data_memory.data_in(dm_data_in);
    data_memory.data_out(exmem_wb_lmd_in);
    
    /* EXMEM/WB Latch */
    exmem_wb_l.npc_in(exmem_wb_npc_in);
    exmem_wb_l.alu_out_1_in(exmem_wb_alu_out_1_in);
    exmem_wb_l.dst_reg_1_in(exmem_wb_dst_reg_1_in);
    exmem_wb_l.alu_out_0_in(exmem_wb_alu_out_0_in);
    exmem_wb_l.lmd_in(exmem_wb_lmd_in);
    exmem_wb_l.dst_reg_0_in(exmem_wb_dst_reg_0_in);
    exmem_wb_l.jal_in(exmem_wb_jal_in);
    exmem_wb_l.mem_read_in(exmem_wb_mem_read_in);
    exmem_wb_l.wb_enable_1_in(exmem_wb_wb_enable_1_in);
    exmem_wb_l.wb_enable_0_in(exmem_wb_wb_enable_0_in);
    /* Outputs */
    exmem_wb_l.clk(clk);
    exmem_wb_l.rst(rst);
    exmem_wb_l.we(exmem_wb_we);
    exmem_wb_l.npc_out(exmem_wb_npc_out);
    exmem_wb_l.alu_out_1_out(exmem_wb_alu_out_1_out);
    exmem_wb_l.dst_reg_1_out(exmem_wb_dst_reg_1_out);
    exmem_wb_l.dst_reg_jal_out(exmem_wb_dst_reg_jal_out);
    exmem_wb_l.alu_out_0_out(exmem_wb_alu_out_0_out);
    exmem_wb_l.lmd_out(exmem_wb_lmd_out);
    exmem_wb_l.dst_reg_0_out(exmem_wb_dst_reg_0_out);
    exmem_wb_l.jal_out(exmem_wb_jal_out);
    exmem_wb_l.mem_read_out(exmem_wb_mem_read_out);
    exmem_wb_l.wb_enable_1_out(exmem_wb_wb_enable_1_out);
    exmem_wb_l.wb_enable_0_out(exmem_wb_wb_enable_0_out);
    

    /*  WB Stage */
    wb_s.clk(clk);
    wb_s.rst(rst);
    wb_s.npc(exmem_wb_npc_out);
    wb_s.alu_out_1(exmem_wb_alu_out_1_out);
    wb_s.dst_reg_1(exmem_wb_dst_reg_1_out);
    wb_s.dst_reg_jal(exmem_wb_dst_reg_jal_out);
    wb_s.alu_out_0(exmem_wb_alu_out_0_out);
    wb_s.lmd(exmem_wb_lmd_out);
    wb_s.dst_reg_0(exmem_wb_dst_reg_0_out);
    wb_s.jal(exmem_wb_jal_out);
    wb_s.mem_read(exmem_wb_mem_read_out);
    wb_s.wb_enable_1(exmem_wb_wb_enable_1_out);
    wb_s.wb_enable_0(exmem_wb_wb_enable_0_out);
    wb_s.reg_file_write_enable_1(reg_file_write_enable_1);
    wb_s.reg_file_write_address_1(reg_file_write_address_1);
    wb_s.reg_file_write_data_1(reg_file_write_data_1);
    wb_s.reg_file_write_enable_0(reg_file_write_enable_0);
    wb_s.reg_file_write_address_0(reg_file_write_address_0);
    wb_s.reg_file_write_data_0(reg_file_write_data_0);

    hdu.clk(clk);
    hdu.rst(rst);
    hdu.if_id_ir_1(if_id_ir_1_out);
    hdu.if_id_ir_0(if_id_ir_0_out);
    hdu.id_exmem_ir_1(id_exmem_ir_1_out);
    hdu.id_exmem_ir_0(id_exmem_ir_0_out);
    hdu.jump_or_branch_taken(jump_or_branch);
    hdu.alu_1_data_ready(hdu_alu_1_data_ready);
    hdu.alu_0_data_ready(hdu_alu_0_data_ready);
    hdu.pc_write_enable(pc_we);
    hdu.if_id_rst(if_id_hdu_rst);
    hdu.if_id_we(if_id_we);
    hdu.id_exmem_rst(id_exmem_hdu_rst);
    hdu.id_exmem_we(id_exmem_we);
    hdu.exmem_wb_we(exmem_wb_we);
	
    /* Tracing section */
    sc_trace(tcf, clk, "001_GBL_clk");
    sc_trace(tcf, rst, "000_GBL_rst");
    
    sc_trace(tcf, cu_branch_taken, "CU_branch_taken");
    sc_trace(tcf, cu_jump, "CU_jump");
    sc_trace(tcf, cu_jal, "CU_jal");
    
    //sc_trace(tcf, cu_alu_src_1, "CU_alu_src_1");
    //sc_trace(tcf, cu_alu_src_0, "CU_alu_src_0");
    //sc_trace(tcf, cu_reg_dst_1, "CU_reg_dst_1");
    //sc_trace(tcf, cu_reg_dst_0, "CU_reg_dst_0");
    //
    //sc_trace(tcf, cu_mem_read, "CU_mem_read");
    //sc_trace(tcf, cu_mem_write, "CU_reg_write");

    
    //sc_trace(tcf, cu.opcode_1, "CU_opcode_1");
    //sc_trace(tcf, cu.opcode_0, "CU_opcode_0");
    
    //sc_trace(tcf, dm_data_in, "DM_data_in");
    //sc_trace(tcf, dm_we, "DM_we");
    //sc_trace(tcf, exmem_wb_alu_out_0_in, "DM_addr");
    //sc_trace(tcf, exmem_wb_lmd_in, "DM_data_out");


    /** Start simulation */
    /* rst assignment */
    rst.write(SC_LOGIC_1);
    sc_start(7, SC_NS);

    rst.write(SC_LOGIC_0);
    sc_start(2000, SC_NS);

    /* Close Tracefile */
    sc_close_vcd_trace_file(tcf);
    return 0;
} 
