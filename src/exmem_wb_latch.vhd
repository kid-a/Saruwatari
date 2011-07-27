-------------------------------------------------------------------------------
-- id_exmem_latch.vhd
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
entity exmem_wb_latch is 
  port (
    clk               : in  std_logic;
    we		      : in  std_logic;
    rst               : in  std_logic;
    npc_in            : in  std_logic_vector(31 downto 0);
    alu_out_1_in      : in  std_logic_vector(31 downto 0); 	
    alu_out_0_in      : in  std_logic_vector(31 downto 0); 	
    dst_reg_1_in      : in  std_logic_vector(4 downto 0); 
    lmd_in            : in  std_logic_vector(31 downto 0);
    dst_reg_0_in      : in  std_logic_vector(4 downto 0); 
    jal_in	      : in  std_logic;
    mem_read_in	      : in  std_logic;
    wb_enable_1_in    : in  std_logic;
    wb_enable_0_in    : in  std_logic;
    
    npc_out           : out std_logic_vector(31 downto 0);
    alu_out_1_out     : out std_logic_vector(31 downto 0); 	
    alu_out_0_out     : out std_logic_vector(31 downto 0); 	
    dst_reg_1_out     : out std_logic_vector(4 downto 0); 
    dst_reg_jal_out   : out std_logic_vector(4 downto 0); 
    lmd_out           : out std_logic_vector(31 downto 0);
    dst_reg_0_out     : out std_logic_vector(4 downto 0); 
    jal_out	      : out std_logic;
    mem_read_out      : out std_logic;
    wb_enable_1_out   : out std_logic;
    wb_enable_0_out   : out std_logic);
      
end exmem_wb_latch;
---------------------------------------------------------------------------
architecture schem of exmem_wb_latch is

  component reg is
    
    generic (
      register_size : integer := 32);

    port (
      data_in  : in  std_logic_vector(register_size - 1 downto 0);
      clk      : in  std_logic;           -- clock
      we       : in  std_logic;           -- write enable
      rst      : in  std_logic;           -- reset
      data_out : out std_logic_vector(register_size - 1 downto 0));

  end component;

  component flag 
    
    port (
      clk      : in  std_logic;           -- clock
      we       : in  std_logic;           -- write enable
      rst      : in  std_logic;           -- reset
      data_in  : in  std_logic;
      data_out : out std_logic);
    
  end component;


begin  -- schem
  
  npc : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => npc_in,
      data_out => npc_out);

  alu_out_1 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => alu_out_1_in,
      data_out => alu_out_1_out);

  dst_reg_1 : reg
   generic map ( register_size => 5)
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => dst_reg_1_in,
      data_out => dst_reg_1_out);

  dst_reg_0 : reg
   generic map ( register_size => 5)
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => dst_reg_0_in,
      data_out => dst_reg_0_out);

  dst_reg_jal : reg
   generic map ( register_size => 5)
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => "11111",
      data_out => dst_reg_jal_out); 

  alu_out_0 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => alu_out_0_in,
      data_out => alu_out_0_out);

  lmd : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => lmd_in,
      data_out => lmd_out);

  jal   : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => jal_in,
      data_out => jal_out);

 mem_read  : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => mem_read_in,
      data_out => mem_read_out);

 wb_enable_1  : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => wb_enable_1_in,
      data_out => wb_enable_1_out);

wb_enable_0   : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => wb_enable_0_in,
      data_out => wb_enable_0_out);

end schem;

