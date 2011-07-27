library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity if_stage_tb is
end if_stage_tb;
-------------------------------------------------------------------------------
architecture test_bench of if_stage_tb is

component if_stage is
  
  port (
    address_after_jump_or_branch : in std_logic_vector(31 downto 0);
    jump_or_branch               : in std_logic;
    clk                          : in std_logic;
    rst                          : in std_logic;
    pc_write_enable              : in std_logic;
    new_program_counter          : out std_logic_vector(31 downto 0);
    ir_1                      	: out std_logic_vector(31 downto 0);
    ir_0                      	: out std_logic_vector(31 downto 0));

end component;

signal address_after_jump_or_branch, new_program_counter, instr_0, instr_1 : std_logic_vector(31 downto 0);
signal pc_write_enable : std_logic;
signal jump_or_branch, clk : std_logic;
signal rst : std_logic := '1';

begin  -- test_bench

  dut : if_stage
    port map (
      address_after_jump_or_branch   => address_after_jump_or_branch,
      jump_or_branch                 => jump_or_branch,
      clk                            => clk,
      rst                            => rst,
      pc_write_enable                => pc_write_enable,
      new_program_counter            => new_program_counter,
      ir_0                        => instr_0,
      ir_1                        => instr_1);

  pc_write_enable <= '1';
  address_after_jump_or_branch <= (others => '0');
  jump_or_branch <= '0';
  rst <= '0' after 15 ns;

  -- purpose: clock generation process
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

end test_bench;
