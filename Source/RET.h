#ifndef RET_H
#define RET_H
#include "Inst1OP.h"
#include "Memory.h"
using namespace std;

class RET : public Inst1OP {
private:
	int return_address;
public:
	RET();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline RET::RET() {
	cycles = 1;
	funcUnit = "JMP";
}

inline void RET::issue()
{
	sim_ptr->fill_station(this);
	sim_ptr->fill_regRenamed(this);
	sim_ptr->fill_ROB(this);

	// need to check regRenamed for this
	//return_address = sim_ptr->rf_rd(operand1);
}

inline bool RET::execute()
{
	cycles--;

	if (cycles == 0) {
		sim_ptr->set_pc(return_address);
		return true;
	}
	else
		return false;
}

inline void RET::writeback() 
{
	ready = 1;
}

inline void RET::commit() {}
#endif