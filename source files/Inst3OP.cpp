#include "Inst3OP.h"

bool Inst3OP::validate(string instruction, int last_read)
{
		stringstream str(instruction);
		string temp = "";
		bool flag = true;

		instruction.erase(0, instruction.find(' ') + 1);
		str.str(instruction);

		// Check number of parameters is correct
		int counter = 0;
		for (int i = 0; i < instruction.size(); i++)
			if (instruction[i] == ',')
				counter += 1;

		if (counter != 2)
			throw invalid_argument("instruction at index " + to_string(last_read) + " has an incorrect number of parameters.");

		// Validating first two operands
		getline(str, temp, ',');
		for (int i = 0; i < 2; i++) {
			// checking if another symbol is used
			if (temp[0] != 'x')
				throw invalid_argument("instruction at index " + to_string(last_read) + " is not written in the correct format.");
			else 
			{
				temp.erase(0, 1);
				for (int j = 0; j < temp.size(); j++)
					if (!isdigit(temp[j]))
						throw invalid_argument("Operand " + to_string(i) + " in instruction at index " + to_string(last_read) + \
							" is not written in the correct format.");
			}
			instruction.erase(0, instruction.find(',') + 1);
			str.str(instruction);
			if (flag) {
				getline(str, temp, ',');
				flag = false;
			}
		}

		// validating third operand
		getline(str, temp);
		if (instruction_name == "LW" || instruction_name == "SW" || instruction_name == "BEQ" \
			|| instruction_name == "ADDI") {
			for (int i = 0; i < temp.size(); i++)
				if (!isdigit(temp[i]) && temp[i] != ' ' && temp[i] != '-')
					throw invalid_argument("instruction at index " + to_string(last_read) + " has invalid parameters.");
			if (instruction_name == "LW" || instruction_name == "SW") 
				if (stoi(temp) < -64 || stoi(temp) > 63)
					throw invalid_argument("Operand 2 in instruction at index " + to_string(last_read) + " is written incorrectly.");
		}

		else if (instruction_name == "ADD" || instruction_name == "SUB" || instruction_name == "NAND" \
			|| instruction_name == "MUL") {
			if (temp[0] != 'x')
				throw invalid_argument("instruction at index " + to_string(last_read) + " is not written in the correct format.");
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

void Inst3OP::set_instruction(string instruction, int last_read)
{
	whole_instruction = instruction;
	string temp = "";
	stringstream str(instruction);
	getline(str, instruction_name, ' ');

	if (validate(instruction, last_read))
	{
		instruction.erase(0, instruction.find(' ') + 1);
		str.str(instruction);

		// Reading first operand
		getline(str, temp, ',');
		temp.erase(0, 1);
		operand1 = stoi(temp);
		instruction.erase(0, instruction.find(',') + 1);
		str.str(instruction);

		// Reading second operand
		getline(str, temp, ',');
		temp.erase(0, 1);
		operand2 = stoi(temp);
		instruction.erase(0, instruction.find(',') + 1);
		str.str(instruction);

		// Reading third operand
		getline(str, temp, '\n');
		if (instruction_name == "ADD" || instruction_name == "SUB" || instruction_name == "NAND" || \
			instruction_name == "MUL")
			temp.erase(0, 1);
		operand3 = stoi(temp);
	}
	else
		throw invalid_argument("instruction at index " + to_string(last_read) + " is not written in the correct format.");
}