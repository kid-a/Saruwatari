-------------------------------------------------------------------------------
-- pipeline.cc    
-------------------------------------------------------------------------------
--
-- This file is part of SARUWATARI.
-- Copyright (C) 2010 Loris Fichera (loris.fichera@gmail.com)
--                    Daniele Marletta (danielemar86@gmail.com) 
--
-- SARUWATARI is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- SARUWATARI is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with SARUWATARI.  If not, see <http://www.gnu.org/licenses/>.
--
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity pipeline is
end pipeline;
-------------------------------------------------------------------------------
architecture sch of pipeline is
  
  -----------------------------------------------------------------------------
  -- components declaration
  -----------------------------------------------------------------------------

  --  IF  Stage
  component if_stage
    port (
      address_after_jump_or_branch : in  std_logic_vector(31 downto 0);
      jump_or_branch               : in  std_logic;
      clk                          : in  std_logic;
      rst                          : in  std_logic;
      pc_write_enable              : in  std_logic;
      new_program_counter          : out std_logic_vector(31 downto 0);
      ir_1                         : out std_logic_vector(31 downto 0);
      ir_0                         : out std_logic_vector(31 downto 0));
  end component;


  --  IF/ID  Latch 
  component if_id_latch
    port (
      clk	: in  std_logic;
      rst     	: in  std_logic;
      we	: in  std_logic;
      npc_in  	: in  std_logic_vector(31 downto 0);
      ir_1_in  	: in  std_logic_vector(31 downto 0);
      ir_0_in  	: in  std_logic_vector(31 downto 0);
      npc_out  	: out std_logic_vector(31 downto 0);
      ir_1_out  : out std_logic_vector(31 downto 0);
      ir_0_out 	: out std_logic_vector(31 downto 0));
  end component;


  --  ID  Stage
  component id_stage
    port (
      npc_in                   : in std_logic_vector(31 downto 0);  -- new program counter
      ir_1_in                  : in std_logic_vector(31 downto 0);
      ir_0_in                  : in std_logic_vector(31 downto 0);
      clk                      : in std_logic;   -- clock
      rst                      : in std_logic;   -- reset

--      id_exmem_ir_1_nop        : in std_logic_vector(31 downto 0);
--      id_exmem_ir_1_selector   : in std_logic;
      
      reg_file_write_enable_1  : in std_logic;   -- write enable for datapath 1
      reg_file_write_address_1 : in std_logic_vector(4 downto 0);   -- destination
                                                                    -- address for
                                                                    -- datapath 1
      reg_file_write_data_1    : in std_logic_vector(31 downto 0);  -- data from
                                                                    -- WB_1
      reg_file_write_enable_0  : in std_logic;   -- write enable for datapath 0
      reg_file_write_address_0 : in std_logic_vector(4 downto 0);   -- destination
                                                                    -- address for
                                                                    -- datapath 0
      reg_file_write_data_0    : in std_logic_vector(31 downto 0);  -- data from
                                                                    -- WB_0
      -- datapath signals
      npc_out                  : out std_logic_vector(31 downto 0);  -- new program counter
      ir_1_out                 : out std_logic_vector(31 downto 0);  -- instruction1
      ir_0_out                 : out std_logic_vector(31 downto 0);  -- instruction0
      immediate_1              : out std_logic_vector(31 downto 0); 
      immediate_0              : out std_logic_vector(31 downto 0);  
      address_after_jump_or_branch : out std_logic_vector(31 downto 0);
      jump_or_branch               : out std_logic;
      A1 		       : out std_logic_vector(31 downto 0);  -- operand A0
      B1		       : out std_logic_vector(31 downto 0);  -- operand B0
      A0 		       : out std_logic_vector(31 downto 0);  -- operand B0
      B0 	               : out std_logic_vector(31 downto 0);  -- operand B1
      -- Control Unit signals
--      if_id_latch_rst        : out std_logic;
--      pc_write_enable        : out std_logic;
      alu_src_1	             : out std_logic; -- I-type LOW -- R-type HIGH
      alu_src_0	             : out std_logic; -- I-type LOW -- R-type HIGH	
      reg_dst_1 	     : out std_logic;
      reg_dst_0 	     : out std_logic;
--      jump		     : out std_logic; -- datapath 1 
      jal   		     : out std_logic; -- datapath 1
--      branch_taken	     : out std_logic; -- datapath 1
      mem_read	             : out std_logic; -- datapath 0 
      mem_write	             : out std_logic; -- datapath 0
      wb_enable_1            : out std_logic;
      wb_enable_0            : out std_logic);
  end component;


  --  ID/EX-MEM  Latch
  component id_exmem_latch
    port (
      clk              	: in  std_logic;  -- clock
      rst               : in  std_logic;  -- reset
      we	        : in std_logic;
      
      npc_in            : in  std_logic_vector(31 downto 0);
      ir_1_in  		: in  std_logic_vector(31 downto 0);
      ir_0_in  		: in  std_logic_vector(31 downto 0);
      immediate_1_in    : in  std_logic_vector(31 downto 0);  
      immediate_0_in    : in  std_logic_vector(31 downto 0);
      A1_in 	      	: in  std_logic_vector(31 downto 0);
      B1_in	      	: in  std_logic_vector(31 downto 0);	
      A0_in 	      	: in  std_logic_vector(31 downto 0);
      B0_in             : in  std_logic_vector(31 downto 0);
      
      -- Control Unit FLAGS
      reg_dst_1_in 	: in  std_logic;
      reg_dst_0_in 	: in  std_logic;
--      branch_taken_in	: in  std_logic; -- datapath 1
--      jump_in		: in  std_logic; -- datapath 1 
      jal_in   		: in  std_logic; -- datapath 1 
      alu_src_1_in	: in  std_logic; -- I-type LOW, R-type HIGH
      alu_src_0_in	: in  std_logic; -- I-type LOW, R-type HIGH
      mem_read_in	: in  std_logic; -- datapath 0 
      mem_write_in	: in  std_logic; -- datapath 0
      wb_enable_1_in    : in  std_logic;
      wb_enable_0_in    : in  std_logic;
      
      --
      --  Output Signals
      --
      npc_out           : out  std_logic_vector(31 downto 0);
      ir_1_out  	: out  std_logic_vector(31 downto 0);
      ir_0_out  	: out  std_logic_vector(31 downto 0);
      immediate_1_out   : out  std_logic_vector(31 downto 0);  
      immediate_0_out   : out  std_logic_vector(31 downto 0);
      A1_out 	      	: out  std_logic_vector(31 downto 0);
      B1_out	      	: out  std_logic_vector(31 downto 0);	
      A0_out 	      	: out  std_logic_vector(31 downto 0);
      B0_out            : out  std_logic_vector(31 downto 0);
      
      -- Control Unit FLAGS
      reg_dst_1_out 	: out  std_logic;
      reg_dst_0_out 	: out  std_logic;
--      branch_taken_out	: out  std_logic; -- datapath 1
--      jump_out		: out  std_logic; -- datapath 1 
      jal_out   	: out  std_logic; -- datapath 1
      alu_src_1_out	: out  std_logic; -- I-type LOW, R-type HIGH
      alu_src_0_out	: out  std_logic; -- I-type LOW, R-type HIGH
      mem_read_out	: out  std_logic; -- datapath 0 
      mem_write_out	: out  std_logic;  -- datapath 0
      wb_enable_1_out   : out  std_logic;
      wb_enable_0_out   : out  std_logic);
  end component;


  --  EXMEM  Stage
  component exmem_stage  
    port(
      clk                          : in std_logic;
      rst                          : in std_logic;
      npc_in                       : in std_logic_vector(31 downto 0);
      ir_1                         : in std_logic_vector(31 downto 0);
      ir_0                         : in std_logic_vector(31 downto 0);
      immediate_1                  : in std_logic_vector(31 downto 0);
      immediate_0                  : in std_logic_vector(31 downto 0);
      A1                           : in std_logic_vector(31 downto 0);
      B1                           : in std_logic_vector(31 downto 0);
      A0                           : in std_logic_vector(31 downto 0);
      B0                           : in std_logic_vector(31 downto 0);
      reg_dst_1                    : in std_logic;
      reg_dst_0                    : in std_logic;
--      branch_taken               : in std_logic;
--      jump                       : in std_logic;
      jal_in                       : in std_logic;
      alu_src_1                    : in std_logic;
      alu_src_0                    : in std_logic;
      mem_read_in                  : in std_logic;
      mem_write                    : in std_logic;
      wb_enable_1_in               : in std_logic;
      wb_enable_0_in               : in std_logic;
      
--      address_after_jump_or_branch : out std_logic_vector(31 downto 0);
--      jump_or_branch               : out std_logic;
      alu_1_data_ready_out         : out std_logic;
      alu_0_data_ready_out         : out std_logic;  
      npc_out                      : out std_logic_vector(31 downto 0);
      alu_out_1                    : out std_logic_vector(31 downto 0);
      dst_reg_1                    : out std_logic_vector(4 downto 0);
      alu_out_0                    : out std_logic_vector(31 downto 0);
      lmd                          : out std_logic_vector(31 downto 0);
      dst_reg_0                    : out std_logic_vector(4 downto 0);
      jal_out			   : out std_logic;
      mem_read_out		   : out std_logic;
      wb_enable_1_out              : out std_logic;
      wb_enable_0_out              : out std_logic);
    
  end component;


  --  EXMEM/WB  Latch
  component exmem_wb_latch is 
    port (
      clk               : in  std_logic;
      rst               : in  std_logic;
      we	        : in std_logic;
      npc_in            : in  std_logic_vector(31 downto 0);
      alu_out_1_in      : in std_logic_vector(31 downto 0); 	
      alu_out_0_in      : in std_logic_vector(31 downto 0); 	
      dst_reg_1_in      : in std_logic_vector(4 downto 0); 
      lmd_in            : in  std_logic_vector(31 downto 0);
      dst_reg_0_in      : in std_logic_vector(4 downto 0); 
      jal_in	        : in std_logic;
      mem_read_in	: in std_logic;
      wb_enable_1_in    : in  std_logic;
      wb_enable_0_in    : in  std_logic;

      npc_out           : out  std_logic_vector(31 downto 0);
      alu_out_1_out     : out std_logic_vector(31 downto 0); 	
      alu_out_0_out     : out std_logic_vector(31 downto 0); 	
      dst_reg_1_out     : out std_logic_vector(4 downto 0); 
      dst_reg_jal_out   : out std_logic_vector(4 downto 0); 
      lmd_out           : out  std_logic_vector(31 downto 0);
      dst_reg_0_out     : out std_logic_vector(4 downto 0); 
      jal_out	        : out std_logic;
      mem_read_out      : out  std_logic;
      wb_enable_1_out   : out std_logic;
      wb_enable_0_out   : out std_logic);
    
  end component;


  -- WB
  component wb_stage 
    port (
      npc         : in std_logic_vector(31 downto 0);
      alu_out_1   : in std_logic_vector(31 downto 0);
      dst_reg_1   : in std_logic_vector(4 downto 0);
      dst_reg_jal : in std_logic_vector(4 downto 0);
      alu_out_0   : in std_logic_vector(31 downto 0);
      lmd         : in std_logic_vector(31 downto 0);
      dst_reg_0   : in std_logic_vector(4 downto 0);
      jal         : in std_logic;
      mem_read    : in std_logic;
      wb_enable_1 : in std_logic;
      wb_enable_0 : in std_logic;
      
      reg_file_write_enable_1  : out std_logic;
      reg_file_write_address_1 : out std_logic_vector(4 downto 0);
      reg_file_write_data_1    : out std_logic_vector(31 downto 0);
      reg_file_write_enable_0  : out std_logic;
      reg_file_write_address_0 : out std_logic_vector(4 downto 0);
      reg_file_write_data_0    : out std_logic_vector(31 downto 0));
    
  end component;

  component reg
    generic (
      register_size : integer);
    port (
      data_in      : in  std_logic_vector(register_size - 1 downto 0);
      clk, we, rst : in  std_logic;
      data_out     : out std_logic_vector(register_size - 1 downto 0));
  end component;

  component hazard_detection_unit
    
    port (
      clk                    : in  std_logic;
      rst                    : in  std_logic;
      if_id_ir_1             : in  std_logic_vector(31 downto 0);
      if_id_ir_0             : in  std_logic_vector(31 downto 0);
      id_exmem_ir_1          : in  std_logic_vector(31 downto 0);
      id_exmem_ir_0          : in  std_logic_vector(31 downto 0);
      jump_or_branch_taken   : in  std_logic;
      alu_1_data_ready       : in  std_logic;
      alu_0_data_ready       : in  std_logic;
      pc_write_enable        : out std_logic;
      id_exmem_rst	   : out std_logic;
      if_id_rst		   : out std_logic;
      if_id_we              : out std_logic;
--    id_exmem_ir_1_nop      : out std_logic_vector(31 downto 0);
--    id_exmem_ir_1_selector : out std_logic;
      id_exmem_we            : out std_logic;
      exmem_wb_we            : out std_logic);

  end component;

  
  -----------------------------------------------------------------------------
  -- global signals
  -----------------------------------------------------------------------------
  signal clk : std_logic;
  signal clk_neg : std_logic;
  signal rst : std_logic := '1';
  signal if_id_we 	: std_logic :='1';
  signal id_exmem_we 	: std_logic :='1';
  signal exmem_wb_we 	: std_logic :='1';
  signal if_id_latch_hdu_reset : std_logic;
