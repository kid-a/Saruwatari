-------------------------------------------------------------------------------
-- id_stage.vhd
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
entity id_stage is
  
  port (
    npc_in                   : in std_logic_vector(31 downto 0);  -- new program counter
    ir_1_in                  : in std_logic_vector(31 downto 0);
    ir_0_in                  : in std_logic_vector(31 downto 0);
    clk                      : in std_logic;   -- clock
    rst                      : in std_logic;   -- reset
    
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
    address_after_jump_or_branch : out std_logic_vector(31 downto 0);
    jump_or_branch               : out std_logic;
    immediate_1              : out std_logic_vector(31 downto 0); 
    immediate_0              : out std_logic_vector(31 downto 0);  
    A1 		             : out std_logic_vector(31 downto 0);  -- operand A0
    B1		             : out std_logic_vector(31 downto 0);  -- operand B0
    A0 		             : out std_logic_vector(31 downto 0);  -- operand B0
    B0 	                     : out std_logic_vector(31 downto 0);  -- operand B1
    -- Control Unit signals
    alu_src_1	             : out std_logic; -- I-type LOW -- R-type HIGH
    alu_src_0	             : out std_logic; -- I-type LOW -- R-type HIGH	
    reg_dst_1 	             : out std_logic;
    reg_dst_0 	             : out std_logic;
    jal   		     : out std_logic; -- datapath 1
    mem_read	             : out std_logic; -- datapath 0 
    mem_write	             : out std_logic; -- datapath 0
    wb_enable_1              : out std_logic;
    wb_enable_0              : out std_logic);

end id_stage;
-------------------------------------------------------------------------------
architecture schem of id_stage is

  component regfile is 
    port (
      read_01	        : in std_logic_vector(4 downto 0);
      read_02	        : in std_logic_vector(4 downto 0);
      read_11	        : in std_logic_vector(4 downto 0);
      read_12   	: in std_logic_vector(4 downto 0);

      write_0   	: in std_logic_vector(4 downto 0);
      write_1   	: in std_logic_vector(4 downto 0);
      we_0      	: in std_logic;	   -- write enable
      we_1      	: in std_logic;	   -- write enable
      
      data_in_0 	: in std_logic_vector(31 downto 0);
      data_in_1 	: in std_logic_vector(31 downto 0);
      
      A0 		: out std_logic_vector(31 downto 0);
      B0		: out std_logic_vector(31 downto 0);	
      A1 		: out std_logic_vector(31 downto 0);
      B1 	        : out std_logic_vector(31 downto 0);	
      
      clk       	: in  std_logic;    -- clock
      rst       	: in  std_logic    -- reset
      );
  end component;


  component zerocomp is 
    port (
      register_in : in std_logic_vector(31 downto 0);
      is_zero : out std_logic
      );
  end component;

  
  component signext is 
    port (
      data_in : in std_logic_vector(15 downto 0);
      data_out : out std_logic_vector(31 downto 0)
      );
  end component;

  component mux2x1 is
    generic (
      port_size : integer := 32);
    port (
      a, b        : in  std_logic_vector(port_size - 1 downto 0);
      selector    : in  std_logic;             -- selector
      output      : out std_logic_vector(port_size - 1 downto 0));
  end component;

  component adder
    generic (
      operand_size : integer);
    port (
      operand_A : in  std_logic_vector(operand_size - 1 downto 0);
      operand_B : in  std_logic_vector(operand_size - 1 downto 0);
      sum       : out std_logic_vector(operand_size - 1 downto 0));
  end component;

  component control_unit is
    port (
      clk                    : in  std_logic;
      rst                    : in  std_logic;
      opcode_1               : in  std_logic_vector(5 downto 0);
      opcode_0               : in  std_logic_vector(5 downto 0);
      branch_operand_is_zero : in  std_logic;
      alu_src_1              : out std_logic; 
      alu_src_0              : out std_logic;
      reg_dst_1              : out std_logic;
      reg_dst_0              : out std_logic;
      jump                   : out std_logic; 
      jal                    : out std_logic;
      branch_taken           : out std_logic;
      mem_read               : out std_logic;
      mem_write              : out std_logic;
      wb_enable_1	     : out std_logic;
      wb_enable_0	     : out std_logic);
  end component;

  signal branch_operand : std_logic_vector(31 downto 0);
  signal branch_operand_is_zero : std_logic;
  signal clk_neg : std_logic;

  
-------------------------------------------------------------------------------
-- jump sign extensor output signal
-------------------------------------------------------------------------------
  signal extended_jump : std_logic_vector(31 downto 0);
  signal branch_address : std_logic_vector(31 downto 0);
  signal immediate_1_internal : std_logic_vector(31 downto 0);
  signal branch_taken_internal : std_logic;
  signal jump_internal : std_logic;
  signal jal_internal : std_logic;
  signal ir_1_internal : std_logic_vector(31 downto 0);

begin  -- schem

  register_file : regfile
    port map (
      read_01   => ir_0_in(25 downto 21),
      read_02   => ir_0_in(20 downto 16),
      read_11   => ir_1_in(25 downto 21),
      read_12   => ir_1_in(20 downto 16),
      write_0   => reg_file_write_address_0,
      write_1   => reg_file_write_address_1,
      we_0      => reg_file_write_enable_0,
      we_1      => reg_file_write_enable_1,
      data_in_0 => reg_file_write_data_0,
      data_in_1 => reg_file_write_data_1,
      A0        => A0,
      B0        => B0,
      A1        => branch_operand,
      B1        => B1,
      clk       => clk_neg,
      rst       => rst);

  zero_comparator : zerocomp
    port map (
      register_in => branch_operand,
      is_zero     => branch_operand_is_zero);
  
  sign_extensor_1 : signext
    port map (
      data_in  => ir_1_in(15 downto 0),
      data_out => immediate_1_internal);

  sign_extensor_0 : signext
    port map (
      data_in  => ir_0_in(15 downto 0),
      data_out => immediate_0);

  adder_instance : adder
    generic map (
      operand_size => 32)
    port map (
      operand_A    => npc_in,
      operand_B    => immediate_1_internal,
      sum          => branch_address);

  mux_jump_branch_address : mux2x1
    port map (
      a        => branch_operand,    -- It is A1
      b        => branch_address,
      selector => branch_taken_internal,
      output   => address_after_jump_or_branch);

  cu : control_unit
    port map (
      clk                    => clk,
      rst                    => rst,
      opcode_1               => ir_1_in(31 downto 26),
      opcode_0               => ir_0_in(31 downto 26),
      branch_operand_is_zero => branch_operand_is_zero,
      alu_src_1              => alu_src_1,
      alu_src_0              => alu_src_0,
      reg_dst_1              => reg_dst_1,
      reg_dst_0              => reg_dst_0,
      jump                   => jump_internal,
      jal                    => jal_internal,
      branch_taken           => branch_taken_internal,
      mem_read               => mem_read,
      mem_write              => mem_write,
      wb_enable_1            => wb_enable_1,
      wb_enable_0            => wb_enable_0);
  
  jump_or_branch <= jump_internal or branch_taken_internal or jal_internal;
  A1 <= branch_operand;
  jal <= jal_internal;
  npc_out <= npc_in;
  ir_0_out <= ir_0_in;
  ir_1_out <= ir_1_in;
  immediate_1 <= immediate_1_internal;
  clk_neg <= not clk;
  
end schem;
