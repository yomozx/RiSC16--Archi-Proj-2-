#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>
using namespace std;

class SIM;

class instruction {
protected:
	
	string instruction_name;
	string whole_instruction;
	int operand1;
	int operand2;
	int operand3;
	static SIM *sim_ptr;
	
public:
	instruction();
	~instruction();
	virtual void set_instruction(string instruction, int last_read) = 0;
	virtual bool validate(string instruction, int last_read) = 0;
	virtual void issue() = 0;
	virtual void execute() = 0;
	virtual void writeback() = 0;
	string get_name();
	void display() const;
};
#endif