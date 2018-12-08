#ifndef LW_H
#define LW_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class LW : public Inst3OP {
private:
	int parameter2, address, immediate;
public:
	LW();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline LW::LW() {
	cycles = 2;
	funcUnit = "LW";
}

inline void LW::issue() 
{
	sim_ptr->fill_station(this);
	sim_ptr->fill_regRenamed(this);
	sim_ptr->fill_ROB(this);
	sim_ptr->fill_loadBuffer(this);

	// need to check regRenamed for this
	//parameter2 = sim_ptr->rf_rd(operand2);
	immediate = operand3;
}

inline bool LW::execute()
{
	cycles--;

	if (cycles == 0) {
		address = parameter2 + immediate;
		result = sim_ptr->datamem_rd(address);
		return true;
	}
	else
		return false;
}

inline void LW::writeback()
{
	ready = 1;
	sim_ptr->edit_regRenamed(this);
}

inline void LW::commit()
{
	sim_ptr->rf_wr(operand1, result);
}
#endif