library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity ram_tb is
end ram_tb;
-------------------------------------------------------------------------------
architecture schem of ram_tb is

component ram is
  
  generic (
    words     : integer := 256);         -- number of words in the memory

  port (
    write_enable : in  std_logic;
    clk          : in  std_logic;
    addr         : in  std_logic_vector(31 downto 0);
    data_in      : in  std_logic_vector(31 downto 0);
    data_out     : out std_logic_vector(31 downto 0));

end component;

signal write_enable : std_logic;
signal clk : std_logic;
signal addr : std_logic_vector(31 downto 0);
signal data_in : std_logic_vector(31 downto 0);
signal data_out : std_logic_vector(31 downto 0);
  
begin  -- schem

  dut : ram
    port map (
      write_enable => write_enable,
      clk          => clk,
      addr         => addr,
      data_in      => data_in,
      data_out     => data_out);
  
  -- purpose: signal generating process
  -- type   : combinational
  -- inputs : 
  -- outputs: 
  process
  begin  -- process
    write_enable <= '0';
    data_in <= "00000000000000000000000000000100";
    addr <= "00000000000000000000000000000010";
    wait for 3 ns;
    write_enable <= '1';
    wait  for 8 ns;
    write_enable <= '0';
    wait for 15 ns;
  end process;
  
  -- purpose: clock generating process
  -- type   : combinational
  -- inputs : 
  -- outputs: 
  process
  begin  -- process
    clk <= '1';
    wait for 5 ns;
    clk <= '0';
    wait for 5 ns;
  end process;

end schem;
