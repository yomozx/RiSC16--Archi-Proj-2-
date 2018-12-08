#ifndef JALR_H
#define JALR_H
#include "Inst2OP.h"
#include "Memory.h"
using namespace std;

class JALR : public Inst2OP {
private:
	int parameter2;
public:
	JALR();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline JALR::JALR() {
	cycles = 1;
	funcUnit = "JMP";
}

inline void JALR::issue()
{
	sim_ptr->fill_station(this);
	sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

	// need to check regRenamed for this
	//parameter2 = sim_ptr->rf_rd(operand2);
}

inline bool JALR::execute()
{
	cycles--;

	if (cycles == 0) {
		result = sim_ptr->get_pc() + 1;
		sim_ptr->set_pc(parameter2);
		return true;
	}
	else
		return false;
}

inline void JALR::writeback()
{
	ready = 1;
	sim_ptr->edit_RAT(this);
}

inline void JALR::commit()
{
	sim_ptr->rf_wr(operand1, result);
}
#endif