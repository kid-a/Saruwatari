-------------------------------------------------------------------------------
-- exmem_stage.vhd
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
entity exmem_stage is
  
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
    jal_in                       : in std_logic;
    alu_src_1                    : in std_logic;
    alu_src_0                    : in std_logic;
    mem_read_in                  : in std_logic;
    mem_write                    : in std_logic;
    wb_enable_1_in               : in std_logic;
    wb_enable_0_in               : in std_logic;


    alu_1_data_ready_out         : out std_logic;
    alu_0_data_ready_out         : out std_logic;
    npc_out                      : out std_logic_vector(31 downto 0);
    alu_out_1                    : out std_logic_vector(31 downto 0);
    dst_reg_1                    : out std_logic_vector(4 downto 0);
    alu_out_0                    : out std_logic_vector(31 downto 0);
    lmd                          : out std_logic_vector(31 downto 0);
    dst_reg_0                    : out std_logic_vector(4 downto 0);
    jal_out			 : out std_logic;
    mem_read_out		 : out std_logic;
    wb_enable_1_out              : out std_logic;
    wb_enable_0_out              : out std_logic);
 
  
end exmem_stage;
-------------------------------------------------------------------------------
architecture schem of exmem_stage is

component alu is
  port (
    a, b       : in  std_logic_vector(31 downto 0);
    op         : in  std_logic_vector(3 downto 0);  -- opcode
    clk        : in  std_logic;
    rst        : in  std_logic;
    data_ready : out std_logic; 
    result     : out std_logic_vector(31 downto 0));
end component;

component ALU_controller is 
  port (
    opcode     : in std_logic_vector(5 downto 0);
    func       : in std_logic_vector(10 downto 0);
    alu_opcode : out std_logic_vector(3 downto 0) 
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

component ram is
  generic (
    words     : integer := 256);         -- number of words in the memory
  port (
    write_enable : in  std_logic;
    clk          : in  std_logic;
    addr         : in  std_logic_vector(31 downto 0);
    data_in      : in  std_logic_vector(31 downto 0);
    data_out     : out std_logic_vector(31 downto 0));
end component;


-------------------------------------------------------------------------------
-- alu_1 input signals
-------------------------------------------------------------------------------
signal alu_controller_alu_opcode_1 : std_logic_vector(3 downto 0);
signal alu_1_operand_0 : std_logic_vector(31 downto 0);
signal alu_1_operand_1 : std_logic_vector(31 downto 0);

-------------------------------------------------------------------------------
-- alu_1 output signal
-------------------------------------------------------------------------------
signal alu_out_1_internal : std_logic_vector(31 downto 0);
signal alu_1_data_ready : std_logic;

-------------------------------------------------------------------------------
-- alu_0 input signals
-------------------------------------------------------------------------------
signal alu_controller_alu_opcode_0 : std_logic_vector(3 downto 0);
signal alu_0_operand_1 : std_logic_vector(31 downto 0);

-------------------------------------------------------------------------------
-- alu_0 output signal
-------------------------------------------------------------------------------
signal alu_out_0_internal : std_logic_vector(31 downto 0);
signal alu_0_data_ready : std_logic;

-------------------------------------------------------------------------------
-- data memory output signal
-------------------------------------------------------------------------------
signal lmd_internal : std_logic_vector(31 downto 0);

-------------------------------------------------------------------------------
-- internal signals
-------------------------------------------------------------------------------
signal clk_neg : std_logic;

begin  -- schem

  alu_1_controller : ALU_controller
    port map (
      opcode     => ir_1(31 downto 26),
      func       => ir_1(10 downto 0),
      alu_opcode => alu_controller_alu_opcode_1);

   mux_1_imm_1_B1 : mux2x1
    port map (
      a        => immediate_1,
      b        => B1,
      selector => alu_src_1,
      output   => alu_1_operand_1);

  alu_1 : alu
    port map (
      a          => A1,
      b          => alu_1_operand_1,
      op         => alu_controller_alu_opcode_1,
      clk        => clk,
      rst        => rst,
      data_ready => alu_1_data_ready,
      result     => alu_out_1_internal);

  alu_0_controller : ALU_controller
    port map (
      opcode     => ir_0(31 downto 26),
      func       => ir_0(10 downto 0),
      alu_opcode => alu_controller_alu_opcode_0);

  mux_0_imm_0_B0 : mux2x1
    port map (
      a        => immediate_0,
      b        => B0,
      selector => alu_src_0,
      output   => alu_0_operand_1);

  alu_0 : alu
    port map (
      a          => A0,
      b          => alu_0_operand_1,
      op         => alu_controller_alu_opcode_0,
      clk        => clk,
      rst        => rst,
      data_ready => alu_0_data_ready,
      result     => alu_out_0_internal);
  
  mux_1_dest_B1 : mux2x1
    generic map (
      port_size => 5)
    port map (
      a        => ir_1(20 downto 16),
      b        => ir_1(15 downto 11),
      selector => reg_dst_1,
      output   => dst_reg_1 );

  mux_0_dest_B0 : mux2x1
    generic map (
      port_size => 5)
    port map (
      a        => ir_0(20 downto 16),
      b        => ir_0(15 downto 11),
      selector => reg_dst_0,
      output   => dst_reg_0 );
 
  data_memory : ram
    port map (
      write_enable => mem_write,
      clk          => clk_neg,
      addr         => alu_out_0_internal,
      data_in      => B0,
      data_out     => lmd_internal);

  alu_1_data_ready_out <= alu_1_data_ready;
  alu_0_data_ready_out <= alu_0_data_ready;
  npc_out <= npc_in;
  alu_out_1 <= alu_out_1_internal;
  alu_out_0 <= alu_out_0_internal;
  lmd <= lmd_internal;
  jal_out <= jal_in;
  mem_read_out <= mem_read_in;
  wb_enable_1_out <= wb_enable_1_in;
  wb_enable_0_out <= wb_enable_0_in;
  clk_neg <= not clk;

end schem;
