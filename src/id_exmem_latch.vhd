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
entity id_exmem_latch is 
  port (
    clk              	: in  std_logic;  -- clock
    rst               	: in  std_logic;  -- reset
    we                	: in  std_logic;  -- write enable
    
    npc_in            	: in  std_logic_vector(31 downto 0);
    ir_1_in  		: in  std_logic_vector(31 downto 0);
    ir_0_in  		: in  std_logic_vector(31 downto 0);
    immediate_1_in     	: in  std_logic_vector(31 downto 0);  
    immediate_0_in    	: in  std_logic_vector(31 downto 0);
    A1_in 	      	: in  std_logic_vector(31 downto 0);
    B1_in	      	: in  std_logic_vector(31 downto 0);	
    A0_in 	      	: in  std_logic_vector(31 downto 0);
    B0_in              	: in  std_logic_vector(31 downto 0);
    
    -- Control Unit FLAGS
    reg_dst_1_in 	: in  std_logic;
    reg_dst_0_in 	: in  std_logic;
    jal_in   		: in  std_logic; -- datapath 1 
    alu_src_1_in	: in  std_logic; -- I-type LOW, R-type HIGH
    alu_src_0_in	: in  std_logic; -- I-type LOW, R-type HIGH
    mem_read_in		: in  std_logic; -- datapath 0 
    mem_write_in	: in  std_logic; -- datapath 0
    wb_enable_1_in      : in  std_logic;
    wb_enable_0_in      : in  std_logic;
    
    --
    --  Output Signals
    --
    npc_out            	: out  std_logic_vector(31 downto 0);
    ir_1_out  		: out  std_logic_vector(31 downto 0);
    ir_0_out  		: out  std_logic_vector(31 downto 0);
    immediate_1_out     : out  std_logic_vector(31 downto 0);  
    immediate_0_out    	: out  std_logic_vector(31 downto 0);
    A1_out 	      	: out  std_logic_vector(31 downto 0);
    B1_out	      	: out  std_logic_vector(31 downto 0);	
    A0_out 	      	: out  std_logic_vector(31 downto 0);
    B0_out              : out  std_logic_vector(31 downto 0);
    
    -- Control Unit FLAGS
    reg_dst_1_out 	: out  std_logic;
    reg_dst_0_out 	: out  std_logic;
    jal_out   		: out  std_logic; -- datapath 1 
    alu_src_1_out	: out  std_logic; -- I-type LOW, R-type HIGH
    alu_src_0_out	: out  std_logic; -- I-type LOW, R-type HIGH
    mem_read_out	: out  std_logic; -- datapath 0 
    mem_write_out	: out  std_logic;  -- datapath 0
    wb_enable_1_out     : out  std_logic;
    wb_enable_0_out     : out  std_logic);

end id_exmem_latch;
---------------------------------------------------------------------------
architecture schem of id_exmem_latch is

  component reg is
    
    generic (
      register_size : integer := 32);

    port (
      clk      : in  std_logic;  -- clock
      rst      : in  std_logic;  -- reset
      we       : in  std_logic;  -- write enable
      data_in  : in  std_logic_vector(register_size - 1 downto 0);
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


begin 
  npc : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we ,
      data_in  => npc_in,
      data_out => npc_out);
  
  instruction_1 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we ,
      data_in  => ir_1_in,
      data_out => ir_1_out);

  instruction_0 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => ir_0_in,
      data_out => ir_0_out);

  
  immediate_1 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => immediate_1_in,
      data_out => immediate_1_out);

  immediate_0 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => immediate_0_in,
      data_out => immediate_0_out);

  A1 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => A1_in,
      data_out => A1_out);
  B1 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => B1_in,
      data_out => B1_out);

  A0 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => A0_in,
      data_out => A0_out);

  B0 : reg
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => B0_in,
      data_out => B0_out);

  jal : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => jal_in,
      data_out => jal_out);

  alu_src_1 : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => alu_src_1_in,
      data_out => alu_src_1_out);

  alu_src_0 : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => alu_src_0_in,
      data_out => alu_src_0_out);

  reg_dst_1 : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => reg_dst_1_in,
      data_out => reg_dst_1_out);

  reg_dst_0 : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => reg_dst_0_in,
      data_out => reg_dst_0_out);


  mem_read : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => mem_read_in,
      data_out => mem_read_out);

  mem_write : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => mem_write_in,
      data_out => mem_write_out);

  wb_enable_1 : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => wb_enable_1_in,
      data_out => wb_enable_1_out);

  wb_enable_0 : flag
    port map (
      clk      => clk,
      rst      => rst,
      we       => we,
      data_in  => wb_enable_0_in,
      data_out => wb_enable_0_out);

end schem;

