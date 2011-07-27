library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity exmem_stage_tb is
end exmem_stage_tb;
-------------------------------------------------------------------------------
architecture test_bench of exmem_stage_tb is

  component exmem_stage is
    port(
      clk                          : in std_logic;
      rst                          : in std_logic;
      npc_in                       : in std_logic_vector(31 downto 0);
      ir_1                         : in std_logic_vector(31 downto 0);
      ir_0                         : in std_logic_vector(31 downto 0);
      immediate_1                  : in std_logic_vector(31 downto 0);
      immediate_0                  : in std_logic_vector(31 downto 0);
      A1                           : in std_logic_vector(31 downto 0);
      B1                           : in std_logic_vector(31 downto 0);
      A0                           : in std_logic_vector(31 downto 0);
      B0                           : in std_logic_vector(31 downto 0);
      reg_dst_1                    : in std_logic;
      reg_dst_0                    : in std_logic;
--      branch_taken                 : in std_logic;
--      jump                         : in std_logic;
      jal_in                       : in std_logic;
      alu_src_1                    : in std_logic;
      alu_src_0                    : in std_logic;
      mem_read_in                     : in std_logic;
      mem_write                    : in std_logic;
      wb_enable_1_in                  : in std_logic;
      wb_enable_0_in                  : in std_logic;
  
--      address_after_jump_or_branch : out std_logic_vector(31 downto 0);
--      jump_or_branch               : out std_logic;
      alu_1_data_ready_out         : out std_logic;
      alu_0_data_ready_out         : out std_logic;
      npc_out                      : out std_logic_vector(31 downto 0);
      alu_out_1                    : out std_logic_vector(31 downto 0);
      dst_reg_1                    : out std_logic_vector(4 downto 0);
      alu_out_0                    : out std_logic_vector(31 downto 0);
      lmd                          : out std_logic_vector(31 downto 0);
      dst_reg_0                    : out std_logic_vector(4 downto 0);
      jal_out			 : out std_logic;
      mem_read_out			 : out std_logic;
      wb_enable_1_out                  : out std_logic;
      wb_enable_0_out                  : out std_logic);
   
      
  end component;

  signal clk : std_logic;
  signal rst : std_logic := '1';
  signal npc_in : std_logic_vector(31 downto 0);
  signal npc_out : std_logic_vector(31 downto 0);
  signal ir_1 : std_logic_vector(31 downto 0);
  signal ir_0 : std_logic_vector(31 downto 0);
  signal immediate_1 : std_logic_vector(31 downto 0);
  signal immediate_0 : std_logic_vector(31 downto 0);
  signal A1 : std_logic_vector(31 downto 0);
  signal B1 : std_logic_vector(31 downto 0);
  signal A0 : std_logic_vector(31 downto 0);
  signal B0 : std_logic_vector(31 downto 0);
  signal reg_dst_1 : std_logic;
  signal reg_dst_0 : std_logic;
--  signal branch_taken : std_logic;
--  signal jump : std_logic;
  signal jal_in : std_logic;
  signal jal_out : std_logic;
  signal alu_src_1 : std_logic;
  signal alu_src_0 : std_logic;
  signal mem_read_in : std_logic;
  signal mem_read_out : std_logic;
  signal mem_write : std_logic;
  signal wb_enable_1_in : std_logic;
  signal wb_enable_1_out : std_logic;
  signal wb_enable_0_in : std_logic;
  signal wb_enable_0_out : std_logic;
--  signal address_after_jump_or_branch : std_logic_vector(31 downto 0);
--  signal jump_or_branch : std_logic;
  signal lmd : std_logic_vector(31 downto 0);
  signal dst_reg_1 : std_logic_vector(4 downto 0);
  signal dst_reg_0 : std_logic_vector(4 downto 0);
  signal alu_out_1 : std_logic_vector(31 downto 0);
  signal alu_out_0 : std_logic_vector(31 downto 0);
    
