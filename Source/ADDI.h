#ifndef ADDI_H
#define ADDI_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class ADDI : public Inst3OP {

private:
	int parameter1, immediate;
	instruction* p;
	bool valid;
public:
	ADDI();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline ADDI::ADDI() {
	cycles = 2;
	funcUnit = "ADD";
	valid = true;
}

inline void ADDI::issue()
{
	if (sim_ptr->get_RAT(operand2) == nullptr) parameter1 = sim_ptr->rf_rd(operand2);
	else
	{
		if (sim_ptr->get_RAT(operand2)->isReady()) parameter1 = sim_ptr->get_RAT(operand2)->get_result();
		else
		{
			valid = false;
			p = sim_ptr->get_RAT(operand2);
		}
	}
	immediate = operand3;

	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);
}

inline bool ADDI::execute()
{
	if (!valid) parameter1 = p->get_result();

	cycles--;

	if (cycles == 0) {
		result = parameter1 + immediate;
		return true;
	}
	else
		return false;
	
}

inline void ADDI::writeback()
{
	ready = 1;
}

inline void ADDI::commit() 
{
	sim_ptr->rf_wr(operand1, result);
	if (sim_ptr->get_RAT(operand1) == this) sim_ptr->set_RAT(operand1, nullptr);
}
#endif