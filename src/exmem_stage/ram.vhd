-------------------------------------------------------------------------------
-- ram.vhd
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
use ieee.std_logic_arith.all;
use ieee.std_logic_signed.all;
-------------------------------------------------------------------------------
entity ram is
  
  generic (
    words     : integer := 256);         -- number of words in the memory

  port (
    write_enable : in  std_logic;
    clk          : in  std_logic;
    addr         : in  std_logic_vector(31 downto 0);
    data_in      : in  std_logic_vector(31 downto 0);
    data_out     : out std_logic_vector(31 downto 0));

end ram;
-------------------------------------------------------------------------------
architecture ram of ram is

  type vector_array is array (0 to words - 1) of
    std_logic_vector(31 downto 0);
  
  signal memory : vector_array;

  signal mem_0, mem_1, mem_2, mem_3, mem_4, mem_5, mem_6, mem_7, mem_8, mem_9 : std_logic_vector(31 downto 0);
  signal mem_10, mem_11, mem_12, mem_13, mem_14, mem_15, mem_16, mem_17, mem_18 : std_logic_vector(31 downto 0);

begin  -- ram

  process (clk)
    variable initialized : boolean := false;
    
  begin  -- process
    if initialized = false then

    -------------------------------------------------------------------------
    -- default initialization
    -------------------------------------------------------------------------
--      for i in 0 to (words - 1) loop
--      memory(i) <= (others => '0');
--      end loop;


    -------------------------------------------------------------------------
    -- inizialize with vector of integers 0 to 9
    -------------------------------------------------------------------------
      for i in 0 to 9 loop
        memory(i) <= conv_std_logic_vector(i,32);
      end loop;  -- i

      for i in 10 to (words - 1) loop
        memory(i) <= (others => '0');
      end loop;  -- i
            
      initialized := true;
    end if;
    
    if clk'event and clk = '1' then  -- rising clock edge
      if write_enable = '1' then
        memory(conv_integer(addr(31 downto 2))) <= data_in after 600 ps;
      end if;
    end if;
  end process;

  process (addr, memory)
    variable actual_address : integer;
  begin  -- process

    actual_address := conv_integer(addr(31 downto 2));

    if actual_address <= (words - 1) then
      data_out <= memory(actual_address) after 400 ps;

    else
      data_out <= (others => '0');
      
    end if;
  end process;

  -----------------------------------------------------------------------------
  -- internal signals assignment (for debug only)
  -----------------------------------------------------------------------------  
  mem_0 <= memory(0);
  mem_1 <= memory(1);
  mem_2 <= memory(2);
  mem_3 <= memory(3);
  mem_4 <= memory(4);
  mem_5 <= memory(5);
  mem_6 <= memory(6);
  mem_7 <= memory(7);
  mem_8 <= memory(8);
  mem_9 <= memory(9);
  mem_10 <= memory(10);
  mem_11 <= memory(11);
  mem_12 <= memory(12);
  mem_13 <= memory(13);
  mem_14 <= memory(14);
  mem_15 <= memory(15);
  mem_16 <= memory(16);
  mem_17 <= memory(17);
  mem_18 <= memory(18);
  
end ram;