--  signal if_id_latch_cu_reset : std_logic;
  signal id_exmem_latch_hdu_reset : std_logic;
  signal id_hdu_pc_write_enable : std_logic;
  -----------------------------------------------------------------------------


  -----------------------------------------------------------------------------
  -- instruction fetch stage -- input signals
  -----------------------------------------------------------------------------
  signal if_address_after_jump_or_branch	: std_logic_vector(31 downto 0);
  signal if_jump_or_branch 			: std_logic;
  signal pc_write_enable : std_logic;
  -----------------------------------------------------------------------------
  -- instruction fetch (IF) stage -- output signals
  -----------------------------------------------------------------------------
  -- NO output signals
  -----------------------------------------------------------------------------
  
  
  -----------------------------------------------------------------------------
  -- if_id latch -- input signals
  -----------------------------------------------------------------------------
  signal if_id_npc_in 	: std_logic_vector(31 downto 0);
  signal if_id_ir_1_in 	: std_logic_vector(31 downto 0);
  signal if_id_ir_0_in 	: std_logic_vector(31 downto 0);
  signal if_id_latch_reset : std_logic;

  -----------------------------------------------------------------------------
  -- if_id latch -- output signals
  -----------------------------------------------------------------------------
  signal if_id_npc_out 	: std_logic_vector(31 downto 0);
  signal if_id_ir_1_out : std_logic_vector(31 downto 0);
  signal if_id_ir_0_out : std_logic_vector(31 downto 0);


  -----------------------------------------------------------------------------
  -- instruction decode (ID) stage -- input signals
  -----------------------------------------------------------------------------
  signal id_reg_file_write_enable_1 	: std_logic;
  signal id_reg_file_write_address_1 	: std_logic_vector(4 downto 0);
  signal id_reg_file_write_data_1 	: std_logic_vector(31 downto 0);
  signal id_reg_file_write_enable_0 	: std_logic;
  signal id_reg_file_write_address_0 	: std_logic_vector(4 downto 0);
  signal id_reg_file_write_data_0 	: std_logic_vector(31 downto 0);
  signal id_exmem_ir_1_nop              : std_logic_vector(31 downto 0);
  signal id_exmem_ir_1_selector         : std_logic;

  -----------------------------------------------------------------------------
  -- instruction decode (ID) stage -- output signals
  -----------------------------------------------------------------------------
