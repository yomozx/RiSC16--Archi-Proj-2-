#ifndef RET_H
#define RET_H
#include "Inst1OP.h"
#include "Memory.h"
using namespace std;

class RET : public Inst1OP {
private:
	int return_address;
	instruction* p;
	bool valid;
public:
	RET(SIM*);
	void issue();
	bool execute();
	void writeback();
	void commit();
	virtual bool ops_ready() { return true; };
};

inline RET::RET(SIM* ptr) {
	cycles = 2;
	funcUnit = "JMP";
	valid = true;
	sim_ptr = ptr;
}

inline void RET::issue()
{
	if (sim_ptr->get_RAT(operand2) == nullptr) return_address = sim_ptr->rf_rd(operand2);
	else
	{
		if (sim_ptr->get_RAT(operand2)->isReady()) return_address = sim_ptr->get_RAT(operand2)->get_result();
		else
		{
			valid = false;
			p = sim_ptr->get_RAT(operand2);
		}
	}

	sim_ptr->set_pc(return_address);
	sim_ptr->flush_iq();

	sim_ptr->fill_station(this);
	sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);
}

inline bool RET::execute()
{
	if (!valid) return_address = p->get_result();
	cycles--;

	if (cycles == 0) {
		return true;
	}
	else
		return false;
}

inline void RET::writeback() 
{
	ready = 1;
}

inline void RET::commit() 
{
	cycles = 2;
	funcUnit = "JMP";
	valid = true;
	ready = 0;
}
#endif