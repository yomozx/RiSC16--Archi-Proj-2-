#ifndef INST1OP_H
#define INST1OP_H
#include <string>
#include <sstream>
#include "Instruction.h"
using namespace std;

class Inst1OP : public instruction {
public:
	void set_instruction(string instruction, int last_read);
	bool validate(string instruction, int last_read);
	virtual void issue() = 0;
	virtual bool execute() = 0;
	virtual void writeback() = 0;
	virtual bool ops_ready() = 0;
	int get_operand1();
	int get_result();
	int get_operand2();
	int get_operand3();
	int number_operands();
};
#endif