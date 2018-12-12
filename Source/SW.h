#ifndef SW_H
#define SW_H
#include "Inst3OP.h"
#include "Memory.h"
using namespace std;

class SW : public Inst3OP {
private:
	int address;
	int parameter1, parameter2;
	instruction* p1;
	instruction* p2;
	bool valid[2];
public:
	SW(SIM*);
	void issue();
	bool execute();
	void writeback();
	void commit();
	virtual bool ops_ready() { return true; };
};

inline SW::SW(SIM* ptr) {
	cycles = 2;
	funcUnit = "SW";
	valid[0] = valid[1] = true;
	sim_ptr = ptr;
}

inline void SW::issue()
{
	if (sim_ptr->get_RAT(operand1) == nullptr) parameter1 = sim_ptr->rf_rd(operand1);
	else
		parameter1 = sim_ptr->get_RAT(operand1)->get_result();
	
	if (sim_ptr->get_RAT(operand2) == nullptr) parameter2 = sim_ptr->rf_rd(operand2);
	else
		parameter2 = sim_ptr->get_RAT(operand2)->get_result();
	
	address = operand3 + parameter2;
	result = address;

	sim_ptr->fill_station(this);
    sim_ptr->fill_RAT(this);
	sim_ptr->fill_ROB(this);

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
	sim_ptr->datamem_wr(address, parameter1);
	cycles = 2;
	funcUnit = "SW";
	valid[0] = valid[1] = true;
	ready = 0;
}
#endif