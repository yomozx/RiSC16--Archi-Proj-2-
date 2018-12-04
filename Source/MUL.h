#ifndef MUL_H
#define MUL_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class MUL : public Inst3OP {
private:
	int parameter1, parameter2, result;
public:
	void issue();
	void execute();
	void writeback();
};

inline void MUL::issue()
{
	parameter1 = sim_ptr->rf_rd(operand2);
	parameter2 = sim_ptr->rf_rd(operand3);
}

inline void MUL::execute()
{
	result = parameter1 * parameter2;
}

inline void MUL::writeback()
{
	sim_ptr->rf_wr(operand1, result);
}
#endif