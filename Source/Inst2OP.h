#ifndef INST2OP_H
#define INST2OP_H
#include <sstream>
#include <string>
#include "Instruction.h"
using namespace std;

class Inst2OP : public instruction
{
	public:
		void set_instruction(string instruction, int last_read);
		bool validate(string instruction, int last_read);
		virtual void issue() = 0;
		virtual void execute() = 0;
		virtual void writeback() = 0;
};


#endif
