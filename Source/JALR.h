#ifndef JALR_H
#define JALR_H
#include "Inst2OP.h"
#include "Memory.h"
using namespace std;

class JALR : public Inst2OP {
private:
	int parameter1;
	instruction* p;
	bool valid;
	int mypc;
public:
	JALR(SIM*);
	void issue();
	bool execute();
	void writeback();
	void commit();
	virtual bool ops_ready() { return true; };
};

inline JALR::JALR(SIM* ptr) {
	cycles = 1;
	funcUnit = "JMP";
	valid = true;
	sim_ptr = ptr;
}

inline void JALR::issue()
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

	mypc = sim_ptr->get_startingAddr() + this->get_ID();
	result = mypc + 1;
	sim_ptr->set_pc(parameter1);
	sim_ptr->flush_iq();

	sim_ptr->fill_station(this);
	sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);
}

inline bool JALR::execute()
{
	cycles--;

	if (cycles == 0) {
		return true;
	}
	else
		return false;
}

inline void JALR::writeback()
{
	ready = 1;
}

inline void JALR::commit()
{
	sim_ptr->rf_wr(operand1, result);
	if (sim_ptr->get_RAT(operand1) == this) sim_ptr->set_RAT(operand1, nullptr);
	cycles = 1;
	funcUnit = "JMP";
	valid = true;
	ready = 0;
	finished_ex = 0;
}
#endif