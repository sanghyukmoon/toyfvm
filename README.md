# toyfvm
Minimal 1-D FVM code for learning...
currently, the code solves a very simple advection equation
  q_t + u q_x = 0 
with periodic boundary condition

Since code is so small, everything goes into main.cpp by now.

# compile & run
g++ -std=c++11 main.cpp
./a.out

# inputs.txt
input parameter goes here.

# output files
first column : x coordinate
second column : conserved quantity $q$
