#ifndef MUL_H
#define MUL_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class MUL : public Inst3OP {
private:
	int parameter1, parameter2;
public:
	MUL();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline MUL::MUL() {
	cycles = 8;
	funcUnit = "MUL";
}

inline void MUL::issue()
{
	sim_ptr->fill_station(this);
	sim_ptr->fill_regRenamed(this);
	sim_ptr->fill_ROB(this);

	// need to check regRenamed for this
	//parameter1 = sim_ptr->rf_rd(operand2);
	//parameter2 = sim_ptr->rf_rd(operand3);
}

inline bool MUL::execute()
{
	cycles--;

	if (cycles == 0) {
		result = parameter1 * parameter2;
		return true;
	}
	else
		return false;
}

inline void MUL::writeback()
{
	ready = 1;
	sim_ptr->edit_regRenamed(this);
}

inline void MUL::commit()
{
	sim_ptr->rf_wr(operand1, result);
}
#endif