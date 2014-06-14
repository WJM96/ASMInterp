/**************************************/
//			Wesley McClintock
//			Assembler & interpreter
//			Desc: This was a school project.
//					Some of the helper functions, like convertToNumber were
//					provided by the teacher. I either didn't modify them, 
//					or only made minor changes.
/**************************************/



//need to create the functions run the array notation. 
#pragma once
#include "assemBase.h"
//#include "assemProj.h"


//I did not write this function, it was provided at the start of class
/***************************************/
int main(int argc, char* argv[])
{
	regis.instrAddr = 0;
	regis.SP = MAX - 1;

	fillMemory();
	cout<<"Here is the memory before run\n";
	printMemoryDump();
	system("pause");
	runProgram();  
	cout<<"\n\nMemory dump after run\n";
	printMemoryDump() ;


	system("pause");
	return 0;
}
/**************************************/

/********************************************************

Helper functions

*********************************************************/
#pragma region helper

/********************************************************
changes the substring into an integer
*********************************************************/
//I did not write this function, it was provided at the start of class
void convertToNumber(string line, unsigned int &start, int &value)
{
	char number[16];
	bool negative = false;
//	cout<< "in convertToNumber before function 1  start is "<<start<<endl;
	int i = 0;
	if (line[start] == '-')
	{
		start++;
		negative = true;
	}
	while (i<16 && line.size() > start&& isdigit(line[start]))
	{
		number[i] = line[start];
	//	cout<<line[start];
		i++;
		start++;
//		cout<<i<<start;
	}
	number[i] = '\0';
	value = atoi(number);
	if (negative == true)
	{
		value = -value;
	}
//	cout<< "in convertToNumber after function 1  start is "<<start<<endl;
}


/********************************************************************
 Displays all of the memory in good columns.
	MAX is the amount of elements in the memory array 
	COL is the number of columns that are to be displayed 
*********************************************************************/
//I did not write this function, it was provided at the start of class
void printMemoryDump()
{
	int numRows=MAX/COL+1; //number of rows to output
	int carryOver=MAX%COL; //number of columns on the bottom row
	int location; //the current location being called
	cout<<setw(3);
	for(int row=0;row<numRows;row++)
	{
		location=row;
		for(int column=0;location<MAX&&column<COL;column++)
		{
			if(!(numRows-1==row&&carryOver-1<column))
			{
				cout<<location<<"."<<setw(5)<<memory[location]<<setw(5);
				location+=(numRows-(carryOver-1<column));
			}
		}
		cout<<endl;
		cout<<setw(3);
	}

	cout<<endl;
	cout<<"AX: "<<regis.AX<<'\t';
	cout<<"BX: "<<regis.BX<<'\t';
	cout<<"CX: "<<regis.CX<<'\t';
	cout<<"DX: "<<regis.DX<<'\t';
	cout<<endl<<endl;
	cout<<"Instruction: "<<regis.instrAddr<<endl;
	cout<<"Flag: "<<regis.flag;
	cout << " SP: " << regis.SP;
	
	cout<<endl<<endl;
}

/*******************************************************************
changes all letters to the same case, lower case
********************************************************************/
//I did not write this function, it was provided at the start of class
void changeToLowerCase(string &line)
{
	unsigned int index=0;
	while (index < line.size())
	{
		line[index] = tolower(line[index]);
		index++;
	}
//	cout<<"the line in change"<<line;

}
#pragma endregion

/*******************************************************************

Assembly related functions

********************************************************************/
#pragma region assemble