--  signal id_pc_write_enable : std_logic;
  -----------------------------------------------------------------------------

  
  -----------------------------------------------------------------------------
  -- id_exmem latch -- input signals
  -----------------------------------------------------------------------------
  signal id_exmem_npc_in		:  std_logic_vector(31 downto 0);
  signal id_exmem_ir_1_in  		:  std_logic_vector(31 downto 0);
  signal id_exmem_ir_0_in  		:  std_logic_vector(31 downto 0);
  signal id_exmem_immediate_1_in     	:  std_logic_vector(31 downto 0);  
  signal id_exmem_immediate_0_in    	:  std_logic_vector(31 downto 0);
  signal id_exmem_A1_in 	      	:  std_logic_vector(31 downto 0);
  signal id_exmem_B1_in	      	:  std_logic_vector(31 downto 0);	
  signal id_exmem_A0_in 	      	:  std_logic_vector(31 downto 0);
  signal id_exmem_B0_in              	:  std_logic_vector(31 downto 0);
  signal id_exmem_reg_dst_1_in 	:  std_logic;
  signal id_exmem_reg_dst_0_in 	:  std_logic;
--  signal id_exmem_branch_taken_in	:  std_logic; -- datapath 1
--  signal id_exmem_jump_in		:  std_logic; -- datapath 1 
  signal id_exmem_jal_in   		:  std_logic; -- datapath 1 
  signal id_exmem_alu_src_1_in	:  std_logic; -- I-type LOW, R-type HIGH
  signal id_exmem_alu_src_0_in	:  std_logic; -- I-type LOW, R-type HIGH
  signal id_exmem_mem_read_in		:  std_logic; -- datapath 0 
  signal id_exmem_mem_write_in	:  std_logic; -- datapath 0
  signal id_exmem_wb_enable_1_in	:  std_logic;               
  signal id_exmem_wb_enable_0_in	:  std_logic; 
  signal id_exmem_latch_reset : std_logic;
  
  
  -----------------------------------------------------------------------------
  -- id_exmem latch -- output signals
  -----------------------------------------------------------------------------
  signal id_exmem_npc_out            	:  std_logic_vector(31 downto 0);
  signal id_exmem_ir_1_out  		:  std_logic_vector(31 downto 0);
  signal id_exmem_ir_0_out  		:  std_logic_vector(31 downto 0);
  signal id_exmem_immediate_1_out     :  std_logic_vector(31 downto 0);  
  signal id_exmem_immediate_0_out    	:  std_logic_vector(31 downto 0);
  signal id_exmem_A1_out 	      	:  std_logic_vector(31 downto 0);
  signal id_exmem_B1_out	      	:  std_logic_vector(31 downto 0);	
  signal id_exmem_A0_out 	      	:  std_logic_vector(31 downto 0);
  signal id_exmem_B0_out              :  std_logic_vector(31 downto 0);
  signal id_exmem_reg_dst_1_out 	:  std_logic;
  signal id_exmem_reg_dst_0_out 	:  std_logic;
