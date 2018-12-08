#ifndef SUB_H
#define SUB_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class SUB : public Inst3OP {
private:
	int parameter1, parameter2;
public:
	SUB();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline SUB::SUB() {
	cycles = 2;
	funcUnit = "ADD";
}

inline void SUB::issue()
{
	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

	// need to check regRenamed for this
	//parameter1 = sim_ptr->rf_rd(operand2);
	//parameter2 = sim_ptr->rf_rd(operand3);
}

inline bool SUB::execute()
{
	cycles--;

	if (cycles == 0) {
		result = parameter1 - parameter2;
		return true;
	}
	else
		return false;
}

inline void SUB::writeback()
{
	ready = 1;
	sim_ptr->edit_RAT(this);
}

inline void SUB::commit()
{
	sim_ptr->rf_wr(operand1, result);
}
#endif