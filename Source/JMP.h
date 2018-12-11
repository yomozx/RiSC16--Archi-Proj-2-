#ifndef JMP_H
#define JMP_H
#include "Inst1OP.h"
#include "Memory.h"
using namespace std;

class JMP : public Inst1OP {
private:
	int immediate;
public:
	JMP();
	void issue();
	bool execute();
	void writeback();
	void commit();
	virtual bool ops_ready() { return true; };
};

inline JMP::JMP() {
	cycles = 2;
	funcUnit = "JMP";
}

inline void JMP::issue() 
{
	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

	immediate = operand1;
}

inline bool JMP::execute()
{
	cycles--;

	if (cycles == 0) {
		int address = sim_ptr->get_pc() + 1 + immediate + sim_ptr->get_startingAddr();
		sim_ptr->set_pc(address);
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