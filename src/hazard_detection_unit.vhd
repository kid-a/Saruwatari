-------------------------------------------------------------------------------
-- Hazard Detection Unit
-- 
-- Daniele Marletta 
-- Loris Fichera
--
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
-------------------------------------------------------------------------------
entity hazard_detection_unit is
  
  port (
    clk                    : in  std_logic;
    rst                    : in  std_logic;
    if_id_ir_1             : in  std_logic_vector(31 downto 0);
    if_id_ir_0             : in  std_logic_vector(31 downto 0);
    id_exmem_ir_1          : in  std_logic_vector(31 downto 0);
    id_exmem_ir_0          : in  std_logic_vector(31 downto 0);
    jump_or_branch_taken   : in  std_logic;

    alu_1_data_ready       : in std_logic;
    alu_0_data_ready       : in std_logic;
    
    pc_write_enable	   : out std_logic;
    id_exmem_rst	   : out std_logic;
    if_id_rst		   : out std_logic; 
    if_id_we               : out std_logic;
    id_exmem_we            : out std_logic;
    exmem_wb_we            : out std_logic);

end hazard_detection_unit;
-------------------------------------------------------------------------------
architecture rtl of hazard_detection_unit is

  type hazard_detection_state is (idle,
                                  structural_hazard,
                                  control_hazard,
                                  data_control_hazard,
                                  data_hazard_0,
                                  data_hazard_1);

  signal pr_state : hazard_detection_state;
  signal nx_state : hazard_detection_state;

  signal pr_state_1 : hazard_detection_state;
  signal pr_state_0 : hazard_detection_state;

begin  -- rtl
  
  process (clk, rst) 
  begin
    if rst = '1' then
      pr_state <= idle;
      pr_state_1 <= idle; 
      pr_state_0 <= idle;
      assert false report "reset" severity note;
      
    elsif clk'event and clk = '1' then 
      pr_state <= nx_state;
      
    end if;
  end process;


  
  process (jump_or_branch_taken, alu_0_data_ready,
           alu_1_data_ready, if_id_ir_1, if_id_ir_0,
           id_exmem_ir_1, id_exmem_ir_0, pr_state)
  begin
    
    case pr_state is

      -------------------------------------------------------------------------
      -- STATE : IDLE
      -------------------------------------------------------------------------
      when idle =>

        assert false
          report "idle"
          severity note;

        -- check if a nop is in the exmem stage
        if id_exmem_ir_1(31 downto 26) = "000000" and
          id_exmem_ir_0(31 downto 26) = "000000" then

          pc_write_enable <= '1';
          if_id_we <= '1';
          if_id_rst <= '0';
          id_exmem_we <= '1';
          id_exmem_rst <= '0';
          exmem_wb_we <= '1';
          nx_state <= idle;
          
          assert false
            report "z"
            severity note;

          -- check for structural hazards or structural - data hazards
        elsif alu_0_data_ready = '0' or
          alu_1_data_ready = '0' then

          -- se c`e` un hazard strutturale seguito da uno dati, la sequenza
          -- degli stati deve essere: idle -> structural -> data_hazard_1 -> idle

          id_exmem_we <= '0';
          if_id_we <= '0';
          if_id_rst <= '0';
          exmem_wb_we <= '0';
          pc_write_enable <= '0';
          nx_state <= structural_hazard;
          assert false
            report "yyy"
            severity note;
          
          -- check for data|control hazards
        else
          id_exmem_we <= '1';
          if_id_we <= '1';
          exmem_wb_we <= '1';
          pc_write_enable <= '1';

          if                                             -- IR0 in EXMEM is a
            ( id_exmem_ir_0(31 downto 26) = "000001" or    -- ADDI
              id_exmem_ir_0(31 downto 26) = "000010" or    -- SUBI
              id_exmem_ir_0(31 downto 26) = "000111" or    -- MULTI
              id_exmem_ir_0(31 downto 26) = "000011" ) and  -- LW
            ( if_id_ir_0(31 downto 30) = "00" and	-- IR0 in ID is a I-type
              id_exmem_ir_0(20 downto 16) = if_id_ir_0(25 downto 21) and
              id_exmem_ir_0(20 downto 16) /= "00000" ) then
            nx_state <= data_hazard_1;
            id_exmem_we <= '0';
            pc_write_enable <= '0';
            if_id_we <= '0';
            assert false report "a" severity note;
              
          elsif
            ( id_exmem_ir_0(31 downto 26) = "000001" or    -- ADDI
              id_exmem_ir_0(31 downto 26) = "000010" or    -- SUBI
              id_exmem_ir_0(31 downto 26) = "000111" or    -- MULTI
              id_exmem_ir_0(31 downto 26) = "000011" ) and  -- LW
            ( if_id_ir_1(31 downto 30) = "00" and 	-- IR1 in ID is a I-type
              id_exmem_ir_0(20 downto 16) = if_id_ir_1(25 downto 21) and
              id_exmem_ir_0(20 downto 16) /= "00000" ) then

            id_exmem_we <= '0';
            pc_write_enable <= '0';
            if_id_we <= '0';

            if if_id_ir_1(31 downto 26) = "000101" or
              if_id_ir_1(31 downto 26) = "000110" then
