#ifndef JMP_H
#define JMP_H
#include "Inst1OP.h"
#include "Memory.h"
using namespace std;

class JMP : public Inst1OP {
private:
	int immediate, mypc;
public:
	JMP(SIM*);
	void issue();
	bool execute();
	void writeback();
	void commit();
	virtual bool ops_ready() { return true; };
};

inline JMP::JMP(SIM* ptr) {
	cycles = 2;
	funcUnit = "JMP";
	sim_ptr = ptr;
}

inline void JMP::issue() 
{
	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

	immediate = operand1;
	mypc = sim_ptr->get_startingAddr() + this->get_ID();

	int address =  mypc + 1 + immediate;
	sim_ptr->set_pc(address);
	sim_ptr->flush_iq();
}

inline bool JMP::execute()
{
	cycles--;

	if (cycles == 0) {
		return true;
	}
	else
		return false;
}

inline void JMP::writeback() 
{
	ready = 1;
}

inline void JMP::commit() {}

#endif