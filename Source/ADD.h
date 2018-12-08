#ifndef ADD_H
#define ADD_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class ADD : public Inst3OP {
private:
	int parameter1, parameter2;
public:
	ADD();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline ADD::ADD() {
	cycles = 2;
	funcUnit = "ADD";
}

inline void ADD::issue()
{
	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

	// Need to check regRenamed first for these
	//parameter1 = sim_ptr->rf_rd(operand2);
	//parameter2 = sim_ptr->rf_rd(operand3);
}

inline bool ADD::execute()
{
		cycles--;

	if (cycles == 0) {
		result = parameter1 + parameter2;
		return true;
	}
	else
		return false;
	
}

inline void ADD::writeback()
{
	ready = 1;
	sim_ptr->edit_RAT(this);
}

inline void ADD::commit() 
{
	sim_ptr->rf_wr(operand1, result);
}

#endif