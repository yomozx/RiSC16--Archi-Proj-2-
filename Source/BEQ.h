#ifndef BEQ_H
#define BEQ_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class BEQ : public Inst3OP {
private:
	int parameter1, parameter2, address;
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

	// need to check regRenamed for this
	//parameter1 = sim_ptr->rf_rd(operand1);
	//parameter2 = sim_ptr->rf_rd(operand2);
}

inline bool BEQ::execute()
{
	cycles--;

	if (cycles == 0) {
		address = sim_ptr->get_pc() + 1 + operand3 + sim_ptr->get_startingAddr();
		if (parameter1 == parameter2)
			sim_ptr->set_pc(address);
		return true;
	}
	else
		return true;
	
}

inline void BEQ::writeback() 
{
	ready = 1;
}

inline void BEQ::commit() {}
#endif