#ifndef SIM_H
#define SIM_H
#include "Memory.h"

using namespace std;

class instruction;

class SIM {
private:
	int pc;
	int last_read;
	int data_memCount;
	int starting_address;
	memory<__int16> data_memory;
	memory<__int16> registers;
	memory<instruction*> inst_memory;

public:
	SIM();
	~SIM();
	void simulate();
	void read_file();
	void read_instr();
	void read_data();
	void execute_instr();
	int toDec(string);
	int datamem_rd(int addr);
	int rf_rd(int addr);
	void datamem_wr(int addr, int data);
	void rf_wr(int addr, int data);
	void set_pc(int addr);
	int get_pc();
	void set_startingAddr(string address);
	int get_startingAddr();
	void displayMem();

	void fill_station(instruction* inst);
	void fill_regRenamed(instruction* inst);
	void edit_regRenamed(instruction* inst);
	void fill_ROB(instruction* inst);
	void fill_loadBuffer(instruction* inst);
};

#endif