library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity reg_tb is
end reg_tb;
-------------------------------------------------------------------------------
architecture test_bench of reg_tb is
  
component reg
  generic (
    register_size : integer := 32);           -- 32
  port (
    data_in  : in  std_logic_vector(register_size - 1 downto 0);
    clk      : in  std_logic;
    rst      : in  std_logic;
    we       : in  std_logic;
    data_out : out std_logic_vector(register_size - 1 downto 0));
end component;

  signal data_in, data_out : std_logic_vector(31 downto 0);
  signal clk, rst, we: std_logic;

begin  -- test_bench

  dut : reg
    port map (
      data_in  => data_in,
      data_out => data_out,
      clk      => clk,
      rst      => rst,
      we       => we);

  process
  begin  -- process
    -- let's put a value into the register
    we <= '1';
    rst <= '0';
    data_in <= "00000000000000000000000011000000";
    wait for 10 ns;
    -- now hold it for 10 ns
    we <= '0';
    wait for 13 ns;
    -- reset the register
--    we <= '0';
    rst <= '1';
    wait for 10 ns;
    -- put 111 into the register
    we <= '1';
    rst <= '0';
    data_in <= "00000000000000000000000000000111";
    wait for 10 ns;
  end process;

  process
  begin  -- clock generating process
    clk <= '1';
    wait for 5 ns;
    clk <= '0';
    wait for 5 ns;
  end process;

end test_bench;
