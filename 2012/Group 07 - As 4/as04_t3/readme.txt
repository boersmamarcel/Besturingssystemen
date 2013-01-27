The goal is generating a string such that i contains the following value: 0xdeadbeef

The stack buffer value address is 0x78 which equals 120 when converted to decimals.
The i address is 0xC which equals 12 when converted to decimals.
In order to overwrite the i value with 0xdeadbeef the memory cells in between must be filled with values. This can be done by passing a string to the program long enough to fill that space.

This is the memory layout:
(buffer):120 ---------------------------|something-----|(i)12----|rest of the program
Memory values should be something like this:
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|0xdeadbeef|rest of the program

The X characters should fill up the space between 120 and 12 which is 108 characters. Furthermore should the tail contain 0xdeadbeef.

The following perl command generates the 108 characters and 0xdeadbeef on the end:
perl -e 'print "X"x108; print "\xef\xbe\xad\xde"'

\x means that the value should be interpret as a hexadecimal value and due to the little-endian (little end first in our case ef and not de) structure the order should be reversed.

So the following command is the command that exploits the bug:
./as4_t3 `perl -e 'print "X"x108; print "\xef\xbe\xad\xde"'`



