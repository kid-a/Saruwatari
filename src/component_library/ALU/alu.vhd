-------------------------------------------------------------------------------
-- alu.vhd
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
-- This ALU is able
-- to perform the following operations:
--
--  operation|   op|
--  ---------+-----+
--  sum      | 0001|
--  sub      | 0010|
--  mult     | 0011|
--  div      | 0100|
--  slt      | 0101|
--  sgt      | 0110|
--  addr_calc| 0111| it is implemented as a sum
--  ---------+-----+
--
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_signed.all;
use ieee.std_logic_arith.all;
-------------------------------------------------------------------------------
entity alu is
  
  port (
    a, b       : in  std_logic_vector(31 downto 0);
    op         : in  std_logic_vector(3 downto 0);  -- opcode
    clk        : in  std_logic;
    rst        : in  std_logic;
    data_ready : out std_logic; 
    result     : out std_logic_vector(31 downto 0));

end alu;
-------------------------------------------------------------------------------
architecture alu of alu is

  type alu_state is (computing, idle);
  signal pr_state : alu_state;
  signal nx_state : alu_state;

begin  -- alu
  
--   sequential section
  process (clk, rst)    
  begin  -- process
    
    if rst = '1' then
      pr_state <= idle;
      
    elsif clk'event and clk = '1' then  -- rising clock edge
      pr_state <= nx_state;
    end if;

  end process;

-- combinational section
  process (a, b, op, pr_state)
    variable mult_result : std_logic_vector(63 downto 0);
    variable div_operand_a : integer;
    variable div_operand_b : integer;
    variable div_result_integer : integer;
    variable div_result_vector : std_logic_vector(31 downto 0);
    
  begin  -- process

    if a'event or b'event or op'event then        
          
          case op is
            when "0000" =>                -- nop
              result <= (others => '0');
              data_ready <= '1';
              nx_state <= idle;
              
            when "0001" =>                    -- sum
              result <= a + b;
              data_ready <= '1';
              nx_state <= idle;
              
            when "0010" =>                    -- sub
              result <= a - b;
              data_ready <= '1';
              nx_state <= idle;
              
            when "0011" =>                    -- mult
              data_ready <= '0';
              nx_state <= computing;
              
            when "0100" =>                    -- div
              data_ready <= '0';
              nx_state <= computing;
              
            when "0101" =>                    -- slt
              if a < b then
                result <= "00000000000000000000000000000001";
              else
                result <= (others => '0');
              end if;
              data_ready <= '1';
              nx_state <= idle;
              
            when "0110" =>                    -- sgt
              if a > b then
                result <= "00000000000000000000000000000001";
              else
                result <= (others => '0');
              end if;
              data_ready <= '1';
              nx_state <= idle;

            when "0111" =>
              result <= a + b;
              data_ready <= '0';
              nx_state <= computing;
              
            when others =>                    -- otherwise
              result <= (others => '0');
              data_ready <= '1';
              nx_state <= idle;
              assert false report "ERROR: Invalid opcode detected.";
          end case;          
      end if;

      if pr_state'event then
        case pr_state is          

          when computing =>
          case op is
            when "0011" =>
              mult_result :=  a * b;
              result <= mult_result(31 downto 0) after 6 ns;
              nx_state <= idle;
              data_ready <= '1' after 6 ns;
              
            when "0100" =>
              div_operand_a := conv_integer(signed(a));
              div_operand_b := conv_integer(signed(b));
              if ( div_operand_b /= 0 ) then
                div_result_integer := div_operand_a / div_operand_b;
                div_result_vector := conv_std_logic_vector(div_result_integer, 32);
                result <= div_result_vector after 6 ns;
              else
                assert FALSE report
                  "ERROR: Division by zero detected."
                  severity ERROR;
              end if;
              nx_state <= idle;
              data_ready <= '1' after 6 ns;

            when "0111" =>
              nx_state <= idle;
              data_ready <= '1' after 6 ns;
              
            when others =>
              assert FALSE report
                "ERROR: Invalid multicycle operation detected."
                severity ERROR;
          end case;

          when others =>
            assert false report "NOTE: Transition from IDLE to IDLE after a mulicycle operation."
              severity note;
        end case;
      end if;
                
  end process;
end alu;
