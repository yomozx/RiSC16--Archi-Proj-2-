#ifndef RET_H
#define RET_H
#include "Inst1OP.h"
#include "Memory.h"
using namespace std;

class RET : public Inst1OP {
private:
	int return_address;
public:
	void issue();
	void execute();
	void writeback();
};

inline void RET::issue()
{
	return_address = sim_ptr->rf_rd(operand1);
}

inline void RET::execute()
{
	sim_ptr->set_pc(return_address);
}

inline void RET::writeback()
{

}
#endif