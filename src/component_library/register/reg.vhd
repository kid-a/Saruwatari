-------------------------------------------------------------------------------
-- reg.vhd
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
-- Generic register VHDL design
--
-- To work properly, this register needs two control signals:
-- we (write enable) and rst (reset). Both of them are active high.
-- The following chart shows how to set we and rst in order to
-- drive the register:
--
--  state              |  we | rst |
--  -------------------+-----+-----+
--  store a new value  |   1 |  0  |
--  hold current value |   0 |  0  |
--  put zeros in output|   _ |  1  |
--  -------------------+-----+-----+
--
--  where _ means "don`t care".
--
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity reg is
  
  generic (
    register_size : integer := 32);

  port (
    data_in  : in  std_logic_vector(register_size - 1 downto 0);
    clk      : in  std_logic;           -- clock
    we       : in  std_logic;           -- write enable
    rst      : in  std_logic;           -- reset
    data_out : out std_logic_vector(register_size - 1 downto 0));

end reg;
-------------------------------------------------------------------------------
architecture beh of reg is
  
begin  -- beh

  process(clk, rst)
    
  begin  -- process

--    if clk'event and clk = '1' and rst = '1' then 	 
--      data_out <= (others => '0') after 10 ps;
    if rst = '1' then
      data_out <= (others => '0') after 300 ps;
    elsif clk'event and clk = '1' and we = '1' then
      data_out <= data_in after 300 ps;
    end if;
     
--    if clk'event and clk = '1' then
--      if rst = '1' then
--        data_out <= (others => '0') after 10 ps;
--      elsif we = '1' then
--        data_out <= data_in after 10 ps;
--      end if;
--    end if;
--      
  end process;

end beh;
