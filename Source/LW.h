#ifndef LW_H
#define LW_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class LW : public Inst3OP {
private:
	int parameter1, address, immediate, word;
	instruction* p1;
	bool valid;
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
	valid = true;
}

inline void LW::issue() 
{
	if (sim_ptr->get_RAT(operand2) == nullptr) parameter1 = sim_ptr->rf_rd(operand2);
	else
		parameter1 = sim_ptr->get_RAT(operand2)->get_result();
	
	address = parameter1 + immediate;
	result = address;
	immediate = operand3;

	sim_ptr->fill_station(this);
	sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);
	sim_ptr->fill_loadBuffer(this);
}

inline bool LW::execute()
{
	if (sim_ptr->CheckSWBuff(address))
		cycles--;

	if (cycles == 0) {
		word = sim_ptr->datamem_rd(address);
		return true;
	}
	else
		return false;
}

inline void LW::writeback()
{
	ready = 1;
}

inline void LW::commit()
{
	sim_ptr->rf_wr(operand1, word);
	if (sim_ptr->get_RAT(operand1) == this) sim_ptr->set_RAT(operand1, nullptr);
}
#endif