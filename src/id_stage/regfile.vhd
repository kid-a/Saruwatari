-------------------------------------------------------------------------------
-- regfile.vhd
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
use ieee.std_logic_unsigned.all;
-------------------------------------------------------------------------------
entity regfile is 
  port (
    read_01	: in std_logic_vector(4 downto 0);
    read_02	: in std_logic_vector(4 downto 0);
    read_11	: in std_logic_vector(4 downto 0);
    read_12	: in std_logic_vector(4 downto 0);

    write_0 	: in std_logic_vector(4 downto 0);
    write_1 	: in std_logic_vector(4 downto 0);
    we_0	: in std_logic;	   -- write enable
    we_1	: in std_logic;	   -- write enable
    
    data_in_0 	: in std_logic_vector(31 downto 0);
    data_in_1 	: in std_logic_vector(31 downto 0);
 
    A0 		: out std_logic_vector(31 downto 0);
    B0		: out std_logic_vector(31 downto 0);	
    A1 		: out std_logic_vector(31 downto 0);
    B1 	        : out std_logic_vector(31 downto 0);	
    
    clk      	: in  std_logic;    -- clock
    rst      	: in  std_logic    -- reset
    );
end regfile;
-------------------------------------------------------------------------------
architecture behavioral of regfile is

  type vector_array_32 is array (31 downto 0)
    of std_logic_vector(31 downto 0);

  signal register_file : vector_array_32;
  signal reg_00 : std_logic_vector(31 downto 0);
  signal reg_01 : std_logic_vector(31 downto 0);
  signal reg_02 : std_logic_vector(31 downto 0);
  signal reg_03 : std_logic_vector(31 downto 0);
  signal reg_04 : std_logic_vector(31 downto 0);
  signal reg_31 : std_logic_vector(31 downto 0);
begin 
  A0 <= register_file(conv_integer(read_01)) after 10 ps;
  B0 <= register_file(conv_integer(read_02)) after 10 ps;
  A1 <= register_file(conv_integer(read_11)) after 10 ps;
  B1 <= register_file(conv_integer(read_11)) after 10 ps;

  reg_00 <= register_file(0);
  reg_01 <= register_file(1);
  reg_02 <= register_file(2);
  reg_03 <= register_file(3);
  reg_04 <= register_file(4);
  reg_31 <= register_file(31);

  process (we_0,write_0,data_in_0,we_1,write_1,data_in_1,clk,rst)
  
  begin
    
    if rst = '1' then
      for i in 0 to 31 loop
        register_file(i) <= (others => '0') after 300 ps;
      end loop;
    else
	    -- write on descending edge
      if clk'event and clk = '1' and we_0 = '1' then
        if write_0 /= "00000" then
          register_file(conv_integer(write_0)) <= data_in_0 after 300 ps;
        end if;
      end if;	
      if clk'event and clk = '1' and we_1 = '1' then
        if write_1 /= "00000" then
          register_file(conv_integer(write_1)) <= data_in_1 after 300 ps;
        end if;
      end if;	
    end if;

end process;	
end architecture behavioral;

