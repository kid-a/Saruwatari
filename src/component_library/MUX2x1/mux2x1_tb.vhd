library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity mux2x1_tb is
end mux2x1_tb;
-------------------------------------------------------------------------------
architecture test_bench of mux2x1_tb is

 component mux2x1
   port (
     a, b     : in  std_logic_vector(31 downto 0);
     selector : in  std_logic;
     output   : out std_logic_vector(31 downto 0));
 end component;

 signal a, b, output : std_logic_vector(31 downto 0);
 signal selector : std_logic;

begin  -- test_bench


  dut : mux2x1 port map (
    a        => a,
    b        => b,
    selector => selector,
    output   => output);
  
  process
  begin  -- process

    a <= "00000000000000000000000000000001";
    b <= "00000000000000000000000000000010";
    selector <= '0';
    wait for 5 ns;
    a <= "00000000000000000000000000000010";
    b <= "00000000000000000000000000000001";
    wait for 5 ns;
    a <= "00000000000000000000000000000001";
    b <= "00000000000000000000000000000010";
    selector <= '1';
    wait for 5 ns;

  end process;

end test_bench;
