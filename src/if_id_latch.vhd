-------------------------------------------------------------------------------
-- if_id_latch.vhd
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
entity if_id_latch is
  
  port (
    clk               : in  std_logic;
    rst               : in  std_logic;
    we	   	      : in  std_logic;
    npc_in            : in  std_logic_vector(31 downto 0);
    ir_1_in  : in  std_logic_vector(31 downto 0);
    ir_0_in  : in  std_logic_vector(31 downto 0);
    npc_out           : out std_logic_vector(31 downto 0);
    ir_1_out : out std_logic_vector(31 downto 0);
    ir_0_out : out std_logic_vector(31 downto 0));

end if_id_latch;
-------------------------------------------------------------------------------
architecture schem of if_id_latch is

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
  
begin  -- schem

  npc : reg
    port map (
      data_in  => npc_in,
      clk      => clk,
      we       => we,
      rst      => rst,
      data_out => npc_out);

  instruction_1 : reg
    port map (
      data_in  => ir_1_in,
      clk      => clk,
      we       => we,
      rst      => rst,
      data_out => ir_1_out);

  instruction_0 : reg
    port map (
      data_in  => ir_0_in,
      clk      => clk,
      we       => we,
      rst      => rst,
      data_out => ir_0_out);
  
end schem;
