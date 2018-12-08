#ifndef ADDI_H
#define ADDI_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class ADDI : public Inst3OP {

private:
	int parameter1, immediate;
public:
	ADDI();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline ADDI::ADDI() {
	cycles = 2;
	funcUnit = "ADD";
}

inline void ADDI::issue()
{
	sim_ptr->fill_station(this);
	sim_ptr->fill_regRenamed(this);
	sim_ptr->fill_ROB(this);

	// need to check regRenamed for this
	//parameter1 = sim_ptr->rf_rd(operand2);
	immediate = operand3;
}

inline bool ADDI::execute()
{
		cycles--;

	if (cycles == 0) {
		result = parameter1 + immediate;
		return true;
	}
	else
		return false;
	
}

inline void ADDI::writeback()
{
	ready = 1;
	sim_ptr->edit_regRenamed(this);
}

inline void ADDI::commit() 
{
	sim_ptr->rf_wr(operand1, result);
}
#endif