--                if_id_ir_1(31 downto 30) = "10" then 
              nx_state <= data_control_hazard;

            else
              nx_state <= data_hazard_1;
                
            end if;
            assert false report "b" severity note;
              
            elsif
              ( id_exmem_ir_0(31 downto 26) = "000001" or    -- ADDI
                id_exmem_ir_0(31 downto 26) = "000010" or    -- SUBI
                id_exmem_ir_0(31 downto 26) = "000111" or    -- MULTI
                id_exmem_ir_0(31 downto 26) = "000011" ) and -- LW
              ( if_id_ir_0(31 downto 30) = "01" and	-- IR0 in ID is a R-type
                ((id_exmem_ir_0(20 downto 16) = if_id_ir_0(25 downto 21) and
                  id_exmem_ir_0(20 downto 16) /= "00000") or
                 (id_exmem_ir_0(20 downto 16) = if_id_ir_0(20 downto 16) and
                  id_exmem_ir_0(20 downto 16) /= "00000"))) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "c" severity note;
              
            elsif
              ( id_exmem_ir_0(31 downto 26) = "000001" or    -- ADDI
                id_exmem_ir_0(31 downto 26) = "000010" or    -- SUBI
                id_exmem_ir_0(31 downto 26) = "000111" or    -- MULTI
                id_exmem_ir_0(31 downto 26) = "000011" ) and -- LW
              ( if_id_ir_1(31 downto 30) = "01" and	-- IR1 in ID is a R-type
                ((id_exmem_ir_0(20 downto 16) = if_id_ir_1(25 downto 21) and
                  id_exmem_ir_0(20 downto 16) /= "00000") or
                 (id_exmem_ir_0(20 downto 16) = if_id_ir_1(20 downto 16) and
                  id_exmem_ir_0(20 downto 16) /= "00000"))) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "d" severity note;

              -- check for control hazard
            elsif
              ( id_exmem_ir_0(31 downto 26) = "000001" or    -- ADDI
                id_exmem_ir_0(31 downto 26) = "000010" or    -- SUBI
                id_exmem_ir_0(31 downto 26) = "000111" or    -- MULTI
                id_exmem_ir_0(31 downto 26) = "000011" ) and -- LW
             ( if_id_ir_1(31 downto 26) = "000101" or
               if_id_ir_1(31 downto 26) = "000110" or
               if_id_ir_1(31 downto 30) = "10" ) then
              nx_state <= control_hazard;
                
