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


entity jumpext_tb is 
end jumpext_tb;
---------------------------------------------------------------------------


Architecture test_bench of jumpext_tb is

	component jumpext port (
		    data_in : in std_logic_vector(25 downto 0);
    		    data_out : out std_logic_vector(31 downto 0));
	end component;

	signal data_in : std_logic_vector(25 downto 0);
	signal data_out : std_logic_vector(31 downto 0);
	--signal clk,rst : std_logic;
begin
	dut : jumpext port map(
	data_in => data_in,
	data_out => data_out);
	process
	begin
    	    data_in <= "00000000000000000000000011";
	    wait for 10 ns;
	    data_in <= "00000000000111111111111111";
    	    wait for 10 ns;
	    data_in <= "00000000001111111111111111";
	    wait for 10 ns;
	    data_in <= "11111111111111111111111111";
	    wait for 10 ns;
        end process;	    

	
end architecture test_bench;

