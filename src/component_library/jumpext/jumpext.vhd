-------------------------------------------------------------------------------
-- jumpext.vhd
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
entity jumpext is 
  port (
    data_in : in std_logic_vector(25 downto 0);
    data_out : out std_logic_vector(31 downto 0)
  );
end jumpext;
-------------------------------------------------------------------------------
architecture rtl of jumpext is

begin  -- behavior

  process(data_in)
  
  begin   -- process
   data_out(25 downto 0) <= data_in(25 downto 0) after 10 ps;
   data_out(31 downto 26) <= (others => '0') after 10 ps;
--			  
  end process;  

end rtl;

