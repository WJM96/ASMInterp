/**************************************/
//			Wesley McClintock
//			Assembler & interpreter
//			Desc: This was a school project.
//					Some of the helper functions, like convertToNumber were
//					provided by the teacher. I either didn't modify them, 
//					or only made minor changes.
//			
//					I'm aware that having global variables and a slew of functions may not be
//					the best structure, or take advantage of C++. 
/**************************************/


#ifndef ASSEMPROJ_H
#define ASSEMPROJ_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "LabelHandler.h"

using namespace std;

const int MAX = 500;  //size of simulators memory
const int COL = 7;	 //number of columns for output

typedef short int Memory;   //creates a new type so we are not using short int over and over.  If the program changes so memory needs to be larger only the typedef needs to change
//I changed it to unsigned because all of our jumps are unsigned

//Opcode constants
//registers
const int REG_AX = 0;
const int REG_BX = 1;
const int REG_CX = 2;
const int REG_DX = 3;

//operand codes, unshifted because their position may vary
const int CONSTANT = 7;
const int REF_BX = 4;
const int REF_OFFSET = 5;
const int REF_CONST = 6;

//special codes
const int JUMP = 1 << 3;
const int NOT = 2 << 3;
const int FUN = 0;
const int RFN = 1;

const int JE = 0;
const int JNE = 1;
const int JB = 2;
const int JBE = 3;
const int JA = 4;
const int JAE = 5;
const int JMP = 6;

//instruction codes, shifted because their position is fixed
const int HALT = 5;
const int SPECIAL = 0;
const int OR = (1 << 5);
const int AND = (2 << 5);
const int CMP = (3 << 5);
const int SUB = (4 << 5);
const int ADD = (5 << 5);
const int MOV_TO_REG = (6 << 5);
const int MOV_TO_MEM = (7 << 5);

//I/O codes
const int GET = 6;
const int PUT = 7;

LabelHandler labelHandler;

struct Registers
{
	Memory AX;
	Memory BX;
	Memory CX;
	Memory DX;
	Memory instrAddr;		//what line of code when the program in running
	Memory flag;			//compare flag
	Memory SP;
} regis;  // a global variable containing the current registers.

int address;							//current address while being created
extern Memory memory[MAX] = {0};		//all of memory, used and modified by most functions  

//prototypes
void printMemoryDump();													//prints memeory with integers commands
void convertToNumber(string line, unsigned int &start, int &value);		//takes in a string and changes it to number
void fillMemory();														//prepares to move the file into memory
void changeToLowerCase(string &line);									//makes sure everything is in lower case

bool convertCommand(ifstream *fin);										//converts one line to machine code in memory
bool compFUN(ifstream *fin);
Memory getOperand(string operand);										//returns the operand, registers or constants etc
Memory getInstruction(string instruction);								//returns the instruction code. ERROR if not an instruction

bool compInstruction(string operand1, string operand2, int i);			//compiles i, op1, and op2 into a single Memory unit. Advances memory
bool extractConstToMem(string operand);									//extracts the first constant in a string to memory, returns false if no constant is found

Memory* getMemFromOp(Memory operand);									//returns a pointer to a register or memory location based off of operand
void getOps(Memory full, Memory &op1, Memory &op2);						//isolates the operands. The oposite of compIRM
bool doCommand(Memory op1, Memory op2, Memory operation);				//moves something to a register
bool callFunc();
bool returnFunc();

void get();						//gets a number and stores it in ax
void put();						//prints the number in ax

void runProgram();
#endif