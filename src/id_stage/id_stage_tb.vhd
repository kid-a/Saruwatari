library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity id_stage_tb is
end id_stage_tb;
-------------------------------------------------------------------------------
architecture test_bench of id_stage_tb is

  component id_stage 
  port (
    npc_in                   : in std_logic_vector(31 downto 0);  -- new program counter
    ir_1_in                  : in std_logic_vector(31 downto 0);
    ir_0_in                  : in std_logic_vector(31 downto 0);
    clk                      : in std_logic;   -- clock
    rst                      : in std_logic;   -- reset
    reg_file_write_enable_1  : in std_logic;   -- write enable for datapath 1
    reg_file_write_address_1 : in std_logic_vector(4 downto 0);   -- destination
                                                                  -- address for
                                                                  -- datapath 1
    reg_file_write_data_1    : in std_logic_vector(31 downto 0);  -- data from
                                                                  -- WB_1
    reg_file_write_enable_0  : in std_logic;   -- write enable for datapath 0
    reg_file_write_address_0 : in std_logic_vector(4 downto 0);   -- destination
                                                                  -- address for
                                                                  -- datapath 0
    reg_file_write_data_0    : in std_logic_vector(31 downto 0);  -- data from
                                                                  -- WB_0
    -- datapath signals
    npc_out                  : out std_logic_vector(31 downto 0);  -- new program counter
    ir_1_out                 : out std_logic_vector(31 downto 0);  -- instruction1
    ir_0_out                 : out std_logic_vector(31 downto 0);  -- instruction0
    address_after_jump_or_branch : out std_logic_vector(31 downto 0);
    jump_or_branch               : out std_logic;
    immediate_1              : out std_logic_vector(31 downto 0); 
    immediate_0              : out std_logic_vector(31 downto 0);  
    A1 		             : out std_logic_vector(31 downto 0);  -- operand A0
    B1		             : out std_logic_vector(31 downto 0);  -- operand B0
    A0 		             : out std_logic_vector(31 downto 0);  -- operand B0
    B0 	                     : out std_logic_vector(31 downto 0);  -- operand B1
    -- Control Unit signals
--    if_id_latch_rst          : out std_logic;
    alu_src_1	             : out std_logic; -- I-type LOW -- R-type HIGH
    alu_src_0	             : out std_logic; -- I-type LOW -- R-type HIGH	
    reg_dst_1 	             : out std_logic;
    reg_dst_0 	             : out std_logic;
--    jump		     : out std_logic; -- datapath 1 
    jal   		     : out std_logic; -- datapath 1
--    branch_taken	     : out std_logic; -- datapath 1
    mem_read	             : out std_logic; -- datapath 0 
    mem_write	             : out std_logic; -- datapath 0
    wb_enable_1              : out std_logic;
    wb_enable_0              : out std_logic);

end component;
--------------------------------------------------------------------------------
-- input signals
-------------------------------------------------------------------------------
signal ir_1_in : std_logic_vector(31 downto 0);
signal ir_0_in : std_logic_vector(31 downto 0);
signal npc_in : std_logic_vector(31 downto 0) :=  "00000000000000000000000000000000";
signal clk : std_logic;
signal rst : std_logic := '1';
signal reg_file_write_enable_1 : std_logic;
signal reg_file_write_address_1 : std_logic_vector(4 downto 0);
signal reg_file_write_data_1 : std_logic_vector(31 downto 0);
signal reg_file_write_enable_0 : std_logic;
signal reg_file_write_address_0 : std_logic_vector(4 downto 0);
signal reg_file_write_data_0 : std_logic_vector(31 downto 0);

-------------------------------------------------------------------------------
-- output signals
-------------------------------------------------------------------------------
signal npc_out : std_logic_vector(31 downto 0) :=  "00000000000000000000000000000000";
signal ir_1_out : std_logic_vector(31 downto 0);
signal ir_0_out : std_logic_vector(31 downto 0);
signal address_after_jump_or_branch : std_logic_vector(31 downto 0);
signal jump_or_branch               :  std_logic;
 signal A0 : std_logic_vector(31 downto 0);
