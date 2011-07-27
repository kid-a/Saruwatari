library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity control_unit_tb is
end control_unit_tb;
-------------------------------------------------------------------------------
architecture test_bench of control_unit_tb is

  component control_unit
    port (
      clk           : in std_logic;
      rst           : in std_logic;
      opcode_1      : in  std_logic_vector(5 downto 0);
      opcode_0      : in  std_logic_vector(5 downto 0);      
      branch_operand_is_zero : in std_logic;
      if_id_latch_rst : out std_logic;
      pc_write_enable : out std_logic;
      alu_src_1  : out std_logic;      
      alu_src_0  : out std_logic;      
      reg_dst_1     : out std_logic;
      reg_dst_0     : out std_logic;
      jump          : out std_logic;
      jal          : out std_logic;
      branch_taken        : out std_logic;   
      mem_read      : out std_logic;   
      mem_write     : out std_logic;
      wb_enable_1   : out std_logic;
      wb_enable_0   : out std_logic);
  end component;

  signal clk, rst : std_logic;
  signal opcode_1, opcode_0 : std_logic_vector(5 downto 0);
  signal if_id_latch_rst : std_logic;
  signal pc_write_enable : std_logic;
  signal alu_source_1, alu_source_0, branch_operand_is_zero: std_logic;
  signal reg_dst_1, reg_dst_0 : std_logic;
  signal jump, jal, branch_taken, mem_read, mem_write, wb_enable_0,wb_enable_1 : std_logic;

begin  -- test_bench

  dut : control_unit
    port map (
      clk          => clk,
      rst          => rst,
      opcode_1     => opcode_1,
      opcode_0     => opcode_0,
      branch_operand_is_zero => branch_operand_is_zero,
      if_id_latch_rst => if_id_latch_rst,
      pc_write_enable => pc_write_enable,
      alu_src_1 => alu_source_1,
      alu_src_0 => alu_source_0,
      reg_dst_1    => reg_dst_1,
      reg_dst_0    => reg_dst_0,
      jump         => jump,
      jal         => jal,
      branch_taken       => branch_taken,
      mem_read     => mem_read,
      mem_write    => mem_write,
      wb_enable_1 => wb_enable_1,
      wb_enable_0 => wb_enable_0);


  -- purpose: signal generation process
  -- type   : combinational
  -- inputs : 
  -- outputs: 
  process
  begin  -- proces
    branch_operand_is_zero <= '0';
    opcode_1 <= "000101";  --beqz
    opcode_0 <= "000011";  --lw
    wait for 5 ns;

    opcode_1 <= "000110"; --bneq
    opcode_0 <= "000100"; --sw
    wait for 5 ns;

    opcode_1 <= "000001"; --addi
    opcode_0 <= "000001"; --addi
    wait for 5 ns;

    opcode_1 <= "010000"; --R-type
    opcode_0 <= "010000"; --R-type
    wait for 5 ns;

    opcode_1 <= "100000"; --J
    opcode_0 <= "010000"; --R-type
    wait for 5 ns;

    opcode_1 <= "100001"; --Jal
    opcode_0 <= "010000"; --R-type
    wait for 5 ns;


  end process;

  rst <= '0';
  
  process
  begin  -- process
    clk <= '1';
    wait for 5 ns;
    clk <= '0';
    wait for 5 ns;
  end process;

end test_bench;
