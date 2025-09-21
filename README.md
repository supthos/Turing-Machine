<p>
  This Turing Machine is a Model which I claim is Turing-Complete.
</p>
<p>It has twelve symbols and knows 2^(sizeof(char)) = 256 integers. :
</br>FE = FALSE, 
</br>TE = TRUE, 
</br>ST = START, Initialize Machine to zero.
</br>LT = LEFT, Move tape head to the left.
</br>RT = RIGHT, Move tape head to the right.
</br>WE = WRITE, Write TE or FE at tape head.
</br>RD = READ, Read the value at the tape head.
</br>RN = RUN, Run a program until the end.
</br>LD = LOAD, Load a machine state program.
</br>CL = CALL, Conditional Call. 
</br>ED = END, 
</br>NG = NOTHING, Do nothing.

<p>It has a bidirectional tape, with an initial size of 1024*64 squares, extensible with a proper way of addressing.
It also has a std::vector<string> to hold the states of the program for the machine. XD
</br>If START is followed by a positive number n, the machine will have, initially, a tape of 2^n squares.

<p>A WRITE instruction must be followed by either TRUE ("TE") or FALSE ("FE"), so it's a 2-byte instruction.
</br>  { WE, TE } or { WE, FE }

<p>For a RUN instruction, all the bytes that follow until either the end of the string or the END symbol "ED", will be run within its own environment from START.

<p>The LOAD command takes all the bytes that follow until the next LOAD symbol "LD" and pushes it back into the vector of states as a std::string.

<p>The Conditional Call has a four-byte instruction. The fourth byte can be the NOTHING symbol "NG". 
</br>  { CL, TE, 5, 7 } means "If the symbol at the head Reads TRUE, Run state number 5. Otherwise, Run state 7."
</br>  { CL, FE, 5, 7 } means "If head reads FALSE, run state 5, else run state 7."
</br>  { CL, TE, 0, NG } means "It head reads TRUE, go to state 0, else do nothing." 

<p>The END symbol "ED", besides being a syntactic delimiter, also prints the state of the Machine to the console.

