#ifndef INST3OP_H
#define INST3OP_H
#include "Instruction.h"
#include <string>
#include <sstream>
using namespace std;

class Inst3OP : public instruction {
public:
	void set_instruction(string instruction, int last_read);
	bool validate(string instruction, int last_read);
	virtual void issue() = 0;
	virtual void execute() = 0;
	virtual void writeback() = 0;
};
#endif