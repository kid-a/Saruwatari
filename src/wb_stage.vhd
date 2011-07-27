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
entity wb_stage is 
  port (
    npc : in std_logic_vector(31 downto 0);
    alu_out_1 : in std_logic_vector(31 downto 0);
    dst_reg_1 : in std_logic_vector(4 downto 0);
    dst_reg_jal : in std_logic_vector(4 downto 0);
    alu_out_0 : in std_logic_vector(31 downto 0);
    lmd : in std_logic_vector(31 downto 0);
    dst_reg_0 : in std_logic_vector(4 downto 0);
    jal : in std_logic;
    mem_read : in std_logic;
    wb_enable_1 : in std_logic;
    wb_enable_0 : in std_logic;

    reg_file_write_enable_1 : out std_logic;
    reg_file_write_address_1 : out std_logic_vector(4 downto 0);
    reg_file_write_data_1: out std_logic_vector(31 downto 0);
    reg_file_write_enable_0 : out std_logic;
    reg_file_write_address_0 : out std_logic_vector(4 downto 0);
    reg_file_write_data_0: out std_logic_vector(31 downto 0));

end wb_stage;
---------------------------------------------------------------------------
architecture schem of wb_stage is

  component mux2x1 is
    generic (
      port_size : integer := 32);
    port (
      a, b        : in  std_logic_vector(port_size - 1 downto 0);
      selector    : in  std_logic;             -- selector
      output      : out std_logic_vector(port_size - 1 downto 0));
  end component;


begin  -- behavior
 
  mux_data_1 : mux2x1
    port map (
      a        => alu_out_1,
      b        => npc,
      selector => jal,
      output   => reg_file_write_data_1);

  mux_address_1 : mux2x1
    generic map ( port_size => 5)
    port map (
      a        => dst_reg_1,
      b        => dst_reg_jal,
      selector => jal,
      output   => reg_file_write_address_1);

    mux_data_0 : mux2x1
    port map (
      a        => alu_out_0,
      b        => lmd,
      selector => mem_read,
      output   => reg_file_write_data_0);

  reg_file_write_enable_1 <= wb_enable_1;
  reg_file_write_enable_0 <= wb_enable_0;
  reg_file_write_address_0 <= dst_reg_0; 

end schem;

