-------------------------------------------------------------------------------
-- ALU_controller.vhd
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
entity ALU_controller is 
  port (
    opcode : in std_logic_vector(5 downto 0);
    func   : in std_logic_vector(10 downto 0);
    alu_opcode : out std_logic_vector(3 downto 0) 
  );
end ALU_controller;
---------------------------------------------------------------------------
architecture rtl of ALU_controller is

begin  -- behavior

  -- purpose: alu control
  -- type   : combinational
  -- inputs : opcode
  -- outputs: 
  process (opcode, func)
  begin  -- process
    case opcode(5 downto 4) is
      when "00" =>                      -- I-type
        
        case opcode(3 downto 0) is
          when "0001" => alu_opcode <= "0001";  -- addi
          when "0010" => alu_opcode <= "0010";  -- subi
          when "0011" => alu_opcode <= "0111";  -- lw
          when "0100" => alu_opcode <= "0111";  -- sw
          when "0101" => alu_opcode <= "0001";  -- beqz
          when "0110" => alu_opcode <= "0001";  -- bnqz
          when "0111" => alu_opcode <= "0011";  -- multi
          when others => 
            alu_opcode <= "0000";
            assert false report "Invalid I-type instruction" severity note;
        end case;
        
      when "01" =>
        case func is
          when "00000000001" => alu_opcode <= "0001";  -- add
          when "00000000010" => alu_opcode <= "0010";  -- sub
          when "00000000011" => alu_opcode <= "0011";  -- mult
          when "00000000100" => alu_opcode <= "0101";  -- slt
          when "00000000101" => alu_opcode <= "0110";  -- sgt
          when "00000000110" => alu_opcode <= "0100";  -- div
          when others => 
            alu_opcode <= "0000";
            assert false
              report "Invalid R-type instruction"
            severity note;
        end case;
        
      when "10" =>
        case opcode(3 downto 0) is
          when "0000" => alu_opcode <= "0000";  -- jump
          when "0001" => alu_opcode <= "0001";  -- jal
          when others => 
            alu_opcode <= "0000";
            assert false
              report "Invalid J-type instruction"
              severity note;
        end case;
        
      when others => 
     	assert false
          report "Invalid opcode instruction"
        severity error;
    end case;
  end process;
end rtl;

