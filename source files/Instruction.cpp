#include <iostream>
#include <string>
#include "Instruction.h"
#include "SIM.h"
using namespace std;

SIM* instruction::sim_ptr = nullptr;

instruction::instruction() : instruction_name(""), operand1(-1),
	operand2(-1), operand3(-1), whole_instruction("") {}

instruction::~instruction() {};

void instruction::display() const
{
	cout << whole_instruction << endl;
}

string instruction::get_name() {
	return instruction_name;
}