--  signal id_exmem_branch_taken_out	:  std_logic; -- datapath 1
--  signal id_exmem_jump_out		:  std_logic; -- datapath 1 
  signal id_exmem_jal_out   		:  std_logic; -- datapath 1 
  signal id_exmem_alu_src_1_out	:  std_logic; -- I-type LOW, R-type HIGH
  signal id_exmem_alu_src_0_out	:  std_logic; -- I-type LOW, R-type HIGH
  signal id_exmem_mem_read_out	:  std_logic; -- datapath 0 
  signal id_exmem_mem_write_out	:  std_logic; -- datapath 0
  signal id_exmem_wb_enable_1_out     :  std_logic;
  signal id_exmem_wb_enable_0_out     :  std_logic;

  -----------------------------------------------------------------------------
  -- executememory (EXMEM) stage -- input signals 
  -----------------------------------------------------------------------------
  -- NO input signals 
  -----------------------------------------------------------------------------
  
  -----------------------------------------------------------------------------
  -- executememory (EXMEM) stage  -- output signals
  -----------------------------------------------------------------------------
  signal alu_1_data_ready : std_logic;
  signal alu_0_data_ready : std_logic;
  

  -----------------------------------------------------------------------------
  -- EXMEM/WB latch -- input signals 
  -----------------------------------------------------------------------------
  signal exmem_wb_npc_in            :   std_logic_vector(31 downto 0);
  signal exmem_wb_alu_out_1_in      :  std_logic_vector(31 downto 0); 	
  signal exmem_wb_alu_out_0_in      :  std_logic_vector(31 downto 0); 	
  signal exmem_wb_dst_reg_1_in      :  std_logic_vector(4 downto 0); 
  signal exmem_wb_lmd_in            :   std_logic_vector(31 downto 0);
  signal exmem_wb_dst_reg_0_in      :  std_logic_vector(4 downto 0); 
  signal exmem_wb_jal_in	      :  std_logic;
  signal exmem_wb_mem_read_in	      :  std_logic;
  signal exmem_wb_wb_enable_1_in    :   std_logic;
  signal exmem_wb_wb_enable_0_in    :   std_logic;
  -----------------------------------------------------------------------------
  
  -----------------------------------------------------------------------------
  -- EXMEM/WB  latch  -- output signals
  -----------------------------------------------------------------------------
  signal exmem_wb_npc_out           :   std_logic_vector(31 downto 0);
  signal exmem_wb_alu_out_1_out     :  std_logic_vector(31 downto 0); 	
  signal exmem_wb_alu_out_0_out     :  std_logic_vector(31 downto 0); 	
  signal exmem_wb_dst_reg_1_out     :  std_logic_vector(4 downto 0); 
  signal exmem_wb_dst_reg_jal_out   :  std_logic_vector(4 downto 0); 
  signal exmem_wb_lmd_out           :   std_logic_vector(31 downto 0);
  signal exmem_wb_dst_reg_0_out     :  std_logic_vector(4 downto 0); 
  signal exmem_wb_jal_out	      :  std_logic;
  signal exmem_wb_mem_read_out      :   std_logic;
  signal exmem_wb_wb_enable_1_out   :  std_logic;
  signal exmem_wb_wb_enable_0_out   :  std_logic;
  
  -----------------------------------------------------------------------------
  
