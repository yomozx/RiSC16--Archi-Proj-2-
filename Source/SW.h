#ifndef SW_H
#define SW_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class SW : public Inst3OP {
private:
	int address, data;
public:
	SW();
	void issue();
	bool execute();
	void writeback();
	void commit();
};

inline SW::SW() {
	cycles = 2;
	funcUnit = "SW";
}

inline void SW::issue()
{
	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

	address = operand3 + sim_ptr->rf_rd(operand2);
	result = address;

	// need to check regRenamed for this
	//data = sim_ptr->rf_rd(operand1);
}

inline bool SW::execute()
{
	cycles--;

	if (cycles == 0) {
		return true;
	}
	else
		return false;
}

inline void SW::writeback() 
{
	ready = 1;
}

inline void SW::commit() 
{
	sim_ptr->datamem_wr(address, data);
}
#endif