signal A1 : std_logic_vector(31 downto 0);
signal B0 : std_logic_vector(31 downto 0);
signal B1 : std_logic_vector(31 downto 0);
signal immediate_1 : std_logic_vector(31 downto 0);
signal immediate_0 : std_logic_vector(31 downto 0);


--signal jump : std_logic;
signal jal : std_logic;    
--signal branch_taken : std_logic;
signal if_id_latch_rst : std_logic;
signal alu_src_1 : std_logic;
signal alu_src_0 : std_logic;
signal reg_dst_1 : std_logic;
signal reg_dst_0 : std_logic;
signal mem_read : std_logic;
signal mem_write : std_logic;
signal wb_enable_1 : std_logic;
signal wb_enable_0 : std_logic;

begin  -- test_bench

  dut : id_stage
    port map (
      npc_in                   => npc_in,
      ir_1_in                  => ir_1_in,
      ir_0_in                  => ir_0_in,
      clk                      => clk,
      rst                      => rst,
      reg_file_write_enable_1  => reg_file_write_enable_1,
      reg_file_write_address_1 => reg_file_write_address_1,
      reg_file_write_data_1    => reg_file_write_data_1,
      reg_file_write_enable_0  => reg_file_write_enable_0,
      reg_file_write_address_0 => reg_file_write_address_0,
      reg_file_write_data_0    => reg_file_write_data_0,
      npc_out                  => npc_out,
      ir_1_out                 => ir_1_out,
      ir_0_out                 => ir_0_out,
      address_after_jump_or_branch => address_after_jump_or_branch,
      jump_or_branch => jump_or_branch,
      immediate_0              => immediate_0,
      immediate_1              => immediate_1,
      A0                       => A0,
      A1                       => A1,
      B0                       => B0,
      B1                       => B1,
--      jump                     => jump,
      jal                      => jal,    
--      branch_taken             => branch_taken,
--      if_id_latch_rst          => if_id_latch_rst,
      alu_src_1                => alu_src_1,
      alu_src_0                => alu_src_0,
      reg_dst_1                => reg_dst_1,
      reg_dst_0                => reg_dst_0,
      mem_read                 => mem_read,
      mem_write                => mem_write,
      wb_enable_1              => wb_enable_1,
      wb_enable_0              => wb_enable_0);

  rst <= '0' after 13 ns;

--INSTRUCTION SAMPLES:

--addi r1,r2,7
--000001 00010 00001 0000000000000111

--sw r4,20(r0)
--000100 00000 00100 0000000000010100

--beqz r0,20
--000101 00000 00000 0000000000010100

--div r1,r2,r3
--010000 00010 00011 00001 00000000110 
  
  -- purpose: signal generating process
  -- type   : combinational
  -- inputs : 
  -- outputs: 
  process
  begin  -- process
    reg_file_write_address_1 <= "00000";
    reg_file_write_enable_1 <= '0';
    reg_file_write_data_1 <= "00000000000000000000000000000000";
    
    reg_file_write_address_0 <= "00000";
    reg_file_write_enable_0 <= '0';
    reg_file_write_data_0 <= "00000000000000000000000000000000";
    
 
    ir_1_in <= "00000100010000010000000000000111";  -- addi r1,r2,7
    ir_0_in <= "00010000000001000000000000010100";  -- sw r4,20(r0)
    wait for 26 ns;

    ir_1_in <= "00010100000000000000000000010100";  -- beqz r0,20
    ir_0_in <= "01000000010000110000100000000110";  -- div r1,r2,r3
    wait for 26 ns;




--    ir_1 <= "00000100000000010000000000010000";  -- addi R1,R0,16
--    reg_file_write_address_1 <= "00000";
--    reg_file_write_enable_1 <= '0';
--    reg_file_write_data_1 <= "00000000000000000000000000000000";
--    wait for 17 ns;

    -- REGISTER FILE WRITE TEST
--    ir_1 <= "00010100000000010000000000000001";  -- beqz R0, +1
--    reg_file_write_address_1 <= "00000";
--    reg_file_write_enable_1 <= '1';
--    reg_file_write_data_1 <= "00000000000000000000000000000001";
--    wait for 27 ns;
  end process;
  
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