begin  -- sch

  --  IF	
  instruction_fetch_stage : if_stage
    port map (
      address_after_jump_or_branch => if_address_after_jump_or_branch,
      jump_or_branch               => if_jump_or_branch,
      clk                          => clk,
      rst                          => rst,
      pc_write_enable              => pc_write_enable,
      new_program_counter          => if_id_npc_in,
      ir_1                         => if_id_ir_1_in,
      ir_0                         => if_id_ir_0_in);
  
  --  IF/ID
  fetch_decode_latch : if_id_latch
    port map (
      clk               => clk,
      rst               => if_id_latch_reset,
      we		=> if_id_we,
      npc_in            => if_id_npc_in,
      ir_1_in  		=> if_id_ir_1_in,
      ir_0_in  		=> if_id_ir_0_in,
      npc_out           => if_id_npc_out,
      ir_1_out          => if_id_ir_1_out,
      ir_0_out          => if_id_ir_0_out);

  --  ID  
  instruction_decode_stage : id_stage
    port map (
      npc_in                   => if_id_npc_out,
      ir_1_in                  => if_id_ir_1_out,
      ir_0_in                  => if_id_ir_0_out,
      clk                      => clk,
      rst                      => rst,

      reg_file_write_enable_1  => id_reg_file_write_enable_1,
      reg_file_write_data_1    => id_reg_file_write_data_1,
      reg_file_write_address_1 => id_reg_file_write_address_1,
      reg_file_write_enable_0  => id_reg_file_write_enable_0,
      reg_file_write_data_0    => id_reg_file_write_data_0,
      reg_file_write_address_0 => id_reg_file_write_address_0,
      npc_out                  => id_exmem_npc_in,
      ir_1_out                 => id_exmem_ir_1_in,
      ir_0_out                 => id_exmem_ir_0_in,
      immediate_1              => id_exmem_immediate_1_in,
      immediate_0              => id_exmem_immediate_0_in,
      A1                       => id_exmem_A1_in,
      B1                       => id_exmem_B1_in,
      A0                       => id_exmem_A0_in,
      B0                       => id_exmem_B0_in,
      address_after_jump_or_branch => if_address_after_jump_or_branch,
      jump_or_branch           => if_jump_or_branch,

      reg_dst_1                => id_exmem_reg_dst_1_in,
      reg_dst_0                => id_exmem_reg_dst_0_in,

      jal                      => id_exmem_jal_in,
      alu_src_1                => id_exmem_alu_src_1_in,
      alu_src_0                => id_exmem_alu_src_0_in,
      mem_read                 => id_exmem_mem_read_in,
      mem_write                => id_exmem_mem_write_in,
      wb_enable_1              => id_exmem_wb_enable_1_in,
      wb_enable_0              => id_exmem_wb_enable_0_in);

  --  ID/EX-MEM
  decode_executememory_latch : id_exmem_latch
    port map (
      clk               => clk,
      we		=> id_exmem_we,
      rst               => id_exmem_latch_reset,
      npc_in            => id_exmem_npc_in,
      ir_1_in           => id_exmem_ir_1_in,
      ir_0_in           => id_exmem_ir_0_in,
      immediate_1_in    => id_exmem_immediate_1_in,      
      immediate_0_in    => id_exmem_immediate_0_in,
      A1_in             => id_exmem_A1_in,
      B1_in	        => id_exmem_B1_in,
      A0_in 	        => id_exmem_A0_in,
      B0_in             => id_exmem_B0_in,
      reg_dst_1_in      => id_exmem_reg_dst_1_in,
      reg_dst_0_in 	=> id_exmem_reg_dst_0_in,

      jal_in   		=> id_exmem_jal_in,
      alu_src_1_in	=> id_exmem_alu_src_1_in,
      alu_src_0_in	=> id_exmem_alu_src_0_in,
      mem_read_in	=> id_exmem_mem_read_in,
      mem_write_in	=> id_exmem_mem_write_in,
      wb_enable_1_in    => id_exmem_wb_enable_1_in,
      wb_enable_0_in    => id_exmem_wb_enable_0_in,
      
      npc_out           => id_exmem_npc_out,
      ir_1_out  	=> id_exmem_ir_1_out,
      ir_0_out  	=> id_exmem_ir_0_out,
      immediate_1_out   => id_exmem_immediate_1_out,
      immediate_0_out   => id_exmem_immediate_0_out,
      A1_out 	      	=> id_exmem_A1_out,
      B1_out	      	=> id_exmem_B1_out,
      A0_out 	      	=> id_exmem_A0_out,
      B0_out            => id_exmem_B0_out,
      reg_dst_1_out 	=> id_exmem_reg_dst_1_out,
      reg_dst_0_out 	=> id_exmem_reg_dst_0_out,

      jal_out   	=> id_exmem_jal_out,    
      alu_src_1_out	=> id_exmem_alu_src_1_out,
      alu_src_0_out	=> id_exmem_alu_src_0_out,
      mem_read_out	=> id_exmem_mem_read_out,
      mem_write_out	=> id_exmem_mem_write_out,
      wb_enable_1_out   => id_exmem_wb_enable_1_out,
      wb_enable_0_out   => id_exmem_wb_enable_0_out);


  --  EXMEM  
  executememory_stage : exmem_stage
    port map (
      clk             => clk,
      rst             => rst,
      npc_in          => id_exmem_npc_out,
      ir_1            => id_exmem_ir_1_out,
      ir_0            => id_exmem_ir_0_out,
      immediate_1     => id_exmem_immediate_1_out,
      immediate_0     => id_exmem_immediate_0_out,
      A1              => id_exmem_A1_out,
      B1              => id_exmem_B1_out,
      A0              => id_exmem_A0_out,
      B0              => id_exmem_B0_out,
      reg_dst_1       => id_exmem_reg_dst_1_out,
      reg_dst_0       => id_exmem_reg_dst_0_out,

      jal_in          => id_exmem_jal_out,
      alu_src_1       => id_exmem_alu_src_1_out,
      alu_src_0       => id_exmem_alu_src_0_out,
      mem_read_in     => id_exmem_mem_read_out,
      mem_write       => id_exmem_mem_write_out,
      wb_enable_1_in  => id_exmem_wb_enable_1_out,
      wb_enable_0_in  => id_exmem_wb_enable_0_out,

      alu_1_data_ready_out => alu_1_data_ready,
      alu_0_data_ready_out => alu_0_data_ready,
      npc_out	      => exmem_wb_npc_in,
      alu_out_1       => exmem_wb_alu_out_1_in,
      dst_reg_1       => exmem_wb_dst_reg_1_in,
      alu_out_0       => exmem_wb_alu_out_0_in,
      lmd             => exmem_wb_lmd_in,
      dst_reg_0       => exmem_wb_dst_reg_0_in,
      jal_out         => exmem_wb_jal_in,
      mem_read_out    => exmem_wb_mem_read_in,
      wb_enable_1_out => exmem_wb_wb_enable_1_in,
      wb_enable_0_out => exmem_wb_wb_enable_0_in);
  
  
  --  EXMEM/WB
  executememory_writeback_latch : exmem_wb_latch 
    port map (
      clk               => clk, 
      we		   => exmem_wb_we,
      rst               => rst, 
      npc_in            => exmem_wb_npc_in, 
      alu_out_1_in      => exmem_wb_alu_out_1_in, 
      alu_out_0_in      => exmem_wb_alu_out_0_in, 
      dst_reg_1_in      => exmem_wb_dst_reg_1_in,  
      lmd_in            => exmem_wb_lmd_in, 
      dst_reg_0_in      => exmem_wb_dst_reg_0_in, 
      jal_in	           => exmem_wb_jal_in, 
      mem_read_in	   => exmem_wb_mem_read_in, 
      wb_enable_1_in    => exmem_wb_wb_enable_1_in, 
      wb_enable_0_in    => exmem_wb_wb_enable_0_in, 
      
      npc_out           => exmem_wb_npc_out, 
      alu_out_1_out     => exmem_wb_alu_out_1_out, 
      alu_out_0_out     => exmem_wb_alu_out_0_out, 
      dst_reg_1_out     => exmem_wb_dst_reg_1_out, 
      dst_reg_jal_out   => exmem_wb_dst_reg_jal_out, 
      lmd_out           => exmem_wb_lmd_out, 
      dst_reg_0_out     => exmem_wb_dst_reg_0_out, 
      jal_out	   => exmem_wb_jal_out, 
      mem_read_out      => exmem_wb_mem_read_out, 
      wb_enable_1_out   => exmem_wb_wb_enable_1_out, 
      wb_enable_0_out   => exmem_wb_wb_enable_0_out); 
  
  
  -- WB
  writeback_stage : wb_stage
    port map (
      npc 		=>	exmem_wb_npc_out,	
      alu_out_1 	=>	exmem_wb_alu_out_1_out,
      dst_reg_1 	=>	exmem_wb_dst_reg_1_out,	
      dst_reg_jal 	=>	exmem_wb_dst_reg_jal_out,
      alu_out_0 	=>	exmem_wb_alu_out_0_out,
      lmd 		=>	exmem_wb_lmd_out,
      dst_reg_0 	=>	exmem_wb_dst_reg_0_out,
      jal 		=>	exmem_wb_jal_out,
      mem_read 	        =>	exmem_wb_mem_read_out,
      wb_enable_1 	=>	exmem_wb_wb_enable_1_out,
      wb_enable_0 	=>	exmem_wb_wb_enable_0_out,
      
      reg_file_write_enable_1	=> id_reg_file_write_enable_1,	
      reg_file_write_address_1 	=> id_reg_file_write_address_1,
      reg_file_write_data_1	=> id_reg_file_write_data_1,
      reg_file_write_enable_0 	=> id_reg_file_write_enable_0,
      reg_file_write_address_0 	=> id_reg_file_write_address_0,
      reg_file_write_data_0	=> id_reg_file_write_data_0);
  
  hdu : hazard_detection_unit
    port map (
      clk                    => clk,
      rst                    => rst,
      if_id_ir_1             => if_id_ir_1_out,
      if_id_ir_0             => if_id_ir_0_out,
      id_exmem_ir_1          => id_exmem_ir_1_out,
      id_exmem_ir_0          => id_exmem_ir_0_out,
      jump_or_branch_taken   => if_jump_or_branch,
      alu_1_data_ready       => alu_1_data_ready,
      alu_0_data_ready       => alu_0_data_ready,
      if_id_rst		     => if_id_latch_hdu_reset,
      id_exmem_rst	     => id_exmem_latch_hdu_reset,
      pc_write_enable        => id_hdu_pc_write_enable,
      if_id_we               => if_id_we,
      id_exmem_we            => id_exmem_we,
      exmem_wb_we            => exmem_wb_we);

  pc_write_enable <= id_hdu_pc_write_enable;
  if_id_latch_reset <= rst or if_id_latch_hdu_reset;
  id_exmem_latch_reset <= rst or id_exmem_latch_hdu_reset;
  
--------------------------------------------------------------------------
  -- reset
--------------------------------------------------------------------------
  rst <= '0' after 12 ns;
  
  -- purpose: clock generating process
  process
  begin  -- process
    clk <= '1';
    clk_neg <= '0';
    wait for 5 ns;
    clk <= '0';
    clk_neg <= '1';
    wait for 5 ns;
  end process;
  
end sch;