--              else
--                -- next state => idle;
--                if_id_we <= '1';
--                id_exmem_we <= '1';
--                id_exmem_rst <= '0';
--                pc_write_enable <= '1';
--                exmem_wb_we <= '1';
--                nx_state <= idle;
--                assert false report "e" severity note;
--              end if;
--              assert false report "ee" severity note;
--            end if;

          elsif
            id_exmem_ir_0(31 downto 26) = "010000" and    -- IR0 in EXMEM is A
                                                          -- R-type
            ( if_id_ir_0(31 downto 30) = "00" and	  -- IR0 in ID is a I-type  	
              id_exmem_ir_0(15 downto 11) = if_id_ir_0(25 downto 21) and
              id_exmem_ir_0(15 downto 11) /= "00000" ) then
            
            nx_state <= data_hazard_1;
            id_exmem_we <= '0';
            pc_write_enable <= '0';
            if_id_we <= '0';
            assert false report "f" severity note;
              
            elsif
              id_exmem_ir_0(31 downto 26) = "010000" and
              ( if_id_ir_1(31 downto 30) = "00" and	-- IR1 in ID is a I-type
                id_exmem_ir_0(15 downto 11) = if_id_ir_1(25 downto 21) and
                id_exmem_ir_0(15 downto 11) /= "00000" ) then

              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';

              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" then
                nx_state <= data_control_hazard;
                
              else
                nx_state <= data_hazard_1;
                
              end if;
              assert false report "g" severity note;  
              
            elsif
              id_exmem_ir_0(31 downto 26) = "010000" and
              ( if_id_ir_0(31 downto 30) = "01" and	-- IR0 in ID is a R-type
                ((id_exmem_ir_0(15 downto 11) = if_id_ir_0(25 downto 21) and
                  id_exmem_ir_0(15 downto 11) /= "00000") or
                 (id_exmem_ir_0(15 downto 11) = if_id_ir_0(20 downto 16) and
                  id_exmem_ir_0(15 downto 11) /= "00000"))) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "h" severity note;
              
            elsif
              id_exmem_ir_0(31 downto 26) = "010000" and
              ( if_id_ir_1(31 downto 30) = "01" and	-- IR1 in ID is a R-type
                ((id_exmem_ir_0(15 downto 11) = if_id_ir_1(25 downto 21) and
                  id_exmem_ir_0(15 downto 11) /= "00000") or
                 (id_exmem_ir_0(15 downto 11) = if_id_ir_1(20 downto 16) and
                  id_exmem_ir_0(15 downto 11) /= "00000"))) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "i" severity note;

            elsif
              id_exmem_ir_0(31 downto 26) = "010000" and 
              ( if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" or
                if_id_ir_1(31 downto 30) = "10" ) then
                
              nx_state <= control_hazard;

--              else
                
--                -- next state => idle
--                if_id_we <= '1';
--                id_exmem_we <= '1';
--                id_exmem_rst <= '0';
--                pc_write_enable <= '1';
--                exmem_wb_we <= '1';
--                nx_state <= idle;
                
--                assert false report "jj" severity note;
--              end if;
--              assert false report "j" severity note;
--            end if;
--          end if;

            -------------------------------------------------------------------
            -- 
            -------------------------------------------------------------------

            elsif
              ( id_exmem_ir_1(31 downto 26) = "000001" or
                id_exmem_ir_1(31 downto 26) = "000010" or
                id_exmem_ir_1(31 downto 26) = "000111" ) and
              ( if_id_ir_0(31 downto 30) = "00" and	-- IR0 in ID is a I-type
                id_exmem_ir_1(20 downto 16) = if_id_ir_0(25 downto 21) and
                id_exmem_ir_1(20 downto 16) /= "00000" ) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "k" severity note;  
              
            elsif
              ( id_exmem_ir_1(31 downto 26) = "000001" or
                id_exmem_ir_1(31 downto 26) = "000010" or
                id_exmem_ir_1(31 downto 26) = "000111" ) and
              ( if_id_ir_1(31 downto 30) = "00" and	-- IR1 in ID is a I-type
                id_exmem_ir_1(20 downto 16) = if_id_ir_1(25 downto 21) and
                id_exmem_ir_1(20 downto 16) /= "00000" ) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "l" severity note;
              
            elsif
              ( id_exmem_ir_1(31 downto 26) = "000001" or
                id_exmem_ir_1(31 downto 26) = "000010" or
                id_exmem_ir_1(31 downto 26) = "000111" ) and
              ( if_id_ir_0(31 downto 30) = "01" and	-- IR0 in ID is a R-type
                ((id_exmem_ir_1(20 downto 16) = if_id_ir_0(25 downto 21) and
                  id_exmem_ir_1(20 downto 16) /= "00000") or 
                 (id_exmem_ir_1(20 downto 16) = if_id_ir_0(20 downto 16) and
                  id_exmem_ir_1(20 downto 16) /= "00000"))) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "m" severity note;
              
            elsif
              ( id_exmem_ir_1(31 downto 26) = "000001" or
                id_exmem_ir_1(31 downto 26) = "000010" or
                id_exmem_ir_1(31 downto 26) = "000111" ) and
              ( if_id_ir_1(31 downto 30) = "01" and	-- IR1 in ID is a R-type
                ((id_exmem_ir_1(20 downto 16) = if_id_ir_1(25 downto 21) and
                  id_exmem_ir_1(20 downto 16) /= "00000") or
                 (id_exmem_ir_1(20 downto 16) = if_id_ir_1(20 downto 16) and
                  id_exmem_ir_1(20 downto 16) /= "00000"))) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "n" severity note;

            elsif
              ( id_exmem_ir_1(31 downto 26) = "000001" or
                id_exmem_ir_1(31 downto 26) = "000010" or
                id_exmem_ir_1(31 downto 26) = "000111" ) and
              ( if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" or
                if_id_ir_1(31 downto 30) = "10") then
                
                nx_state <= control_hazard;

