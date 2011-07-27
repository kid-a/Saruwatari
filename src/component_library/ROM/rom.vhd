-------------------------------------------------------------------------------
-- rom.vhd
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
--
-- Chip Select was omitted (it will be the only ROM memory in the system)
--
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use std.textio.all;
use ieee.std_logic_textio.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_signed.all;
-------------------------------------------------------------------------------
entity rom is
  
  generic (
    words     : integer := 256);        -- number of words into the memory

  port (
    addr  : in  std_logic_vector(31 downto 0); -- integer range 0 to words - 1;
    oe    : in  std_logic;                     -- output_enable
                                               -- active low
    data  : out std_logic_vector(63 downto 0);
    ready : out std_logic);                    -- '1' if data ready, '0' otherwise

end rom;
-------------------------------------------------------------------------------
architecture rom of rom is

  type vector_array is array (0 to words - 1)
    of std_logic_vector(31 downto 0);

begin  -- rom
  process (addr,oe)
    variable initialized : boolean := false;
    file memory_data_file : text;
    variable word : line;
    variable memory : vector_array;
    variable i : integer := 0;
    variable program_size : integer := 0;
    variable actual_address : integer := 0;

    --code to read from file
  begin  -- process

    if initialized = false then
--      data <= (others => 'Z');
      file_open(memory_data_file,"mem_dump.dat",READ_MODE);
      while not endfile(memory_data_file) and i < words loop
        readline(memory_data_file, word);
        read(word, memory(i));
        i := i + 1;
        program_size := program_size + 1;
      end loop;

      for i in program_size to (words - 1) loop
        memory(i) := (others => '0');
        
      end loop;  -- i

      initialized := true;
      
    end if;
    file_close(memory_data_file);

    actual_address := conv_integer(addr(31 downto 2));

      if oe = '0' and (actual_address <= words) then
        data(63 downto 32) <= memory(actual_address + 1) after 1 ns;  -- inertial delay
        data(31 downto 0) <= memory(actual_address) after 1 ns;  -- inertial delay
        ready <= '1' after 1 ns;
      else
        data <= (others => '0');
        ready <= '1' after 1 ns;
      end if;
--    end if;
    
  end process;
end rom;
