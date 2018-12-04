#ifndef JALR_H
#define JALR_H
#include "Inst2OP.h"
#include "Memory.h"
using namespace std;

class JALR : public Inst2OP {
private:
	int address, parameter2;
public:
	void issue();
	void execute();
	void writeback();
};

inline void JALR::issue()
{
	parameter2 = sim_ptr->rf_rd(operand2);
}

inline void JALR::execute()
{
	address = sim_ptr->get_pc() + 1;
	sim_ptr->set_pc(parameter2);
}

inline void JALR::writeback()
{
	sim_ptr->rf_wr(operand1, address);
}
#endif