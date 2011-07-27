---------------------------------------------------------------------------
-- Authors   : Loris Fichera A63/000232
-- 	       Daniele Marletta A63/000213
--
-- File      : adder_tb.vhd
--
-- Abstract : 
--
---------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

---------------------------------------------------------------------------

entity adder_tb is 
end adder_tb;

---------------------------------------------------------------------------

architecture test_bench of adder_tb is
  
  component adder 
    generic(
      operand_size : integer := 32);
    port (
      operand_A : in std_logic_vector( operand_size -1 downto 0);
      operand_B : in std_logic_vector( operand_size -1 downto 0);
      sum	: out std_logic_vector( operand_size -1 downto 0) 
    );
  end component; 

  signal operand_A : std_logic_vector(31 downto 0) := 
    "00000000000000000000000000001000";
  signal operand_B : std_logic_vector(31 downto 0);
  signal sum : std_logic_vector(31 downto 0);

begin  -- behavior
  
  dut : adder port map(
    operand_A => operand_A,
    operand_B => operand_B,
    sum => sum);

  process
  begin   -- process
    operand_B <= "00000000000000000000000000001000";
    wait for 10 ns;
    operand_B <= "00000000000000000000000000000000";
    wait for 10 ns;
    operand_B <= "00000000000000000000000000000111";
    wait for 10 ns;
    operand_B <= "00000000000000000000000000000001";
    wait for 10 ns;
   end process;  

end test_bench;

