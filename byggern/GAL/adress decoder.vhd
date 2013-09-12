library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity adress_decoder  is
	Port (
		a11 : In std_logic;
		a10 : In std_logic;
		a9 : In std_logic;
		a8 : In std_logic;

		ram_cs : Out std_logic;
		adc_cs : Out std_logic;
		oled_cs: Out std_logic;
	);
	attribute LOC : string;
	attribute LOC of ram_cs : signal is "P19";
	attribute LOC of adc_cs : signal is "P18";
	attribute LOC of oled_cs: signal is "P17";

	attribute LOC of a11	: signal is "P1";
	attribute LOC of a10	: signal is "P2";
	attribute LOC of a9	: signal is "P3";
	attribute LOC of a8	: signal is "P4";
end adress_decoder;

architecture behavioral of adress_decoder is begin
	oled_cs <= (not a8) and (not a9) and (not a10);
	adc_cs <= (not a8) and a9;
	ram_cs <= a8;
end behavioral;

