-------------------------------------------------------------------------------
-- adder.vhd
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
entity adder is 
  generic(
    operand_size : integer := 32);
  port (
    operand_A : in std_logic_vector(operand_size - 1 downto 0);
    operand_B : in std_logic_vector(operand_size - 1 downto 0);
    sum	      : out std_logic_vector(operand_size - 1 downto 0) 
  );
end adder;
-------------------------------------------------------------------------------
architecture behavior of adder is

begin  -- behavior

  sum <= operand_A + operand_B after 10 ps;

end behavior;

