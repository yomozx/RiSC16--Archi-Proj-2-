#ifndef INST3OP_H
#define INST3OP_H
#include "Instruction.h"
#include <string>
#include <sstream>
using namespace std;

class Inst3OP : public instruction {
protected:
	int result;
public:
	void set_instruction(string instruction, int last_read);
	bool validate(string instruction, int last_read);
	virtual void issue() = 0;
	virtual bool execute() = 0;
	virtual void writeback() = 0;
	int get_result();
	int get_operand1();
	int get_operand2();
	int get_operand3();
	int number_operands();
};
#endif