begin  -- test_bench

  dut : exmem_stage
    port map (
      clk                          => clk,
      rst                          => rst,
      npc_in                          => npc_in,
      ir_1                         => ir_1,
      ir_0                         => ir_0,
      immediate_1                  => immediate_1,
      immediate_0                  => immediate_0,
      A1                           => A1,
      B1                           => B1,
      A0                           => A0,
      B0                           => B0,
      reg_dst_1                    => reg_dst_1,
      reg_dst_0                    => reg_dst_0,
  --    branch_taken                 => branch_taken,
  --    jump                         => jump,
      jal_in                          => jal_in,
      alu_src_1                    => alu_src_1,
      alu_src_0                    => alu_src_0,
      mem_read_in                     => mem_read_in,
      mem_write                    => mem_write,
      wb_enable_1_in                  => wb_enable_1_in,
      wb_enable_0_in                  => wb_enable_0_in,
    --  address_after_jump_or_branch => address_after_jump_or_branch,
    --  jump_or_branch               => jump_or_branch,
      lmd                          => lmd,
      dst_reg_1                    => dst_reg_1,
      dst_reg_0                    => dst_reg_0,
      alu_out_1                    => alu_out_1,
      jal_out                          => jal_out,
      mem_read_out                     => mem_read_out,
      wb_enable_1_out                  => wb_enable_1_out,
      wb_enable_0_out                 => wb_enable_0_out,
      npc_out			   => npc_out,
      alu_out_0                    => alu_out_0);

  process
  begin  -- process
    wait for 10 ns;
    npc_in <= "00000000000000000000000000010010";  -- 0x18
    ir_1 <= "00000100010000010000000000000111";  -- addi r1,r2,7
    ir_0 <= "00010000000001000000000000010100";  -- sw r4,20(r0)
    immediate_1 <= "00000000000000000000000000000111";
    immediate_0 <= "00000000000000000000000000010100";
    A1 <= "00000000000000000000000000000010";
    B1 <= "00000000000000000000000000000001";
    A0 <= "00000000000000000000000000000001";
    B0 <= "00000000000000000000000000000100";
    reg_dst_1 <= '0';
    reg_dst_0 <= '0';          
--    branch_taken <= '0';
--    jump <= '0';
    jal_in <= '0';
    alu_src_1 <= '0';
    alu_src_0 <= '0';          
    mem_read_in <= '0';          
    mem_write <= '1' after 3 ns;        -- this is needed in order to prevent
                                        -- the instruction memory from storing
                                        -- "strange" values that are generated
                                        -- by the alu_0 while rst is on
    wb_enable_1_in <= '1';
    wb_enable_0_in <= '0';          
--    jump_or_branch <= '0';
    wait for 26 ns;
    
    npc_in <= "00000000000000000000000000010010";  -- 0x20
    ir_1 <= "00010100000000000000000000010100";  -- beqz r0,20
    ir_0 <= "01000000010000110000100000000110";  -- div r1,r2,r3
    immediate_1 <= "00000000000000000000000000010100";
    immediate_0 <= "00000000000000000000100000000110"; 
    A1 <= "00000000000000000000000000000010";
    B1 <= "00000000000000000000000000000001";
    A0 <= "00000000000000000000000000000001";
    B0 <= "00000000000000000000000000000100";
    reg_dst_1 <= '0';
    reg_dst_0 <= '1';          
--    branch_taken <= '1';
--    jump <= '0';
    jal_in <= '0';
    alu_src_1 <= '0';
    alu_src_0 <= '1';          
    mem_read_in <= '0';          
    mem_write <= '0';             
    wb_enable_1_in <= '0';
    wb_enable_0_in <= '1';
--    jump_or_branch <= '1';
    wait for 26 ns;
  end process;
  
  
  rst <= '0' after 13 ns;

  process
  begin  -- process
    clk <= '1';
    wait for 5 ns;
    clk <= '0';
    wait for 5 ns;
  end process;
  
end test_bench;
