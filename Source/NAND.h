#ifndef NAND_H
#define NAND_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class NAND : public Inst3OP {
private:
	int parameter1, parameter2;
	instruction* p1;
	instruction* p2;
	bool valid[2];
public:
	NAND(SIM*);
	void issue();
	bool execute();
	void writeback();
	void commit();
	virtual bool ops_ready();
};

inline NAND::NAND(SIM* ptr) {
	cycles = 1;
	funcUnit = "NAND";
	valid[0] = valid[1] = true;
	sim_ptr = ptr;
}

inline void NAND::issue()
{
	if (sim_ptr->get_RAT(operand2) == nullptr) parameter1 = sim_ptr->rf_rd(operand2);
	else
	{
		if (sim_ptr->get_RAT(operand2)->isReady()) parameter1 = sim_ptr->get_RAT(operand2)->get_result();
		else
		{
			valid[0] = false;
			p1 = sim_ptr->get_RAT(operand2);
		}
	}

	if (sim_ptr->get_RAT(operand3) == nullptr) parameter2 = sim_ptr->rf_rd(operand3);
	else
	{
		if (sim_ptr->get_RAT(operand3)->isReady()) parameter2 = sim_ptr->get_RAT(operand3)->get_result();
		else
		{
			valid[1] = false;
			p2 = sim_ptr->get_RAT(operand3);
		}
	}

	sim_ptr->fill_station(this);
	sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);
}

inline bool NAND::execute()
{
	if (!valid[0]) parameter1 = p1->get_result();
	if (!valid[1]) parameter2 = p2->get_result();

	cycles--;

	if (cycles == 0) {
		result = ~(parameter1 & parameter2);
		return true;
	}
	else
		return false;
}

inline void NAND::writeback()
{
	ready = 1;
}

inline void NAND::commit()
{
	sim_ptr->rf_wr(operand1, result);
	if (sim_ptr->get_RAT(operand1) == this) sim_ptr->set_RAT(operand1, nullptr);
	cycles = 1;
	funcUnit = "NAND";
	valid[0] = valid[1] = true;
	ready = 0;
	finished_ex = 0;
}

inline bool NAND::ops_ready()
{
	if ((p1 == nullptr || p1->isReady()) && (p2 == nullptr || p2->isReady())) return true;
	else return false;
}

#endif