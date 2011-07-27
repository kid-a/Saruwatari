---------------------------------------------------------------------------
-- Authors   : Loris Fichera <loris.fichera@gmail.com>
-- 	       Daniele Marletta <danielemar86@gmail.com>
-- Creation Date : 05/03/2010
-- File          : regfile_tb.vhd
--
-- Abstract : 
--
---------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
---------------------------------------------------------------------------


entity regfile_tb is 
end regfile_tb;
---------------------------------------------------------------------------


Architecture regfile_tb of regfile_tb is

  component regfile is 
    port (
      read_01	: in std_logic_vector(4 downto 0);
      read_02	: in std_logic_vector(4 downto 0);
      read_11	: in std_logic_vector(4 downto 0);
      read_12	: in std_logic_vector(4 downto 0);
  
      write_0 	: in std_logic_vector(4 downto 0);
      write_1 	: in std_logic_vector(4 downto 0);
      we_0	: in std_logic;	   -- write enable
      we_1	: in std_logic;	   -- write enable
      
      data_in_0 	: in std_logic_vector(31 downto 0);
      data_in_1 	: in std_logic_vector(31 downto 0);
   
      A0 		: out std_logic_vector(31 downto 0);
      B0		: out std_logic_vector(31 downto 0);	
      A1 		: out std_logic_vector(31 downto 0);
      B1 	        : out std_logic_vector(31 downto 0);	
      
      clk      	: in  std_logic;    -- clock
      rst      	: in  std_logic    -- reset
      );
  end component;
 
  signal read_01, read_02, read_11, read_12 : std_logic_vector(4 downto 0);
  signal write_0, write_1 : std_logic_vector(4 downto 0);
  signal data_in_0, data_in_1, A0, B0, A1, B1 : std_logic_vector(31 downto 0);
  signal we_0, we_1, clk, rst : std_logic;
 

begin
  dut : regfile
    port map (  
      read_01	=>	read_01,      
      read_02	=>	read_02,
      read_11	=>	read_11,
      read_12	=>	read_12,
  
      write_0 	=>	write_0,
      write_1 	=>	write_1,
      we_0	=>	we_0,
      we_1	=>      we_1,
      data_in_0 =>	data_in_0,
      data_in_1 =>	data_in_1,
   
      A0 	=>	A0,
      B0	=>	B0,
      A1 	=>	A1,
      B1 	=>	B1,
      
      clk      	=>	clk,	
      rst       =>	rst
    ); 

process 
  begin
    rst <= '0';
    we_0 <= '0';
    we_1 <= '0';
    data_in_0 <= "00000000000000000000000000000111";
    data_in_1 <= "00000000000000000000000000001000";
    wait for 10 ns;
    write_0 <= "00011";
    write_1 <= "00101";
    we_0 <= '1';
    we_1 <= '1';
    wait for 10 ns;
    we_0 <= '0';
    we_1 <= '0';
    read_01 <= "00001";
    read_11 <= "00011";
    wait for 13 ns;
    rst <= '1';
    wait for 7 ns;
end process;
process
  begin  -- clock generating process
    clk <= '1';
    wait for 5 ns;
    clk <= '0';
    wait for 5 ns;
  end process;


end regfile_tb;