/*******************************************************************
Get the file and prepare to move the file into the assembler memory.
********************************************************************/
void fillMemory()
{
	string line;
	cout<<"Enter the assembly file name.  No spaces. filename.asm";
	cin>>line;
	address = 0;
	ifstream fin;
	fin.open(line);
	if (fin.fail())
	{
		cout<<"Error, file didn't open\n";
		system("pause");
		exit(1);
	}

	//I wrote what was below this comment, but above was provided


	//first pass, does everything EXCEPT fill in the labels
	//Spaces that need labels will be a 0. If you see 0s where there shouldn't be,
	//it's probably a label issue.
	while(!fin.fail() && fin.peek() != EOF)	
	{
		if (!convertCommand(&fin))
		{
			cout << "Error @ address [" << address  << "]" <<endl;
		}
	}

	//second pass, fill in all the labels
	Label request, definition;

	request = labelHandler.getRequestedLabel();
	while(request.address != -1)
	{

		definition = labelHandler.getLabel(request.name);
		if(definition.name == "INVALID_LABEL")
		{
			cout << "Label \"" << request.name << "\" not found! @ address: " << request.address << endl;
			fin.close();
			return;
		}
		else
		{
			memory[request.address] = definition.address;
		}

		request = labelHandler.getRequestedLabel();
	}

	fin.close();
}

/****************************************************************
puts machine code into memory, one line per call. returns true if successful.
	fin		input file
*****************************************************************/
bool convertCommand(ifstream* fin)
{
	string instruction, operand1, operand2;
	*fin >> instruction;
	if (fin->fail())	//this evaluates true if there is whitespace at the end of the file. The error message was driving me crazy so I set it to return true;
	{
		return true;
	}
	changeToLowerCase(instruction);


	//if its a comment
	if(instruction.at(0) == ';')
	{
		char temp;
		while(fin->get(temp) && temp != '\n')	// if it's a comment, go until the next line;
		{};

		return true;
	}
	//if its a label DECLARATION, file the label
	else if(instruction.back() == ':')
	{
		if(labelHandler.addLabel(instruction.substr(0, instruction.size() - 1), address) != ERROR)	//I don't use ERROR because it's not defined for LabelHandler
		{
			return true;
		}
		else
		{
			cout <<  "Label " << instruction << " already exists";
			return false;
		}
	}
	else if(instruction == "def")		//I used labels to define constants. you can type in "def null 0" and then you can use "null" like a label, that will be 0. 
	{
		Memory temp;
		*fin >> operand1 >> temp;
		if (fin->fail())
			return false;
		changeToLowerCase(operand1);
		labelHandler.addLabel(operand1, temp);
	}
	else if (instruction == "mov")
	{
		*fin >> operand1 >> operand2;
		if (fin->fail())
			return false;
		changeToLowerCase(operand1);
		changeToLowerCase(operand2);

		if(operand1.size() > 1 && operand1.at(1) == 'x')
		{
			return compInstruction(operand1, operand2, MOV_TO_REG);
		}
		else if (operand1.at(0) == '[' || operand1.at(0) >= 'a' && operand1.at(0) <= 'z')
		{
			return compInstruction(operand1, operand2, MOV_TO_MEM);
		}
		else
		{
			return false;
		}
	}
	else		//handle all the other instructions
	{
		Memory ins = getInstruction(instruction);

		if(ins == HALT || ins == GET || ins == PUT || ins == RFN)	//if the instruction requires no arguments
		{
			memory[address] = ins;
			address++;
			return true;
		}
		else if(ins == FUN)	//the fun instruction is a special snowflake, and gets its own function
		{
			compFUN(fin);
		}
		else if(ins == AND || ins == OR || ins == ADD || ins == SUB || ins == CMP)	//if the instruction requires two arguments
		{
			*fin >> operand1 >> operand2;
			if (fin->fail())
			{
				return false;
			}
			changeToLowerCase(operand1);
			changeToLowerCase(operand2);

			return compInstruction(operand1, operand2, ins);
		}
		else if(ins == JUMP)
		{
			*fin >> operand1;
			if (fin->fail())
			{
				return false;
			}
			changeToLowerCase(operand1);

			memory[address] = JUMP;

			if(instruction == "je")
				memory[address] += JE;
			else if(instruction == "jne")
				memory[address] += JNE;
			else if(instruction == "jb")
				memory[address] += JB;
			else if(instruction == "jbe")
				memory[address] += JBE;
			else if(instruction == "ja")
				memory[address] += JA;
			else if(instruction == "jae")
				memory[address] += JAE;
			else if(instruction == "jmp")
				memory[address] += JMP;

			address++;

			return extractConstToMem(operand1);
		}
		else if(ins == NOT)
		{
			*fin >> operand1;
			if (fin->fail())
			{
				return false;
			}
			changeToLowerCase(operand1);

			Memory type = getOperand(operand1);
			if(type == ERROR)
				return false;

			memory[address] = type + NOT;
			address++;

			if(type > REF_BX)
			{
				return extractConstToMem(operand1);
			}
			else
			{
				return true;
			}
		}
		else if(!extractConstToMem(instruction))
		{
			return false;
		}
	}
}

