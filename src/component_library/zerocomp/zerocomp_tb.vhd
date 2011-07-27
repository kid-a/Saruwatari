---------------------------------------------------------------------------
-- Authors   : Loris Fichera A63/000232
-- 	       Daniele Marletta A63/000213
--
-- File      : zerocomp_tb.vhd
--
-- Abstract : 
--
---------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

---------------------------------------------------------------------------
---------------------------------------------------------------------------

entity zerocomp_tb is 
end zerocomp_tb;

architecture test_bench of zerocomp_tb is
  
  component zerocomp port (
    register_in : in std_logic_vector(31 downto 0);
    is_zero : out std_logic );
  end component;
  
  signal register_in : std_logic_vector(31 downto 0);
  signal is_zero : std_logic;
begin  -- behavior

  dut : zerocomp port map(
    register_in => register_in,
    is_zero => is_zero);
  process
  
  begin   -- process
	register_in <= "00000000000000000000000000000000";
	wait for 10 ns;
	register_in <= "00000000000000000000001000000000";
	wait for 10 ns;
 
  end process;  

end test_bench;

