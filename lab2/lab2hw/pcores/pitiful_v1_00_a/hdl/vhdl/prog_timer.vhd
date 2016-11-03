library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all ;

entity mod_n_counter is
	Generic (
		WIDTH : natural := 32
	);
	 Port ( clk : in  STD_LOGIC;
           ctrl: in STD_LOGIC_VECTOR(WIDTH-1 downto 0) := (others=>'0');
			  delay: inout STD_LOGIC_VECTOR(WIDTH-1 downto 0) := (others=>'0');
           interrupt : out  STD_LOGIC);
end mod_n_counter;

--- Control Register Pin Layout
-- 0 : allows the counter to decrement if set to '1', holds the counter at its current value if set to a '0'.
-- 1 : enables interrupts if set to a '1', disables interrupts if set to a '0'.
-- 2 :  if set to '1', the timer-counter reloads with the contents of the delay-value register when it reaches 0.
--			 No reload is performed if the bit is '0'.
-- 3 : Reset



architecture Behavioral of mod_n_counter is
	signal r_reg: unsigned(WIDTH-1 downto 0) := (others=>'1'); -- default value is FFFFFFFFin 1's
	signal r_next: unsigned(WIDTH-1 downto 0) := (others=>'1');
	signal r_reg_delay: unsigned(WIDTH-1 downto 0) := (others=>'0');
	signal r_next_delay: unsigned(WIDTH-1 downto 0) := (others=>'0');
begin
-- register
process(clk,ctrl)
begin
	if(ctrl(3)='1') then -- asynchronous clock reset
		r_reg <= r_reg_delay-1; -- not 0's because of requirements to start from value 
		r_reg_delay <= (others=>'1'); -- reset to FFFFFF
	elsif(clk'event and clk='1') then
		r_reg <= r_next;
		r_reg_delay <= r_next_delay;
	end if;
end process;

--next state logic (priority based)
r_next <= 	r_reg - 1 when (ctrl(0)='1' and r_reg > 0) else
				r_reg_delay when (ctrl(0)='1' and r_reg <= 0) else
				r_reg_delay when (ctrl(2)='1') else								-- Redundant, but keeps priority
				r_reg;

r_next_delay <= unsigned(delay)-1 when (ctrl(2)='1') else
						r_reg_delay;
-- output logic
interrupt <= '1' when (r_reg=0 and ctrl(1)='1')else
			'0';
delay <= std_logic_vector(r_reg_delay);
end Behavioral;