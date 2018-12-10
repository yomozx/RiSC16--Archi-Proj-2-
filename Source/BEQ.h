#ifndef BEQ_H
#define BEQ_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class BEQ : public Inst3OP {
private:
	int parameter1, parameter2, address, oldPC;
	bool taken;
public:
	BEQ();
	void issue();
	bool execute();
	void writeback();
	void commit();

};

inline BEQ::BEQ() {
	cycles = 1;
	funcUnit = "BEQ";
}

inline void BEQ::issue()
{
	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

	address = sim_ptr->get_pc() + 1 + operand3 + sim_ptr->get_startingAddr();

	// branch prediction
	if (operand3 < 0) {
		taken = 1;
		sim_ptr->set_pc(address);
	}
	else
		taken = 0;

	oldPC = sim_ptr->get_pc() + 1;

	// need to check regRenamed for this
	//parameter1 = sim_ptr->rf_rd(operand1);
	//parameter2 = sim_ptr->rf_rd(operand2);
}

inline bool BEQ::execute()
{
	cycles--;

	if (cycles == 0) {
		if (parameter1 == parameter2)
			if (taken)
				result = 1;
			else
				result = 0;
		else
			if (taken)
				result = 0;
			else
				result = 1;
		return true;
	}
	else
		return true;
	
}

inline void BEQ::writeback() 
{
	ready = 1;
}

inline void BEQ::commit() 
{
	if (!result)
		sim_ptr->set_pc(oldPC);
}
#endif