--              else
                
--                -- next state => idle
--                if_id_we <= '1';
--                id_exmem_we <= '1';
--                id_exmem_rst <= '0';
--                pc_write_enable <= '1';
--                exmem_wb_we <= '1';
--                nx_state <= idle;
--              end if;

--              assert false report "o" severity note;
--            end if;

            elsif id_exmem_ir_1(31 downto 26) = "010000" and -- IR1 in EXMEM
                                                             -- is a R-type  
              ( if_id_ir_0(31 downto 30) = "00" and	-- IR0 in ID is a I-type  	
                id_exmem_ir_1(15 downto 11) = if_id_ir_0(25 downto 21) and
                id_exmem_ir_1(15 downto 11) /= "00000" ) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "p" severity note;

            elsif id_exmem_ir_1(31 downto 26) = "010000" and 
              ( if_id_ir_1(31 downto 30) = "00" and	-- IR1 in ID is a I-type
                id_exmem_ir_1(15 downto 11) = if_id_ir_1(25 downto 21) and
                id_exmem_ir_1(15 downto 11) /= "00000" ) then

              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              
              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" then
                nx_state <= data_control_hazard;
                
              else
                nx_state <= data_hazard_1;
              end if;

              assert false report "q" severity note;
              
            elsif id_exmem_ir_1(31 downto 26) = "010000" and
              ( if_id_ir_0(31 downto 30) = "01" and	-- IR0 in ID is a R-type
                ((id_exmem_ir_1(15 downto 11) = if_id_ir_0(25 downto 21) and
                  id_exmem_ir_1(15 downto 11) /= "00000") or
                 (id_exmem_ir_1(15 downto 11) = if_id_ir_0(20 downto 16) and
                  id_exmem_ir_1(15 downto 11) /= "00000"))) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "r" severity note;
              
            elsif id_exmem_ir_1(31 downto 26) = "010000" and
              ( if_id_ir_1(31 downto 30) = "01" and	-- IR1 in ID is a R-type
                ((id_exmem_ir_1(15 downto 11) = if_id_ir_1(25 downto 21) and
                  id_exmem_ir_1(15 downto 11) /= "00000") or
                 (id_exmem_ir_1(15 downto 11) = if_id_ir_1(20 downto 16) and
                  id_exmem_ir_1(15 downto 11) /= "00000"))) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "s" severity note;
              
            elsif
              id_exmem_ir_1(31 downto 26) = "010000" and
              ( if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" or
                if_id_ir_1(31 downto 30) = "10") then
                
              nx_state <= control_hazard;

            else
                
                -- next state => idle
                if_id_we <= '1';
                id_exmem_we <= '1';
                id_exmem_rst <= '0';
                pc_write_enable <= '1';
                exmem_wb_we <= '1';
                nx_state <= idle;
                assert false report "t" severity note;
            end if;
        end if;

        -----------------------------------------------------------------------
        
        -----------------------------------------------------------------------
        -- STATE : STRUCTURAL HAZARD
        -----------------------------------------------------------------------
        -- se c`e` un hazard strutturale seguito da uno dati, la sequenza
        -- degli stati deve essere: idle -> structural -> data_hazard_1 -> idle
        -----------------------------------------------------------------------
      when structural_hazard =>
        assert false
          report "structural_hazard"
          severity note;

        -- check if a nop is in the exmem stage
        if id_exmem_ir_1(31 downto 26) = "000000" and
          id_exmem_ir_0(31 downto 26) = "000000" then

          pc_write_enable <= '1';
          if_id_we <= '1';
          if_id_rst <= '0';
          id_exmem_we <= '1';
          id_exmem_rst <= '0';
          exmem_wb_we <= '1';
          nx_state <= idle;
          
          assert false
            report "z"
            severity note;

