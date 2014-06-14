/**************************************/
//			Wesley McClintock
//			Assembler & interpreter
//			Desc: This was a school project.
//					Some of the helper functions, like convertToNumber were
//					provided by the teacher. I either didn't modify them, 
//					or only made minor changes.
/**************************************/

#include "LabelHandler.h"

//constructor
LabelHandler::LabelHandler(void)
{
}

/********************************************************
	Finds and returns the address of an EXISTING label. -1 if not found.
*********************************************************/
Label LabelHandler::getLabel(string name)
{
	Label label = {"INVALID_LABEL", -1};
	for (std::list<Label>::iterator iterator = labels.begin(); iterator != labels.end(); iterator++)
	{
		if (iterator->name == name)
		{
			label = *iterator;
		}
	}
	return label;
}
/********************************************************
	Registers a new label. Will return false if the label is already defined.
	Address in this case is the address of label.
*********************************************************/
bool LabelHandler::addLabel(string name, int address)
{
	Label temp;
	
	if(getLabel(name).address == -1)
	{
		temp.name = name;
		temp.address = address;
		labels.push_back(temp);
		return true;
	}
	else
	{
		cout << "Label \"" << name << "\" already defined!";
		return false;
	}
}

/********************************************************
	Files a request for a label address to be filled in. 
	In this case, the address is the location that NEEDS the address
*********************************************************/
void LabelHandler::requestLabel(string name, int address)
{
	Label temp;
	temp.name = name;
	temp.address = address;
	
	labelRequests.push_back(temp);
}

/********************************************************
	Pops a label request of the back of the vector. Used when filling
	in addresses after first pass assembly. 
	Returns a label with -1 address and name "INVALID_LABEL" if there are no more label requests.
*********************************************************/
Label LabelHandler::getRequestedLabel()
{
	Label temp;
	temp.name = "INVALID_LABEL";
	temp.address = -1;
	
	if(labelRequests.size() > 0)
	{
		temp = labelRequests.back();
		labelRequests.pop_back();
		return temp;
	}
	else
	{
		return temp;
	}
}

//destructor
LabelHandler::~LabelHandler(void)
{
}
