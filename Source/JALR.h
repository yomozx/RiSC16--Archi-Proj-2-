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
public:
	JALR();
	void issue();
	bool execute();
	void writeback();
	void commit();
	virtual bool ops_ready() { return true; };
};

inline JALR::JALR() {
	cycles = 2;
	funcUnit = "JMP";
	valid = true;
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

	sim_ptr->fill_station(this);
	sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);
}

inline bool JALR::execute()
{
	if (!valid) parameter1 = p->get_result();

	cycles--;

	if (cycles == 0) {
		result = sim_ptr->get_pc() + 1;
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
	sim_ptr->set_pc(parameter1);
}
#endif