--        elsif pr_state = structural_hazard and  -- if there's a structural
--                                                -- hazard followed by a branch
--                                                -- or a jump
--          (alu_1_data_ready = '0' or
--           alu_0_data_ready = '0') and
--          (if_id_ir_1(31 downto 26) = "000101" or
--           if_id_ir_1(31 downto 26) = "000110" or
--           if_id_ir_1(31 downto 30) = "10") then
--          pc_write_enable <= '1';
--          if_id_we <= '1';
--          if_id_rst <= '0';
--          id_exmem_we <= '1';
--          id_exmem_rst <= '0';
--          exmem_wb_we <= '1';
--          nx_state <= idle;

          -- check for structural hazards or structural - data hazards
        elsif alu_0_data_ready = '0' or
          alu_1_data_ready = '0' then
          
          id_exmem_we <= '0';
          if_id_we <= '0';
          if_id_rst <= '0';
          exmem_wb_we <= '0';
          pc_write_enable <= '0';
          nx_state <= structural_hazard;
          assert false report "y" severity note;
          
          -- check for data|control hazards
        else
          id_exmem_we <= '1';
          if_id_we <= '1';
          exmem_wb_we <= '1';
          pc_write_enable <= '1';

          if                                             -- IR0 in EXMEM is a
            id_exmem_ir_0(31 downto 26) = "000001" or    -- ADDI
            id_exmem_ir_0(31 downto 26) = "000010" or    -- SUBI
            id_exmem_ir_0(31 downto 26) = "000111" or    -- MULTI
            id_exmem_ir_0(31 downto 26) = "000011" then  -- LW
            
            if if_id_ir_0(31 downto 30) = "00" and	-- IR0 in ID is a I-type
              id_exmem_ir_0(20 downto 16) = if_id_ir_0(25 downto 21) and
              id_exmem_ir_0(20 downto 16) /= "00000" then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "a" severity note;
              
            elsif if_id_ir_1(31 downto 30) = "00" and 	-- IR1 in ID is a I-type
              id_exmem_ir_0(20 downto 16) = if_id_ir_1(25 downto 21) and
              id_exmem_ir_0(20 downto 16) /= "00000" then

              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';

              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" then
