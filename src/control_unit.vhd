-------------------------------------------------------------------------------
-- control_unit.vhd
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
-- when editing this design BE CAREFUL about all the possible opcodes that may
-- come in.
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity control_unit is

  port (
    -- add input for zerocomp
    clk                    : in  std_logic;
    rst                    : in  std_logic;
    opcode_1               : in  std_logic_vector(5 downto 0);
    opcode_0               : in  std_logic_vector(5 downto 0);
    branch_operand_is_zero : in  std_logic;

--    if_id_latch_rst        : out std_logic;  -- flushes the first pipeline register
--    pc_write_enable        : out std_logic;
    alu_src_1              : out std_logic;  -- R-type HIGH;    I-type LOW
    alu_src_0              : out std_logic;  -- R-type HIGH;    I-type LOW 
    reg_dst_1              : out std_logic;  -- R-type HIGH;    I-type LOW   
    reg_dst_0              : out std_logic;  -- R-type HIGH;    I-type LOW 
    jump                   : out std_logic;  -- active if instruction_1 is a jump
    jal                    : out std_logic;  -- active if instruction_1 is a jal
    branch_taken           : out std_logic;  -- active if branch was taken
    mem_read               : out std_logic;  -- active if instruction_0 is a LW
    mem_write              : out std_logic;  -- active if instruction_0 is a SW
    wb_enable_1		   : out std_logic;  -- active if instruction_1 writes on regfiles
    wb_enable_0		   : out std_logic); -- active if instruction_0 writes on regfiles
  
end control_unit;
-------------------------------------------------------------------------------
architecture rtl of control_unit is

  type control_hazard_state is (idle, hazard);
  signal pr_state : control_hazard_state;
  signal nx_state : control_hazard_state;
  
begin  -- rtl

--  if_id_latch_rst <= '0';
--  pc_write_enable <= '1';
  
  process (opcode_1, branch_operand_is_zero)
  begin  -- process
    case opcode_1(5 downto 4) is
      when  "00" =>                     -- I-type
        case opcode_1(3 downto 0) is
          when "0000" =>               -- nop
            alu_src_1  <= '0';      
            reg_dst_1  <= '0'; 
            jump       <= '0';
            jal        <= '0';
            branch_taken <= '0';   
            wb_enable_1   <= '0';
          when "0101" =>              -- beqz
	    if branch_operand_is_zero = '1' then
              alu_src_1  <= '0';      
              reg_dst_1  <= '0'; -- don't care
              jump       <= '0';
              jal        <= '0';
              branch_taken <= '1';
              wb_enable_1   <= '0';
      	    elsif branch_operand_is_zero = '0' then
	      alu_src_1  <= '0';      
              reg_dst_1  <= '0'; -- don't care
              jump       <= '0';
              jal        <= '0';
              branch_taken <= '0';
              wb_enable_1   <= '0';
      	    else 
              assert false
                report "BEQZ: Signal branch_operand_is_zero is not valid";
      	    end if;
            
          when "0110" =>              -- bnqz
	    if branch_operand_is_zero = '0' then
              alu_src_1  <= '0';      
              reg_dst_1  <= '0'; -- don't care
              jump       <= '0';
              jal        <= '0';
              branch_taken <= '1';
              wb_enable_1 <= '0';
      	    elsif branch_operand_is_zero = '1' then
	      alu_src_1  <= '0';      
              reg_dst_1  <= '0'; -- don't care
              jump       <= '0';
              jal        <= '0';
              branch_taken <= '0';
              wb_enable_1 <= '0';
      	    else 
              assert false
                report "BNQZ: Signal branch_operand_is_zero is not valid";
      	    end if;
          when others =>                -- other  I-type instructions
            alu_src_1  <= '0';      
            reg_dst_1  <= '0';
            jump       <= '0';
            jal        <= '0';
            branch_taken <= '0';   
            wb_enable_1 <= '1';
        end case;
        
      when "01" =>                      -- R-type
        alu_src_1 <= '1';      
        reg_dst_1 <= '1';
        jump <= '0';
        jal <= '0';
        branch_taken <= '0';
        wb_enable_1 <= '1';
        
      when "10" =>			-- J-type
        case opcode_1(3 downto 0) is
          when "0000"  =>	--jump
            alu_src_1 <= '0'; --don't care      
            reg_dst_1 <= '0'; -- don't care
            jump <= '1';
            jal <= '0';
            branch_taken <= '0';
            wb_enable_1 <= '0';

          when "0001" =>  --jal
            alu_src_1 <= '0'; --don't care      
            reg_dst_1 <= '0'; -- don't care
            jump <= '0';
            jal <= '1';
            branch_taken <= '0';
            wb_enable_1 <= '1';

          when others =>
            assert true report "Invalid J-Type Instruction";
        end case;  -- J case
        
      when others =>
	assert true report "Invalid Opcode in Instruction 1";
    end case;  -- Opcode case
  end process;


  
  -- Datapath 0
  process (opcode_0)
  begin  -- process
    case opcode_0(5 downto 4) is
      when  "00" =>                     -- I-type
	case opcode_0(3 downto 0) is

          when "0000" =>               -- nop
            alu_src_0  <= '0';      
            reg_dst_0  <= '0'; 
            mem_read <= '0';
            mem_write <= '0';
            wb_enable_0 <= '0';
            
          when "0011" =>	-- LW
            alu_src_0  <= '0';
            reg_dst_0  <= '0'; 
            mem_read <= '1';
            mem_write <= '0';
            wb_enable_0 <= '1';
            
          when "0100" => --SW
            alu_src_0  <= '0';
            reg_dst_0  <= '0'; -- don't care
            mem_read <= '0';
            mem_write <= '1';
            wb_enable_0 <= '0';
            
          when others => 
            alu_src_0  <= '0';
            reg_dst_0  <= '0'; -- don't care
            mem_read <= '0';
            mem_write <= '0';
            wb_enable_0 <= '1';
	end case;

      when  "01" =>		-- R-Type
        alu_src_0  <= '1';
        reg_dst_0  <= '1'; -- don't care
        mem_read <= '0';
        mem_write <= '0';
        wb_enable_0 <= '1';
        
      when others =>
	assert false
          report "Invalid Opcode in Instruction 0";
    end case;
  end process;    
end rtl;
