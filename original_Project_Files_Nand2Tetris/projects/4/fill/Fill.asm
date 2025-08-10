// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// Replace this comment with your code.
@24576  //sett limit to for RAM[SCREEN] to compare to
D=A
@screenLimit
M=D

@0  //set counter for RAM[SCREEN+n]
D=A
@n
M=D
//(RESTART) //Restart RAM[SCREEN] when KBD was == 0 and now is == 1
//@16384    //leaving this code here but no reason to restart. 
//D=A       //SCREEN is a constant variable to cannot change
//@SCREEN   //that is why we use counter n for iterating through RAM[SCREEN+n]
//A=D

(LOOPB) //JUMP to LOOPW if no button is pressed, otherwise LOOPB commences 
@KBD
D=M
@BREAK
D;JEQ

//@SCREEN //If black is still pushed and screenLimit is reached, nothing happens. Screen stays black with no RAM overflow
//D=A 	  //Leaving this here but needs a rewrite to include the n counter.	
//@n
//D=D+M
//@screenLimit
//D=D-M
//@LOOPB
//D;JGT

@n //add -1 to RAM[SCREEN+n]
D=M
@SCREEN
A=D+A
M=-1

@n  //add to n counter for next RAM position in loop
M=M+1
@LOOPB
0;JMP

(BREAK)

(LOOPW) //Jump to LOOPB if a key is pressed, otherwise LOOPW commences
@KBD
D=M
@LOOPB
D;JGT

//@SCREEN   //If white is still pushed and screenLimit is reached, nothing happens. Screen stays black with no RAM overflow
//D=A 		//logic is correct but n must be used. SCREEN is a constant and cannot change its value
//@screenLimit
//D=D-M
//@LOOPW
//D;JGT

@n  //add 0 to RAM[SCREEN-n]
D=M
@SCREEN
A=D+A
M=0

@n 		//-1 to n counter
M=M-1
@LOOPW
0;JMP

@END 	//im not sure if the prog in its current state has a way to END. 
0;JMP