/****************************************************************
assembles the FUN (call function) instruction.
*****************************************************************/
bool compFUN(ifstream *fin)
{
	memory[address] = FUN;		//no, no its not.
	address++;

	string addr, strNumParams;
	*fin >> addr;
	*fin >> strNumParams;
	if (fin->fail())
	{
		return false;
	}

	//first, we put the address of the function to be called in memory
	if(!extractConstToMem(addr) || !extractConstToMem(strNumParams))
	{
		return false;
	}
	
	//then, we get the number of parameters
	int numParams = memory[address-1];

	//and extract them all
	string param;
	int paramType;
	for(int i = 0; i < numParams; i++)
	{
		*fin >> param;
		if (fin->fail())
			return false;

		paramType = getOperand(param);
		if(paramType != ERROR)
		{
			memory[address] = paramType;
			address++;
			if(paramType > REF_BX) //if its not a register or [bx]
			{
				extractConstToMem(param);
				if(paramType != CONSTANT)
					address++; //leave space for the runtime extraction
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

/****************************************************************
returns the appropriate 3 bit number for an instruction.
	instruction - the string to be converted.
*****************************************************************/
Memory getInstruction(string instruction)
{
	//we don't handle mov because it's finnicky, and is actually two instructions
	if(instruction == "add")
	{
		return ADD;
	}
	else if(instruction == "sub")
	{
		return SUB;
	}
	else if(instruction == "and")
	{
		return AND;
	}
	else if(instruction == "or")
	{
		return OR;
	}
	else if(instruction == "not")
	{
		return NOT;
	}
	else if(instruction == "cmp")
	{
		return CMP;
	} 
	else if(instruction.at(0) == 'j')
	{
		return JUMP;
	}
	else if(instruction == "get")
	{
		return GET;
	}
	else if(instruction == "put")
	{
		return PUT;
	}
	else if(instruction == "fun")
	{
		return FUN;
	}
	else if(instruction == "rfn")
	{
		return RFN;
	}
	else if (instruction == "halt")
	{
		return HALT;
	}
	return ERROR;
}

/****************************************************************
returns the appropriate 3 bit or 2 bit number for a register/argument thingy.
	operand		thestring to be converted
*****************************************************************/
Memory getOperand(string operand)
{	
	if (operand == "ax")
	{
		return REG_AX;
	}
	else if (operand == "bx")
	{
		return REG_BX;
	}
	else if (operand == "cx")
	{
		return REG_CX;
	}
	else if (operand == "dx")
	{
		return REG_DX;
	}
	else if(operand == "[bx]")		//this NEEDS to be before the next else if, otherwise it wont catch the [bx]'s
	{
		return REF_BX;
	}
	else if(operand.at(0) == '[')	//if its a label or a constant mem address
	{
		if(operand.find('+') != string::npos)
			return REF_OFFSET;
		else
			return REF_CONST;
	}
	else if((operand.at(0) == '-' || (operand.at(0) >= '0' && operand.at(0) <= '9')) || operand.at(0) >= 'a' && operand.at(0) <= 'z')
	{
		return CONSTANT;
	}
	else
	{
		cout << "Operand \"" << operand << "\" does not exist!" << endl;
		return ERROR;
	}
}

/****************************************************************
Takes the appropriate bits, shifts them into place, and increments the address.
this function is just to save space. It only works with commands of the syntax
operation op1 op2. NOT special like not and jmp.

	operand1	string to be converted
	operand2	string to be converted
	int i		the instruction
*****************************************************************/
bool compInstruction(string operand1, string operand2, int i)
{	

	int op1 = getOperand(operand1);
	int op2 = getOperand(operand2);

	//this makes sure that op1 is ALWAYS equal to a register
	if(i == MOV_TO_MEM)
	{
		swap(op1, op2);
	}

	memory[address] = i;
	memory[address] += (op1 << 3);
	memory[address] += op2;

	address++;

	//handle the operands
	if(i == MOV_TO_MEM && op2 != REF_BX)	//if its mov to mem
	{
		return extractConstToMem(operand1);		//if it gets here, it must be a constant address
	}
	else if (op2 > REF_BX) //if its just a normal operation AND needs a constant in memory. otherwise, no const need be extracted
	{
		return extractConstToMem(operand2);	
	}

	return true;
}

/****************************************************************
Any time an operand in the source needs a constant extracted to memory (anything that isnt a register), this function does it.
It can take: label [label] [label+bx] ### [###] [###+bx]
will return false if it failed.
*****************************************************************/
bool extractConstToMem(string operand)
{
	int startBr = operand.find('[');
	int endBr = operand.find(']');
	int plus = operand.find('+');

	unsigned startNumLabel = 0;
	unsigned endNumLabel = 0;

	if(startBr != string::npos)
	{
		startNumLabel = startBr + 1;
		if(plus != string::npos)
		{
			endNumLabel = plus;
		}
		else if(endBr != string::npos)
		{
			endNumLabel = endBr;
		}
		else
		{
			return false;
		}
	}
	else
	{
		startNumLabel = 0;
		endNumLabel = operand.length();
	}

	string numLabel = operand.substr(startNumLabel, endNumLabel - startNumLabel);
	
	if((numLabel.at(0) >= '0' && numLabel.at(0) <= '9') || numLabel.at(0) == '-')	//if its just a constant
	{
		unsigned int start = 0;
		int temp = 0;
		convertToNumber(numLabel, start, temp);
		memory[address] = temp;
		address++;
	}
	else   //if its a label
	{
		labelHandler.requestLabel(numLabel, address);
		address++;
	}
	
	return true;
	
}
#pragma endregion 

/****************************************************************

Program run functions

*****************************************************************/
#pragma region run

/****************************************************************
Executes the program, starting from 0 and going till halt.
*****************************************************************/
void runProgram()
{
	Memory instruction = 0; //the instruction on its own
	Memory op1 = 0;			//the first operand
	Memory op2 = 0;			//the second operand

	bool running = true; //used to exit the while loop w/o a bunch of returns
	while (memory[regis.instrAddr] != HALT && running)
	{

		instruction = (memory[regis.instrAddr] >> 5) << 5;
		getOps(memory[regis.instrAddr], op1, op2);
		if (instruction != SPECIAL)
		{
			running = doCommand(op1, op2, instruction);
			regis.instrAddr++;
		}
		else
		{

			if(op1 << 3 == JUMP)
			{
				if(op2 == JE && regis.flag == 0)
					regis.instrAddr = memory[regis.instrAddr + 1];
				else if(op2 == JNE && regis.flag != 0)
					regis.instrAddr = memory[regis.instrAddr + 1];
				else if(op2 == JB && regis.flag == -1)
					regis.instrAddr = memory[regis.instrAddr + 1];
				else if(op2 == JBE && regis.flag < 1)
					regis.instrAddr = memory[regis.instrAddr + 1];
				else if(op2 == JA && regis.flag == 1)
					regis.instrAddr = memory[regis.instrAddr + 1];
				else if(op2 == JAE && regis.flag > -1)
					regis.instrAddr = memory[regis.instrAddr + 1];
				else if(op2 == JMP)
					regis.instrAddr = memory[regis.instrAddr + 1];
				else
					regis.instrAddr += 2; //so we skip over the jump address
			}
			else if(op1 == 0)
			{
				if (op2 == GET)
				{
					get();
					regis.instrAddr++;
				}
				else if(op2 == PUT)
				{
					put();
					regis.instrAddr++;
				}
				else if (op2 == FUN)
				{
					running = callFunc();
				}
				else if (op2 = RFN)
				{
					running = returnFunc();
				}
			}
			else if(op1 << 3 == NOT)
			{
				Memory* target = getMemFromOp(op2);
				if(target == NULL)
				{
					running = false;
				}
				else
				{
					*target = ~*target;
					regis.instrAddr++;
				}
			}

		}
	}

	if (!running)	//if the program was stopped prematurely
	{
		cout << "Runtime Error! @ address[" << regis.instrAddr << "]" << endl;
	}
}

/********************************************************
takes a full Opcode and isolates each operand
	full	the full opcode
	op1		an operand that gets "returned" by reference
	op2		an operand that gets "returned" by reference
*********************************************************/
void getOps(Memory full, Memory &op1, Memory &op2)
{
	op1 = (full >> 3) & 3;		//this shifts the mid 2 bits to the bottom, then ands out everything else;
	op2 = (full & 7);			//this ANDS out everything that isn't in the lower 3

	if((full >> 5) << 5 == MOV_TO_MEM)
	{
		swap(op1, op2);
	}
}

/********************************************************
callFunc executes the FUN instruction. Mainly, it places things on the stack
and puts the appropriate operands in memory.

	fun pushes(in order) the stack pointer, flag, registers(from dx->ax),
	and instruction pointer(that points to the next instruction).
	bx is then set the new stack pointer, and the program jumps to the function.
	arguments are stored in memory //after the function call//
	so in memory it looks like :
		fun
		<address>
		<number of arguments>

		<type of argument>
		<argument value(if needed)>
		and so on...

	arguments can be accessed in the function like so:
		mov bx [bx]			;bx = oldInstructionPointer
		sub bx 1			;bx now points to the last argument
		mov ax [bx]			;ax = lastArg

*********************************************************/
bool callFunc()
{
	int caller = regis.instrAddr;

	regis.instrAddr++;
	int dest = memory[regis.instrAddr];
	regis.instrAddr++;
	int numParams = memory[regis.instrAddr];
	regis.instrAddr++;

	Memory type; 
	Memory sourceAddress;	//not used by registers
	for (int i = 0; i < numParams; i++)
	{
		type = memory[regis.instrAddr];
		regis.instrAddr++;

		switch (type)
		{
		case REG_AX:
		case REG_BX:
		case REG_CX:
		case REG_DX:
			break;
		case CONSTANT:
			regis.instrAddr++;
			break;

		case REF_CONST:
			sourceAddress = memory[regis.instrAddr];
			regis.instrAddr++;
			memory[regis.instrAddr] = sourceAddress;
			break;

		case REF_OFFSET:
			sourceAddress = memory[regis.instrAddr] + regis.BX;
			regis.instrAddr++;
			if (sourceAddress > MAX || sourceAddress < 0)
				return false;
			memory[regis.instrAddr] = sourceAddress;
			break;

		case REF_BX:
			if (regis.BX > MAX || regis.BX < 0) //make sure we aren't accessing out of memory
				return false;
			memory[regis.instrAddr] = memory[regis.BX];
			break;

		default:
			return false;
		}
	}
	if (regis.SP != MAX - 1)
		regis.SP--;
	memory[regis.SP] = regis.SP;
	regis.SP--;
	memory[regis.SP] = regis.flag;
	regis.SP--;
	memory[regis.SP] = regis.DX;
	regis.SP--;
	memory[regis.SP] = regis.CX;
	regis.SP--;
	memory[regis.SP] = regis.BX;
	regis.SP--;
	memory[regis.SP] = regis.AX;
	regis.SP--;
	memory[regis.SP] = regis.instrAddr;
	regis.BX = regis.SP;

	regis.instrAddr = dest;

}

/********************************************************
just pops all the appropriate things off the stack. returns false 
if theres not enough stuff to pop off.
*********************************************************/
bool returnFunc()
{
	if (regis.SP > MAX - 7 )
		return false;
	
	regis.instrAddr = memory[regis.SP];
	regis.SP++;

	regis.AX = memory[regis.SP];
	regis.SP++;

	regis.BX = memory[regis.SP];
	regis.SP++;

	regis.CX = memory[regis.SP];
	regis.SP++;

	regis.DX = memory[regis.SP];
	regis.SP++;

	regis.flag = memory[regis.SP];
	regis.SP++;

	regis.SP++;
	
}

/********************************************************
Returns a pointer to a piece of memory or a register based off the operand.
Returns NULL if failed. The purpose of this function is to save space so I 
don't have a ton of gigantic switch statements. Probably not as safe as it could be.
*********************************************************/
Memory* getMemFromOp(Memory operand)
{
	switch (operand)
	{
		case REG_AX:
			return &(regis.AX);
			break;
		case REG_BX:
			return &(regis.BX);
			break;
		case REG_CX:
			return &(regis.CX);
			break;
		case REG_DX:
			return &(regis.DX);
			break;
		case REF_CONST:
			regis.instrAddr++; //because we grabbed the const from the next memory spot, we want to increment this
			return &memory[memory[regis.instrAddr]];
			break;
		case REF_OFFSET:
			regis.instrAddr++; //because we grabbed the const from the next memory spot, we want to increment this
			if(memory[regis.instrAddr] + regis.BX > MAX || memory[regis.instrAddr] + regis.BX < 0)
				return NULL;
			return &memory[memory[regis.instrAddr] + regis.BX];
			break;
		case CONSTANT:
			regis.instrAddr++; //because we grabbed the const from the next memory spot, we want to increment this
			return &memory[regis.instrAddr];
			break;
		case REF_BX:
			if(regis.BX > MAX || regis.BX < 0) //make sure we aren't accessing out of memory
				return NULL;
			return &memory[regis.BX];
			break;
		default: 
			return NULL;
	}
}

/********************************************************
Takes in two operators and the instruction, and executes it.
		op1		the first operand
		op2		the second operand
		operation the instruction 
											function that takes two operands by reference
*********************************************************/
bool doCommand(Memory op1, Memory op2, Memory operation)
{
	Memory* dest = getMemFromOp(op1);
	Memory* source = getMemFromOp(op2);
	if(dest == NULL || source == NULL)
			return false;

	switch(operation)
	{
	case MOV_TO_REG:
	case MOV_TO_MEM:
		*dest = *source;
		break;
	case ADD:
		*dest += *source;
		break;
	case SUB:
		*dest -= *source;
		break;
	case AND:
		*dest &= *source;
		break;
	case OR:
		*dest |= *source;
		break;
	case CMP:
		if(*dest < *source)
		regis.flag = -1;
		else if(*dest > *source)
			regis.flag = 1;
		else
			regis.flag = 0;
		break;
	default:
		return false;
	}

	return true;
}

/********************************************************
	I/O functions, very simple.
*********************************************************/
#pragma region io
//outputs AX
void put()
{
	cout << regis.AX << endl;
}

//gets input for AX
void get()
{
	cout << "<enter a number>:";
	cin >> regis.AX;
}
#pragma endregion

#pragma endregion
