library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity rom_tb is
end rom_tb;
-------------------------------------------------------------------------------
architecture tb of rom_tb is

  component rom is
    
    generic (
--      word_size : integer := 32;          
      words     : integer := 256);        -- number of words into the memory

    port (
      addr  : in  std_logic_vector(31 downto 0);
      oe    : in  std_logic;              -- output_enable
                                          -- active low
      data  : out std_logic_vector(63 downto 0);
      ready : out std_logic);             -- '1' if data ready, '0' otherwise

  end component;

  signal address : std_logic_vector(31 downto 0);
  signal oe, ready : std_logic;
  signal data : std_logic_vector(63 downto 0);
  
begin  -- tb

  dut : rom generic map (
    words     => 256)
    port map (
      addr  => address,
      oe    => oe,
      data  => data,
      ready => ready);

  

  process
  begin  -- process

    address <= "00000000000000000000000000000000";
    oe <= '0';
    wait for 10 ns;
    address <= "00000000000000000000000000001000";
    oe <= '0';
    wait for 10 ns;
    address <= "00000000000000000000000000010000";
    oe <= '0';
    wait for 10 ns;
    address <= "00000000000000000000000000011000";
    oe <= '0';
    wait for 10 ns;
    address <= "00000000000011000000000000011000";
    oe <= '0';
    wait for 10 ns;
        
  end process;
end tb;
