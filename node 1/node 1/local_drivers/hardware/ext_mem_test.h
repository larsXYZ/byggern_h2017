library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
entity address
_
decoder is
Port (
a11 : In std_logic;
a10 : In std_logic;
a9
: In std_logic;
a8  : In std_logic;
ram
_cs
: Out std_logic;
adc
_cs
: Out std_logic;
oled_cs
: Out std_logic;
oled_dc
:
Out std_logic
--
Not necessary
...
);
attribute LOC : string;
attribute LOC of
ram_cs
: signal is "P19";
attribute LOC of
adc
_cs
: signal is "P18";
attribute LOC of
oled
_
cs
: signal is "P17";
attribute LOC of
oled
_
dc
: signal is "P16";
--
Not necessary...
attr
ibute LOC of a11
: signal is "P
1
";
attribute LOC of a10
: signal
is "P
2
";
attr
ibute LOC of a9
: signal is "P
3
";
attri
bu
te LOC of a8
: signal is "P
4
";
end address
_
decoder;
architecture behave of address
_
decoder is
begin
--
implement the functionality here
end
behave;