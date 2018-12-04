#ifndef LW_H
#define LW_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class LW : public Inst3OP {
private:
	int parameter2, result, address, immediate;
public:
	void issue();
	void execute();
	void writeback();
};

inline void LW::issue() 
{
	parameter2 = sim_ptr->rf_rd(operand2);
	immediate = operand3;
}

inline void LW::execute()
{
	address = parameter2 + immediate;
	result = sim_ptr->datamem_rd(address);
}

inline void LW::writeback()
{
	sim_ptr->rf_wr(operand1, result);
}
#endif