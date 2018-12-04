#ifndef BEQ_H
#define BEQ_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class BEQ : public Inst3OP {
private:
	int parameter1, parameter2, address;
public:
	void issue();
	void execute();
	void writeback();
};

inline void BEQ::issue()
{
	parameter1 = sim_ptr->rf_rd(operand1);
	parameter2 = sim_ptr->rf_rd(operand2);
}

inline void BEQ::execute()
{
	address = sim_ptr->get_pc() + 1 + operand3;
	if (parameter1 == parameter2)
		sim_ptr->set_pc(address);
}

inline void BEQ::writeback()
{}
#endif