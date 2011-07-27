---------------------------------------------------------------------------
-- Authors   : Loris Fichera A63/000232
-- 	       Daniele Marletta A63/000213
--
-- File      : flag_tb.vhd
--
-- Abstract : 
--
---------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

---------------------------------------------------------------------------

entity flag_tb is 
end flag_tb;

---------------------------------------------------------------------------

architecture test_bench of flag_tb is

  component flag
    port (
      clk      : in  std_logic;           -- clock
      we       : in  std_logic;           -- write enable
      rst      : in  std_logic;           -- reset
      data_in  : in  std_logic;
      data_out : out std_logic);
  end component;

  signal data_in, data_out : std_logic;
  signal clk, rst, we: std_logic;

begin  -- test_bench

  dut : flag
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
    data_in <= '1';
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
    data_in <= '0';
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

