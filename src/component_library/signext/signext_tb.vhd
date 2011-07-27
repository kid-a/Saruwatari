---------------------------------------------------------------------------
-- Authors   : Loris Fichera <loris.fichera@gmail.com>
-- 	       Daniele Marletta <danielemar86@gmail.com>
-- Creation Date : 17/02/2010
-- File          : signext_tb.vhd
--
-- Abstract : 
--
---------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
---------------------------------------------------------------------------


entity signext_tb is 
end signext_tb;
---------------------------------------------------------------------------


Architecture test_bench of signext_tb is

	component signext port (
		    data_in : in std_logic_vector(15 downto 0);
    		    data_out : out std_logic_vector(31 downto 0));
	end component;

	signal data_in : std_logic_vector(15 downto 0);
	signal data_out : std_logic_vector(31 downto 0);
	--signal clk,rst : std_logic;
begin
	dut : signext port map(
	data_in => data_in,
	data_out => data_out);

	process
	begin
    	    data_in <= "0000000000000011";
	    wait for 10 ns;
	    data_in <= "1111111111111111";
    	    wait for 10 ns;
	    data_in <= "1000000000000001";
	    wait for 10 ns;
	    data_in <= "1111111111010111";
	    wait for 10 ns;
        end process;	    

	
end architecture test_bench;

