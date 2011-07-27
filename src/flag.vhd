-------------------------------------------------------------------------------
-- flag.vhd
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
entity flag is 

  port (
    clk      : in  std_logic;           -- clock
    we       : in  std_logic;           -- write enable
    rst      : in  std_logic;           -- reset
    data_in  : in  std_logic;
    data_out : out std_logic);

end flag;
-------------------------------------------------------------------------------
architecture rtl of flag is

begin  -- behavior
  process(clk,rst)
    
  begin  -- process

    if clk'event and clk = '1' and rst = '1' then 	 
      data_out <= '0' after 10 ps;
    else
      if clk'event and clk = '1' and we = '1' then
        data_out <= data_in after 10 ps;
      end if;
    end if;
     
 end process;

end rtl;

