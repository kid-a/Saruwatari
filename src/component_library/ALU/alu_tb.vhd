library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
-------------------------------------------------------------------------------
entity alu_tb is
end alu_tb;
-------------------------------------------------------------------------------
architecture test_bench of alu_tb is

  component alu
    port (
      a, b       : in  std_logic_vector(31 downto 0);
      op         : in  std_logic_vector(3 downto 0);
      clk        : in  std_logic;
      rst        : in  std_logic;
      data_ready : out std_logic;
      result     : out std_logic_vector(31 downto 0));
  end component;

  signal a, b, result : std_logic_vector(31 downto 0);
  signal data_ready : std_logic;
  signal op : std_logic_vector(3 downto 0);
  signal clk : std_logic;
  signal rst : std_logic := '1';
  
begin  -- test_bench

  dut : alu port map (
    a          => a,
    b          => b,
    op         => op,
    clk        => clk,
    rst        => rst,
    data_ready => data_ready,
    result     => result);

  rst <= '0' after 3 ns;

  -- purpose: testing alu
  -- type   : combinational
  -- inputs : a, b, op
  -- outputs: result
  process
  begin  -- process
  wait for 3 ns;
    a  <= "00000000000000000000000000000000";
    b  <= "00000000000000000000000000000001";
    op <= "0001";
    wait for 20 ns;
    op <= "0000";
    wait for 20 ns;
    a  <= "00000000000000000000000000000011";
    b  <= "00000000000000000000000000000001";
    op <= "0010";
    wait for 20 ns;
    a  <= "00000000000000000000000000000011";
    b  <= "00000000000000000000000000000011";
    op <= "0001";
    wait for 20 ns;
    op <= "0010";
    wait for 20 ns;
    op <= "0100";
    wait for 20 ns;
    a  <= "00000000000000000000000000001001";
    wait for 20 ns;
    op <= "0011";
    wait for 20 ns;
    op <= "0110";
    wait for 20 ns;
  end process;


  process
  begin  -- process
    clk <= '1';
    wait for 5 ns;
    clk <= '0';
    wait for 5 ns;
  end process;
  

end test_bench;
