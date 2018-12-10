#include "Inst1OP.h"


bool Inst1OP::validate(string instruction, int last_read)
{
		stringstream str(instruction);
		string temp = "";

		instruction.erase(0, instruction.find(' ') + 1);
		str.str(instruction);
		getline(str, temp);

		// Checking number of parameters
		int counter = 0;
		for (int i = 0; i < instruction.size(); i++)
			if (instruction[i] == ',')
				counter += 1;

		if (counter != 0)
			throw invalid_argument("instruction at index " + to_string(last_read) + " has an incorrect number of parameters.");

		if (instruction_name == "JMP")
		{
			for (int i = 0; i < temp.size(); i++)
				if (!isdigit(temp[i]) && temp[i] != ' ' && temp[i] != '-')
					throw invalid_argument("Operand 0 in instruction at index " + to_string(last_read) + " is written incorrectly.");
		}
		else if (instruction_name == "RET")
		{
			if (temp[0] != 'x')
				throw invalid_argument("Operand 0 in instruction at index " + to_string(last_read) + " is written incorrectly.");
			else
			{
				temp.erase(0, 1);
				for (int j = 0; j < temp.size(); j++)
					if (!isdigit(temp[j]) && temp[j] != ' ')
						throw invalid_argument("Operand 2 in instruction at index " + to_string(last_read) + \
							" is not written in the correct format.");
			}
		}

		return true;
}

int Inst1OP::get_operand1()
{
	return operand1;
}

int Inst1OP::get_result()
{
	return -1;
}

int Inst1OP::get_operand2()
{
	return operand2;
}

int Inst1OP::get_operand3()
{
	return operand3;
}

void Inst1OP::set_instruction(string instruction, int last_read)
{
	whole_instruction = instruction;
	stringstream str(instruction);
	string temp = "";
	getline(str, instruction_name, ' ');

	if (validate(instruction, last_read))
	{
		instruction.erase(0, instruction.find(' ') + 1);
		str.str(instruction);
		getline(str, temp);

		if (instruction_name == "RET")
			temp.erase(0, 1);
		operand1 = stoi(temp);
		operand2 = -1;
		operand3 = -1;
	}
	else
		throw invalid_argument("instruction at index " + to_string(last_read) + " is not written in the correct format.");
}


int Inst1OP::number_operands() {
	return 1;
}