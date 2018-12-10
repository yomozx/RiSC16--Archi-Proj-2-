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
	int ID;
	int operand2;
	int operand3;
	int cycles;
	bool ready;
	string funcUnit;
	static SIM *sim_ptr;

public:
	instruction();
	~instruction();
	virtual void set_instruction(string instruction, int last_read) = 0;
	virtual bool validate(string instruction, int last_read) = 0;
	virtual void issue() = 0;
	virtual bool execute() = 0;
	virtual void writeback() = 0;
	virtual void commit() = 0;
	virtual int get_result() = 0;
	virtual int get_operand1() = 0;
	virtual int get_operand2() = 0;
	virtual int get_operand3() = 0;
	virtual int number_operands() = 0; //gets number of operands
	string get_name();
	int get_ID();
	void set_ID(int);
	string get_funcUnit();
	void display() const;
	bool isReady();
};
#endif