--                if_id_ir_1(31 downto 30) = "10" then
                nx_state <= data_control_hazard;

              else
                nx_state <= data_hazard_1;
                
              end if;
              assert false report "b" severity note;
              
            elsif if_id_ir_0(31 downto 30) = "01" and	-- IR0 in ID is a R-type
              ((id_exmem_ir_0(20 downto 16) = if_id_ir_0(25 downto 21) and
                id_exmem_ir_0(20 downto 16) /= "00000") or
               (id_exmem_ir_0(20 downto 16) = if_id_ir_0(20 downto 16) and
                id_exmem_ir_0(20 downto 16) /= "00000")) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "c" severity note;
              
            elsif if_id_ir_1(31 downto 30) = "01" and	-- IR1 in ID is a R-type
              ((id_exmem_ir_0(20 downto 16) = if_id_ir_1(25 downto 21) and
                id_exmem_ir_0(20 downto 16) /= "00000") or
               (id_exmem_ir_0(20 downto 16) = if_id_ir_1(20 downto 16) and
                id_exmem_ir_0(20 downto 16) /= "00000")) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "d" severity note;

              -- check for control hazard
            else
              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" or
                if_id_ir_1(31 downto 30) = "10" then
                nx_state <= control_hazard;
                
              else
                -- next state => idle;
                if_id_we <= '1';
                id_exmem_we <= '1';
                id_exmem_rst <= '0';
                pc_write_enable <= '1';
                exmem_wb_we <= '1';
                nx_state <= idle;
                assert false report "e" severity note;
              end if;
              assert false report "ee" severity note;
            end if;

          elsif
            id_exmem_ir_0(31 downto 26) = "010000" then    -- IR0 in EXMEM is A
                                                           -- R-type

            if if_id_ir_0(31 downto 30) = "00" and	-- IR0 in ID is a I-type  	
              id_exmem_ir_0(15 downto 11) = if_id_ir_0(25 downto 21) and
              id_exmem_ir_0(15 downto 11) /= "00000" then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "f" severity note;
              
            elsif if_id_ir_1(31 downto 30) = "00" and	-- IR1 in ID is a I-type
              id_exmem_ir_0(15 downto 11) = if_id_ir_1(25 downto 21) and
              id_exmem_ir_0(15 downto 11) /= "00000" then

              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';

              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" then
                nx_state <= data_control_hazard;
                
              else
                nx_state <= data_hazard_1;
                
              end if;
              assert false report "g" severity note;  
              
            elsif if_id_ir_0(31 downto 30) = "01" and	-- IR0 in ID is a R-type
              ((id_exmem_ir_0(15 downto 11) = if_id_ir_0(25 downto 21) and
                id_exmem_ir_0(15 downto 11) /= "00000") or
               (id_exmem_ir_0(15 downto 11) = if_id_ir_0(20 downto 16) and
                id_exmem_ir_0(15 downto 11) /= "00000")) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "h" severity note;
              
            elsif if_id_ir_1(31 downto 30) = "01" and	-- IR1 in ID is a R-type
              ((id_exmem_ir_0(15 downto 11) = if_id_ir_1(25 downto 21) and
                id_exmem_ir_0(15 downto 11) /= "00000") or
               (id_exmem_ir_0(15 downto 11) = if_id_ir_1(20 downto 16) and
                id_exmem_ir_0(15 downto 11) /= "00000")) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "i" severity note;

            else
              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" or
                if_id_ir_1(31 downto 30) = "10" then
                
                nx_state <= control_hazard;

              else
                
                -- next state => idle
                if_id_we <= '1';
                id_exmem_we <= '1';
                id_exmem_rst <= '0';
                pc_write_enable <= '1';
                exmem_wb_we <= '1';
                nx_state <= idle;
                
                assert false report "jj" severity note;
              end if;
              assert false report "j" severity note;
            end if;

          elsif
            id_exmem_ir_1(31 downto 26) = "000001" or
            id_exmem_ir_1(31 downto 26) = "000010" or
            id_exmem_ir_1(31 downto 26) = "000111" then
            
            if if_id_ir_0(31 downto 30) = "00" and	-- IR0 in ID is a I-type
              id_exmem_ir_1(20 downto 16) = if_id_ir_0(25 downto 21) and
              id_exmem_ir_1(20 downto 16) /= "00000" then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "k" severity note;  
              
            elsif if_id_ir_1(31 downto 30) = "00" and	-- IR1 in ID is a I-type
              id_exmem_ir_1(20 downto 16) = if_id_ir_1(25 downto 21) and
              id_exmem_ir_1(20 downto 16) /= "00000" then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "l" severity note;
              
            elsif if_id_ir_0(31 downto 30) = "01" and	-- IR0 in ID is a R-type
              ((id_exmem_ir_1(20 downto 16) = if_id_ir_0(25 downto 21) and
                id_exmem_ir_1(20 downto 16) /= "00000") or 
               (id_exmem_ir_1(20 downto 16) = if_id_ir_0(20 downto 16) and
                id_exmem_ir_1(20 downto 16) /= "00000")) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "m" severity note;
              
            elsif if_id_ir_1(31 downto 30) = "01" and	-- IR1 in ID is a R-type
              ((id_exmem_ir_1(20 downto 16) = if_id_ir_1(25 downto 21) and
                id_exmem_ir_1(20 downto 16) /= "00000") or
               (id_exmem_ir_1(20 downto 16) = if_id_ir_1(20 downto 16) and
                id_exmem_ir_1(20 downto 16) /= "00000")) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "n" severity note;

            else
              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" or
                if_id_ir_1(31 downto 30) = "10" then
                
                nx_state <= control_hazard;

              else
                
                -- next state => idle
                if_id_we <= '1';
                id_exmem_we <= '1';
                id_exmem_rst <= '0';
                pc_write_enable <= '1';
                exmem_wb_we <= '1';
                nx_state <= idle;
              end if;

              assert false report "o" severity note;
            end if;

          elsif id_exmem_ir_1(31 downto 26) = "010000" then  -- IR1 in EXMEM
                                                             -- is a R-type  
            if if_id_ir_0(31 downto 30) = "00" and	-- IR0 in ID is a I-type  	
              id_exmem_ir_1(15 downto 11) = if_id_ir_0(25 downto 21) and
              id_exmem_ir_1(15 downto 11) /= "00000" then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "p" severity note;
              
            elsif if_id_ir_1(31 downto 30) = "00" and	-- IR1 in ID is a I-type
              id_exmem_ir_1(15 downto 11) = if_id_ir_1(25 downto 21) and
              id_exmem_ir_1(15 downto 11) /= "00000" then

              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              
              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" then
                nx_state <= data_control_hazard;
                
              else
                nx_state <= data_hazard_1;
              end if;

              assert false report "q" severity note;
              
            elsif if_id_ir_0(31 downto 30) = "01" and	-- IR0 in ID is a R-type
              ((id_exmem_ir_1(15 downto 11) = if_id_ir_0(25 downto 21) and
                id_exmem_ir_1(15 downto 11) /= "00000") or
               (id_exmem_ir_1(15 downto 11) = if_id_ir_0(20 downto 16) and
                id_exmem_ir_1(15 downto 11) /= "00000")) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "r" severity note;
              
            elsif if_id_ir_1(31 downto 30) = "01" and	-- IR1 in ID is a R-type
              ((id_exmem_ir_1(15 downto 11) = if_id_ir_1(25 downto 21) and
                id_exmem_ir_1(15 downto 11) /= "00000") or
               (id_exmem_ir_1(15 downto 11) = if_id_ir_1(20 downto 16) and
                id_exmem_ir_1(15 downto 11) /= "00000")) then
              nx_state <= data_hazard_1;
              id_exmem_we <= '0';
              pc_write_enable <= '0';
              if_id_we <= '0';
              assert false report "s" severity note;

            else
              if if_id_ir_1(31 downto 26) = "000101" or
                if_id_ir_1(31 downto 26) = "000110" or
                if_id_ir_1(31 downto 30) = "10" then
                
                nx_state <= control_hazard;

              else
                
                -- next state => idle
                if_id_we <= '1';
                id_exmem_we <= '1';
                id_exmem_rst <= '0';
                pc_write_enable <= '1';
                exmem_wb_we <= '1';
                nx_state <= idle;
              end if;
              assert false report "t" severity note;
            end if;
          end if;          
        end if;
        -----------------------------------------------------------------------


        -----------------------------------------------------------------------
        -- STATE : CONTROL HAZARD
        -----------------------------------------------------------------------
      when control_hazard =>

        assert false report "control hazard" severity note;

        if if_id_ir_1(31 downto 26) = "000000" then
          if_id_rst <= '0';

        elsif jump_or_branch_taken = '1' or
          if_id_ir_1(31 downto 30) = "10" then
          if_id_rst <= '1';

        else
          if_id_rst <= '0';
          assert false report "xx" severity note;
          
        end if;

        nx_state <= idle;

        -----------------------------------------------------------------------
        -- STATE : DATA HAZARD + SUBSEQUENT CONTROL HAZARD
        -----------------------------------------------------------------------
      when data_control_hazard =>
        assert false report "data and control hazard" severity note;
        
        nx_state <= data_hazard_1;
        pc_write_enable <= '1';
        if_id_we <= '1';
        assert false report "t" severity note;

        -----------------------------------------------------------------------
        -- STATE : DATA HAZARD CYCLE 0
        -----------------------------------------------------------------------
      when data_hazard_0 =>
        assert false report "data hazard 0" severity note;
        nx_state <= data_hazard_1;
        pc_write_enable <= '0';
--        id_exmem_rst <= '1';
        assert false report "x" severity note;

        -----------------------------------------------------------------------
        -- STATE : DATA HAZARD CYCLE 1
        -----------------------------------------------------------------------
      when data_hazard_1 =>
        assert false report "data hazard 1" severity note;
        if (if_id_ir_1(31 downto 26) = "000101" or
            if_id_ir_1(31 downto 26) = "000110") and
          jump_or_branch_taken = '1' then
          if_id_rst <= '1';

        else
          nx_state <= idle;
          pc_write_enable <= '1';
          if_id_we <= '1';
          id_exmem_we <= '1';
          id_exmem_rst <= '0';
          if_id_rst <= '0';
          exmem_wb_we <= '1';
          
        end if;
        assert false
          report "v"
          severity note;
        
      when others =>
        assert false
          report "HAZARD DETECTION UNIT: Invalid internal state."
          severity error;
        
    end case;
  end process;
end rtl;
