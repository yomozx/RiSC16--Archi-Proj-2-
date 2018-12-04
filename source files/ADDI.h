#ifndef ADDI_H
#define ADDI_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class ADDI : public Inst3OP {

private:
	int parameter1, immediate, result;
public:
	void issue();
	void execute();
	void writeback();
};

inline void ADDI::issue()
{
	parameter1 = sim_ptr->rf_rd(operand2);
	immediate = operand3;
}

inline void ADDI::execute()
{
	result = parameter1 + immediate;
}

inline void ADDI::writeback()
{
	sim_ptr->rf_wr(operand1, result);
}
#endif