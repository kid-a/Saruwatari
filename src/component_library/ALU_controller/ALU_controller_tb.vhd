---------------------------------------------------------------------------
-- Authors   : Loris Fichera A63/000232
-- 	       Daniele Marletta A63/000213
--
-- File      : ALU_controller_tb.vhd
--
-- Abstract : 
--
---------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

---------------------------------------------------------------------------

entity ALU_controller_tb is 
end ALU_controller_tb;

---------------------------------------------------------------------------

architecture test_bench of ALU_controller_tb is

  component ALU_controller port (
    opcode : in std_logic_vector(5 downto 0);
    func   : in std_logic_vector(10 downto 0);
    alu_opcode : out std_logic_vector(3 downto 0));
  end component;

  signal opcode : std_logic_vector(5 downto 0);
  signal func   : std_logic_vector(10 downto 0);
  signal alu_opcode : std_logic_vector(3 downto 0);
 
begin  -- behavior
  
  dut : ALU_controller port map(
    opcode => opcode,
    func => func,
    alu_opcode => alu_opcode);

  process
  begin   -- process
    func <= "00000000000";
    opcode <= "000001";
    wait for 5 ns;
    opcode <= "000010";
    wait for 5 ns;
    opcode <= "000111";
    wait for 5 ns;
    opcode <= "010000";
    func <= "00000000001";  
    wait for 5 ns;
    opcode <= "010000";
    func <= "00000000010";  
    wait for 5 ns;
    opcode <= "010000";
    func <= "00000000011";  
    wait for 5 ns;
    opcode <= "010000";
    func <= "00000000100";  
    wait for 5 ns;
    opcode <= "010000";
    func <= "00000000101";  
    wait for 5 ns;
    opcode <= "010000";
    func <= "00000000110";  
    wait for 5 ns;
    opcode <= "100000";
    wait for 5 ns;
    opcode <= "100001";
    wait for 5 ns;
    
  end process;  

end test_bench;

