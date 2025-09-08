This Turing Machine has twelve symbols and knows 2^(sizeof(char)) = 256 integers. :
enum TMInst{ FE, TE, ST, LT, RT, WT, RD, RN, LD, CL, ED, NG = -1 };
FE = FALSE, 
TE = TRUE, 
ST = START, Initialize Machine to zero.
LT = LEFT, Move tape head to the left.
RT = RIGHT, Move tape head to the right.
WT = WRITE, Write TE or FE at tape head.
RD = READ, Read the value at the tape head.
RN = RUN, Run a program until the end.
LD = LOAD, Load a machine state program.
CL = CALL, Conditional Call. 
ED = END, 
NG = NOTHING, Do nothing.

It has a bidirectional tape, which can be extended, with a current limit of (-65,64) = 128 squares.
It also has a std::vector<string> to hold the states of the program for the machine. XD

A WRITE instruction must be followed by either TRUE ("TE") or FALSE ("FE"), so it's a 2-byte instruction.
  { WT, TE } or { WT, FE }
For a RUN instruction, all the bytes that follow until either the end of the string or the END symbol "ED", will be run within its own environment from START.
The LOAD command takes all the bytes that follow until the next LOAD symbol "LD" and pushes it back into the vector of states as a std::string.
The Conditional Call is a four-byte instruction. The fourth byte can be the NOTHING symbol "NG". 
  { CL, TE, 5, 7 } means "If the symbol at the head Reads TRUE, Run state number 5. Otherwise, Run state 7."
  { CL, FE, 5, 7 } means "If head reads FALSE, run state 5, else run state 7."
  { CL, TE, 0, NG } means "It head reads TRUE, go to state 0, else do nothing." 
The END symbol "ED", besides being a syntactic delimiter, also prints the state of the Machine to the console.

