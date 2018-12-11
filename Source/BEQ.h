#ifndef BEQ_H
#define BEQ_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class BEQ : public Inst3OP {
private:
	int parameter1, parameter2, address, oldPC;
	instruction* p1;
	instruction* p2;
	bool valid[2];
	bool taken;
	int mypc;
public:
	BEQ(SIM*);
	void issue();
	bool execute();
	void writeback();
	void commit();
	virtual bool ops_ready();
};

inline BEQ::BEQ(SIM* ptr) {
	cycles = 2;
	funcUnit = "BEQ";
	valid[0] = valid[1] = true;
	sim_ptr = ptr;
}

inline void BEQ::issue()
{
	if (sim_ptr->get_RAT(operand1) == nullptr) parameter1 = sim_ptr->rf_rd(operand1);
	else
	{
		if (sim_ptr->get_RAT(operand1)->isReady()) parameter1 = sim_ptr->get_RAT(operand1)->get_result();
		else
		{
			valid[0] = false;
			p1 = sim_ptr->get_RAT(operand1);
		}
	}

	if (sim_ptr->get_RAT(operand2) == nullptr) parameter2 = sim_ptr->rf_rd(operand2);
	else
	{
		if (sim_ptr->get_RAT(operand2)->isReady()) parameter2 = sim_ptr->get_RAT(operand2)->get_result();
		else
		{
			valid[1] = false;
			p2 = sim_ptr->get_RAT(operand2);
		}
	}

	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

	mypc = sim_ptr->get_startingAddr() + this->get_ID();

	address = mypc + 1 + operand3;

	// branch prediction
	if (operand3 < 0) {
		taken = 1;
		sim_ptr->set_pc(address);
		sim_ptr->flush_iq();
	}
	else
		taken = 0;

	oldPC = mypc + 1;

}

inline bool BEQ::execute()
{
	if (!valid[0]) parameter1 = p1->get_result();
	if (!valid[1]) parameter2 = p2->get_result();

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

inline bool BEQ::ops_ready()
{
	if ((p1 == nullptr || p1->isReady()) && (p2 == nullptr || p2->isReady())) return true;
	else return false;
}

#endif