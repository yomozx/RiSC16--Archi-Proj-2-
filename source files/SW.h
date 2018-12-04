#ifndef SW_H
#define SW_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class SW : public Inst3OP {
private:
	int address, data;
public:
	void issue();
	void execute();
	void writeback();
};

inline void SW::issue()
{
	address = operand3 + sim_ptr->rf_rd(operand2);
	data = sim_ptr->rf_rd(operand1);
}

inline void SW::execute()
{
	sim_ptr->datamem_wr(address, data);
}

inline void SW::writeback() {}
#endif