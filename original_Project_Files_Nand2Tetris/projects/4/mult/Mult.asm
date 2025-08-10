// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//// Replace this comment with your code.
//sets n counter = o
@n  
M=0
//Sets R2 to 0
@R2
M=0
//Test n == R0, goes to END if TRUE, if FALSE carry's on with the LOOP
@R0
D=M
@n
D=D-M
@END
D;JEQ

(LOOP)
//Test n == R1, goes to END if TRUE, if FALSE carrys on with the LOOP
@R1
D=M
@n
D=D-M
@END
D;JEQ

//R2=R2+R0
@R2
D=M
@R0
D=D+M
@R2
M=D

//+1 to n counter
@n
M=M+1
@LOOP
0;JMP

//END
(END)
@END
0;JMP



