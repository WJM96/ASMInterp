/**************************************/
//			Wesley McClintock
//			Assembler & interpreter
//			Desc: This was a school project.
//					Some of the helper functions, like convertToNumber were
//					provided by the teacher. I either didn't modify them, 
//					or only made minor changes.
/**************************************/


/**************************************
The Labelhandler class is essentially two lists.
	
The first list contains label definitions, basically just
	names and the address they refer to.
The second list contains requests for labels. The labels
	in this list instead refer to the label name being requested, 
	and then the location in memory that requires the address.

**************************************/


#ifndef LABELHANDLER
#define LABELHANDLER

#include <iostream>
#include <list>
#include <string>
using namespace std;

const int ERROR = -1;

//basic label structure, used in LabelHandler, as well as in the assembler
struct Label
{
	string name;
	int address;
};

//Manages label definitions and requests.
class LabelHandler
{
public:
	LabelHandler(void);
	
	Label getLabel(string name);					//finds the address associated with a label.
	bool addLabel(string name, int address);				//adds a label.

	void requestLabel(string name, int address);			//files a request for a label to be filled in
	Label getRequestedLabel();								//pops a label off the labelRequests and returns it

	~LabelHandler(void);
private:
	list<Label> labels;				//used to store labels
	list<Label> labelRequests;		//used to store requests for labels
};
#endif