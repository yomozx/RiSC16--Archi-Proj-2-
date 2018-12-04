#ifndef JMP_H
#define JMP_H
#include "Inst1OP.h"
#include "Memory.h"
using namespace std;

class JMP : public Inst1OP {
private:
	int immediate;
public:
	void issue();
	void execute();
	void writeback();
};

inline void JMP::issue() 
{
	immediate = operand1;
}

inline void JMP::execute()
{
	int address = sim_ptr->get_pc() + 1 + immediate;
	sim_ptr->set_pc(address);
}

inline void JMP::writeback() {}
#endif