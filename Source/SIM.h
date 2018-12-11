#ifndef SIM_H
#define SIM_H
#include "Memory.h"
#include <queue>

using namespace std;

class instruction;

class SIM {
private:
	int pc;
	int last_read;
	int data_memCount;
	int starting_address;
	int num_cycles;
	int instr_commits;
	int branches;
	int branch_misses;
	bool stall = false;
	memory<int> data_memory;
	memory<int> registers;
	bool valid_bits[8]; //array of validity bits for RAT
	memory<instruction*> RAT; //register alias table (for renaming); points to most up to date source of data (rf or rob).
	memory<instruction*> inst_memory;
	queue<instruction*> instq; //size is 4 instructions
	queue<instruction*> ROB;   //size is 6 instructions
	instruction* ADD_stations[3];
	instruction* BEQ_stations[2];
	instruction* LW_stations[2]; //load buffer
	instruction* SW_stations[2];
	instruction* JMP_stations[3];
	instruction* NAND_stations[1];
	instruction* MUL_stations[2];


public:
	SIM();
	~SIM();
	void simulate();
	void read_file();
	bool CheckSWBuff(int address, int ID);
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
	bool are_busy(string); //returns if all unit are busy
	bool dependent(instruction*, instruction*); //checks if 1st instruction's rd is used in second instruction
    bool valid (instruction *); //checks if all operand of instruction is valid
	bool rtype(instruction*);

	void RAT_validate(int addr);
	void RAT_invalidate(int addr);

	void fill_station(instruction* inst);
	void fill_RAT(instruction *inst);
	instruction* get_RAT(int addr);
	void set_RAT(int addr, instruction*);
	void fill_ROB(instruction* inst);
	void fill_loadBuffer(instruction* inst);
};

#endif