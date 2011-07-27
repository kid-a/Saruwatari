-------------------------------------------------------------------------------
-- if_stage.vhd
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
entity if_stage is
  
  port (
    address_after_jump_or_branch : in std_logic_vector(31 downto 0);
    jump_or_branch               : in std_logic;
    clk                          : in std_logic;
    rst                          : in std_logic;
    pc_write_enable              : in std_logic;
    new_program_counter          : out std_logic_vector(31 downto 0);
    ir_1                         : out std_logic_vector(31 downto 0);
    ir_0                         : out std_logic_vector(31 downto 0));

end if_stage;
-------------------------------------------------------------------------------
architecture sch of if_stage is

  component mux2x1
    port (
      a, b     : in  std_logic_vector(31 downto 0);
      selector : in  std_logic;
      output   : out std_logic_vector(31 downto 0));
  end component;


  component adder
    generic (
      operand_size : integer);
    port (
      operand_A : in  std_logic_vector(operand_size - 1 downto 0);
      operand_B : in  std_logic_vector(operand_size - 1 downto 0);
      sum       : out std_logic_vector(operand_size - 1 downto 0));
  end component;
  
  component rom
    generic (
      words     : integer);
    port (
      addr  : in  std_logic_vector(31 downto 0);
      oe    : in  std_logic;
      data  : out std_logic_vector(63 downto 0);
      ready : out std_logic);
  end component;

  component reg
    generic (
      register_size : integer);
    port (
      data_in      : in  std_logic_vector(register_size - 1 downto 0);
      clk, we, rst : in  std_logic;
      data_out     : out std_logic_vector(register_size - 1 downto 0));
  end component;

  signal adder_8_operand : std_logic_vector(31 downto 0) :=
    "00000000000000000000000000001000";  -- always add 8
                                         -- this is because the processor
                                         -- has to load two instructions
                                         -- per clock cycle

  signal adder_output : std_logic_vector(31 downto 0);
  signal pc_output : std_logic_vector(31 downto 0);
  
  signal npc : std_logic_vector(31 downto 0) :=
    "00000000000000000000000000000000";
  
  signal instruction_read : std_logic_vector(63 downto 0);
  signal data_is_ready : std_logic;
  signal clk_neg : std_logic;
  
begin  -- sch

  pc : reg
    generic map (
    register_size => 32)
    port map (
      data_in => npc,
      clk     => clk,
      we      => pc_write_enable,
      rst     => rst,
      data_out => pc_output);

  adder_instance : adder
    generic map (
      operand_size => 32)
    port map (
      operand_A => adder_8_operand,
      operand_B => pc_output,
      sum       => adder_output);

  mux : mux2x1
    port map (
      a        => adder_output,
      b        => address_after_jump_or_branch,
      selector => jump_or_branch,
      output   => npc);

  memory : rom
    generic map (
      words     => 256)
    port map (
      addr  => pc_output,
      oe    => '0',
      data  => instruction_read,
      ready => data_is_ready);

  new_program_counter <= npc;
  ir_1 <= instruction_read(63 downto 32);
  ir_0 <= instruction_read(31 downto 0);
  clk_neg <= not clk;
  
end sch;
