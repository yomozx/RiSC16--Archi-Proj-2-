#include "Inst2OP.h"

bool Inst2OP::validate(string instruction, int last_read)
{
		stringstream str(instruction);
		string temp = "";
		instruction.erase(0, instruction.find(' ') + 1);
		str.str(instruction);

		// Checking number of parameters
		int counter = 0;
		for (int i = 0; i < instruction.size(); i++)
			if (instruction[i] == ',')
				counter += 1;

		if (counter != 1)
			throw invalid_argument("instruction at index " + to_string(last_read) + " has an incorrect number of parameters.");

		// Validating first operand
		getline(str, temp, ',');
		if (temp[0] != 'x')
			throw invalid_argument("instruction at index " + to_string(last_read) + " is not written in the correct format.");
		else
		{
			temp.erase(0, 1);
			for (int j = 0; j < temp.size(); j++)
				if (!isdigit(temp[j]))
					throw invalid_argument("Operand 0 in instruction at index " + to_string(last_read) + \
						" is not written in the correct format.");
		}
		instruction.erase(0, instruction.find(',') + 1);
		str.str(instruction);

		// Validating second operand
		getline(str, temp);
		if (temp[0] != 'x')
			throw invalid_argument("instruction at index " + to_string(last_read) + " is not written in the correct format.");
		else
		{
			temp.erase(0, 1);
			for (int j = 0; j < temp.size(); j++)
				if (!isdigit(temp[j]) && temp[j] != ' ')
					throw invalid_argument("Operand 0 in instruction at index " + to_string(last_read) + \
						" is not written in the correct format.");
		}

		return true;
}

int Inst2OP::get_result()
{
	return result;
}

int Inst2OP::get_operand1()
{
	return operand1;
}

int Inst2OP::get_operand2()
{
	return operand2;
}

void Inst2OP::set_instruction(string instruction, int last_read)
{
	whole_instruction = instruction;
	stringstream str(instruction);
	string temp = "";
	getline(str, instruction_name, ' '); // Reading instruction name

	if (validate(instruction, last_read))
	{
		instruction.erase(0, instruction.find(' ') + 1);
		str.str(instruction);

		getline(str, temp, ',');
		temp.erase(0, 1);
		operand1 = stoi(temp);
		instruction.erase(0, instruction.find(',') + 1);
		str.str(instruction);

		getline(str, temp);
		temp.erase(0, 1);
		operand2 = stoi(temp);
		operand3 = -1;
	}
	else
		throw invalid_argument("instruction at index " + to_string(last_read) + " is not written in the correct format.");

}


int Inst2OP::number_operands() {
	return 2;
}