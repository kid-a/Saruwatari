---------------------------------------------------------------------------
-- Authors   : Loris Fichera A63/000232
-- 	       Daniele Marletta A63/000213
--
-- File      : wb_stage_tb.vhd
--
-- Abstract : 
--
--------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
--------------------------------------------------------------------------

entity wb_stage_tb is 
end wb_stage_tb;
--------------------------------------------------------------------------

architecture test_bench of wb_stage_tb is

  component wb_stage is 
    port (
      npc : in std_logic_vector(31 downto 0);
      alu_out_1 : in std_logic_vector(31 downto 0);
      dst_reg_1 : in std_logic_vector(4 downto 0);
      dst_reg_jal : in std_logic_vector(4 downto 0);
      alu_out_0 : in std_logic_vector(31 downto 0);
      lmd : in std_logic_vector(31 downto 0);
      dst_reg_0 : in std_logic_vector(4 downto 0);
      jal : in std_logic;
      mem_read : in std_logic;
      wb_enable_1 : in std_logic;
      wb_enable_0 : in std_logic;
  
      reg_file_write_enable_1 : out std_logic;
      reg_file_write_address_1 : out std_logic_vector(4 downto 0);
      reg_file_write_data_1: out std_logic_vector(31 downto 0);
      reg_file_write_enable_0 : out std_logic;
      reg_file_write_address_0 : out std_logic_vector(4 downto 0);
      reg_file_write_data_0: out std_logic_vector(31 downto 0));
  
  end component;
  

      signal npc :  std_logic_vector(31 downto 0);
      signal alu_out_1 :  std_logic_vector(31 downto 0);
      signal dst_reg_1 :  std_logic_vector(4 downto 0);
      signal dst_reg_jal :  std_logic_vector(4 downto 0);
      signal alu_out_0 :  std_logic_vector(31 downto 0);
      signal lmd :  std_logic_vector(31 downto 0);
      signal dst_reg_0 :  std_logic_vector(4 downto 0);
      signal jal :  std_logic;
      signal mem_read :  std_logic;
      signal wb_enable_1 :  std_logic;
      signal wb_enable_0 :  std_logic;
  
      signal reg_file_write_enable_1 :  std_logic;
      signal reg_file_write_address_1 :  std_logic_vector(4 downto 0);
      signal reg_file_write_data_1:  std_logic_vector(31 downto 0);
      signal reg_file_write_enable_0 :  std_logic;
      signal reg_file_write_address_0 :  std_logic_vector(4 downto 0);
      signal reg_file_write_data_0:  std_logic_vector(31 downto 0);
  
  begin   -- schem
   
  dut : wb_stage 
    port map(
      npc =>npc, 
      alu_out_1 =>alu_out_1, 
      dst_reg_1 =>dst_reg_1, 
      dst_reg_jal =>dst_reg_jal, 
      alu_out_0 =>alu_out_0, 
      lmd =>lmd, 
      dst_reg_0 =>dst_reg_0, 
      jal =>jal, 
      mem_read =>mem_read, 
      wb_enable_1 =>wb_enable_1, 
      wb_enable_0 =>wb_enable_0, 

      reg_file_write_enable_1 =>reg_file_write_enable_1, 
      reg_file_write_address_1 =>reg_file_write_address_1, 
      reg_file_write_data_1=> reg_file_write_data_1,
      reg_file_write_enable_0 =>reg_file_write_enable_0, 
      reg_file_write_address_0 =>reg_file_write_address_0, 
      reg_file_write_data_0=>reg_file_write_data_0); 

  process
  begin
    npc <= "00000000000000000000000000000010";
    alu_out_1 <= "00000000000000000000000000000100";
    dst_reg_1 <= "00001";
    dst_reg_jal <= "11111";
    alu_out_0 <= "00000000000000000000000000001000";
    lmd <= "00000000000000000000000000000111";
    dst_reg_0 <= "00000";
    jal <= '0';
    mem_read <='0';
    wb_enable_1 <='0';
    wb_enable_0 <='0';
  wait for 10 ns;
    jal <= '1';
    mem_read <='1';
    wb_enable_1 <='1';
    wb_enable_0 <='1';
  wait for 10 ns;

end process;
 	  


  -- purpose: clock generation process
  -- type   : combinational
  -- inputs : 
  -- outputs: 
--  process
--  begin  -- process
--    clk <= '1';
--    wait for 5 ns;
--    clk <= '0';
--    wait for 5 ns;
--  end process;
--

end test_bench;

