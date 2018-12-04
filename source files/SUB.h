#ifndef SUB_H
#define SUB_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class SUB : public Inst3OP {
private:
	int parameter1, parameter2, result;
public:
	void issue();
	void execute();
	void writeback();
};

inline void SUB::issue()
{
	parameter1 = sim_ptr->rf_rd(operand2);
	parameter2 = sim_ptr->rf_rd(operand3);
}

inline void SUB::execute()
{
	result = parameter1 - parameter2;
}

inline void SUB::writeback()
{
	sim_ptr->rf_wr(operand1, result);
}
#endif