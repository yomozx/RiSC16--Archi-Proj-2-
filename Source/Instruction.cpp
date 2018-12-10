#include <iostream>
#include <string>
#include "Instruction.h"
#include "SIM.h"
using namespace std;

SIM* instruction::sim_ptr = nullptr;

instruction::instruction() : instruction_name(""), operand1(-1),
	operand2(-1), operand3(-1), whole_instruction(""), ready(0), ID(-1) {}

instruction::~instruction() {};

void instruction::display() const
{
	cout << whole_instruction << endl;
}

bool instruction::isReady()
{
	return ready;
}

string instruction::get_name() {
	return instruction_name;
}

int instruction::get_ID()
{
	return ID;
}

void instruction::set_ID(int num)
{
	ID = num;
}

string instruction::get_funcUnit()
{
	return funcUnit;
}
