#include "SIM.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Instruction.h"
#include "ADD.h"
#include "ADDI.h"
#include "BEQ.h"
#include "JALR.h"
#include "JMP.h"
#include "LW.h"
#include "MUL.h"
#include "NAND.h"
#include "RET.h"
#include "SUB.h"
#include "SW.h"

using namespace std;

SIM::SIM() : pc(0), last_read(0), data_memory(64 * 1024), registers(8), data_memCount(0),
             starting_address(0), instr_commits(0) {

    //initializing stations
    for (instruction *element:ADD_stations)
        element = nullptr;
    for (instruction *element:BEQ_stations)
        element = nullptr;
    for (instruction *element:LW_stations)
        element = nullptr;
    for (instruction *element:SW_stations)
        element = nullptr;
    for (instruction *element:JMP_stations)
        element = nullptr;
    for (instruction *element:NAND_stations)
        element = nullptr;
    for (instruction *element:MUL_stations)
        element = nullptr;
}

SIM::~SIM() {}

void SIM::simulate() {

	pc = starting_address;
	num_cycles = 0;
	instruction* ins1 = nullptr;
	instruction* ins2 = nullptr;
	bool loadstore = false;
	bool all_read = false;
	bool program_finished = false;

	while (!program_finished) {

		//Commit
		if (!ROB.empty()) {
			while (!ROB.empty() && ROB.front()->isReady()) {
				if (!(ROB.front()->get_name() == "BEQ")) { // if not branch
					ROB.front()->commit();
					instr_commits++;
					ROB.pop();
				}
				else { // if it's a branch
					branches++;
					if (!ROB.front()->get_result()) { // if we mispredicted
						ROB.front()->commit();
						branch_misses++;
						instr_commits++;
						while (!ROB.empty())
							ROB.pop();
					}
					else {//if it was correctly predicted
						ROB.front()->commit();
						instr_commits++;
						ROB.pop();
					}
				}
			}
		}

		//Execute & Writeback
		for (int i = 0; i < 3; i++)
			if (ADD_stations[i] != nullptr)
				if (valid(ADD_stations[i]))
					if (ADD_stations[i]->execute()) {
						ADD_stations[i]->writeback();
						ADD_stations[i] = nullptr; //freeing up the station
					}

		for (int i = 0; i < 2; i++)
			if (BEQ_stations[i] != nullptr)
				if (valid(BEQ_stations[i]))
					if (BEQ_stations[i]->execute()) {
						BEQ_stations[i]->writeback();
						BEQ_stations[i] = nullptr; //freeing up the station
					}
		for (instruction *element : LW_stations)
			if (element != nullptr)
				if (valid(element))
					if (element->execute()) {
						element->writeback();
						element = nullptr; //freeing up the station
					}
		for (int i = 0; i < 2; i++)
			if (SW_stations[i] != nullptr)
				if (valid(SW_stations[i]))
					if (SW_stations[i]->execute()) {
						SW_stations[i]->writeback();
						SW_stations[i] = nullptr; //freeing up the station
					}
		for (int i = 0; i < 3; i++)
			if (JMP_stations[i] != nullptr)
				if (valid(JMP_stations[i]))
					if (JMP_stations[i]->execute()) {
						JMP_stations[i]->writeback();
						JMP_stations[i] = nullptr; //freeing up the station
					}
		for (int i = 0; i < 1; i++)
			if (NAND_stations[i] != nullptr)
				if (valid(NAND_stations[i]))
					if (NAND_stations[i]->execute()) {
						NAND_stations[i]->writeback();
						NAND_stations[i] = nullptr; //freeing up the station
					}
		for (int i = 0; i < 2; i++)
			if (MUL_stations[i] != nullptr)
				if (valid(MUL_stations[i]))
					if (MUL_stations[i]->execute()) {
						MUL_stations[i]->writeback();
						MUL_stations[i] = nullptr; //freeing up the station
					}


		//Issue
		if (!instq.empty()) { //checking first instruction
			if ((instq.front()->get_name() == "LW" || instq.front()->get_name() == "SW") && !valid(instq.front())) //going to check load buffers
				loadstore = true;

			if(!loadstore && !are_busy(instq.front()->get_funcUnit()) && ROB.size() < 6)
			{
				ins1 = instq.front();
				instq.pop();
				ins1->issue();


				if (!instq.empty()) { // checking second instruction
					if ((instq.front()->get_name() == "LW" || instq.front()->get_name() == "SW") && !valid(instq.front())) //going to check load buffers
						loadstore = true;

					if (!loadstore && !are_busy(instq.front()->get_funcUnit()) && ROB.size() < 6) //if we have structural hazard or load/store hazard
					{
						if (!dependent(ins1, instq.front())) { //issuing two instructions if no dependancy
							ins2 = instq.front();
							instq.pop();
							ins2->issue();
						}
					}
				}
			}
		}


		//Fetch
		if (!all_read)
		{
			if (instq.size() == 4) //checking for capacity
				stall = true;
			else if(!stall)
			{
				instq.push(inst_memory.readData(pc));
				if (inst_memory.readData(pc + 1) != nullptr)
				{
					if (instq.size() == 4)
					{ //checking for capacity after first issue
						stall = true;
						pc++; //because we are stalling, we need to account for instruction that has already been pushed
					}
					else if (inst_memory.readData(pc)->get_name() != "JMP" && inst_memory.readData(pc)->get_name() != "JALR" && inst_memory.readData(pc)->get_name() != "RET")
						instq.push(inst_memory.readData(pc + 1));
				}
			}
			else stall = false; //resetting the value
		}

		pc = stall ? pc : pc + 2;
		if (pc >= last_read) all_read = true;
		num_cycles++;
		loadstore = false;
		program_finished = instq.empty() && ROB.empty() && all_read;
	}


	cout << "Program finished executing instructions" << endl << "\nResults:\n";
	registers.Display();
	cout << "Cycles Elapsed: " << num_cycles << endl;
	cout << "IPC: " << float(instr_commits) / num_cycles << endl;
	if (branches > 0) cout << "Branch Miss (%): " << (branch_misses / branches) * 100;
}

void SIM::read_file() {
    ifstream file1;
    string filename;
    cout << "Please enter file name containing instructions: ";
    cin >> filename;
    file1.open(filename.c_str());
    string ReadLine = "";
	int ID = 0;
    bool text = false, data = false;
    int address = 0;
    stringstream stream;
    string temp = "";

    if (!file1.fail())
        while (!file1.eof()) {
            getline(file1, ReadLine);

            if (ReadLine == ".text") {
                text = true;
                data = false;
            }

            if (ReadLine == ".data") {
                text = false;
                data = true;
            }

            if (text == true && data == false && ReadLine != ".text") {
                instruction *inst;
                string name;
                stringstream str(ReadLine);
                getline(str, name, ' ');

                if (name != "") {
                    if (name == "ADD")
                        inst_memory.storeData(last_read, inst = new ADD(this));
                    else if (name == "SUB")
                        inst_memory.storeData(last_read, inst = new SUB);
                    else if (name == "MUL")
                        inst_memory.storeData(last_read, inst = new MUL);
                    else if (name == "JMP")
                        inst_memory.storeData(last_read, inst = new JMP);
                    else if (name == "LW")
                        inst_memory.storeData(last_read, inst = new LW);
                    else if (name == "SW")
                        inst_memory.storeData(last_read, inst = new SW);
                    else if (name == "BEQ")
                        inst_memory.storeData(last_read, inst = new BEQ);
                    else if (name == "ADDI")
                        inst_memory.storeData(last_read, inst = new ADDI);
                    else if (name == "NAND")
                        inst_memory.storeData(last_read, inst = new NAND);
                    else if (name == "JALR")
                        inst_memory.storeData(last_read, inst = new JALR);
                    else if (name == "RET")
                        inst_memory.storeData(last_read, inst = new RET);
                    else
                        throw invalid_argument(
                                "instruction at index " + to_string(last_read) + " name is not supported.");
                    inst->set_instruction(ReadLine, last_read);
					inst->set_ID(ID);
					ID++;
                    last_read += 1;
                }
            }

            if (text == false && data == true && ReadLine != ".data") {
                if (ReadLine != "") {
                    stream.str(ReadLine);
                    getline(stream, temp, ':');
                    address = toDec(temp);
                    ReadLine.erase(0, ReadLine.find(':') + 1);
                    stream.str(ReadLine);
                    getline(stream, temp);

                    for (int i = 0; i < temp.size(); i++)
                        if (temp[i] != '\t' && !isdigit(temp[i]) && temp[i] != '-' && temp[i] != ' ')
                            throw invalid_argument("Data " + to_string(data_memCount) + \
                                " in the .data section is written incorrectly.");

                    data_memory.storeData(address, stoi(temp));
                    stream.clear();
                    data_memCount++;
                }
            }
        }
    else
        throw string("File could not be opened.");


    //data_memory.Display();

}

bool SIM::CheckSWBuff(int address, int ID)
{
	for (int i = 0; i < 2; i++)
		if (SW_stations[i] != nullptr && SW_stations[i]->get_result() == address && ID > SW_stations[i]->get_ID())
			return false;
	return true;
}

void SIM::execute_instr() {
    while (inst_memory.readData(pc) != nullptr) {
        inst_memory.readData(pc)->execute();
        pc += 1;
    }
}

int SIM::toDec(string hex) {
    int base = 1;
    int decimal_value = 0;

    if (hex[0] != '0' || hex[1] != 'x')
        throw invalid_argument("Address " + to_string(data_memCount) + " is written incorrectly.");

    string temp = hex.substr(2, hex.size() - 1);
    for (int i = 0; i < temp.size(); i++)
        if (temp[i] != ':' && !(temp[i] >= '0' && temp[i] <= '9') && !(temp[i] >= 'A' && temp[i] <= 'F'))
            throw invalid_argument("Address " + to_string(data_memCount) + " is written incorrectly.");

    for (int i = hex.size() - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            decimal_value += (hex[i] - 48) * base;
            base *= 16;
        }

        if (hex[i] >= 'A' && hex[i] <= 'F') {
            decimal_value += (hex[i] - 55) * base;
            base *= 16;
        }
    }
    return decimal_value;
}

int SIM::datamem_rd(int addr) {
    return data_memory.readData(addr);
}

int SIM::rf_rd(int addr) {
    return registers.readData(addr);
}

void SIM::datamem_wr(int addr, int data) {
    data_memory.storeData(addr, data);
}

void SIM::rf_wr(int addr, int data) {
    registers.storeData(addr, data);
}

void SIM::set_pc(int addr) {
    pc = addr;
}

int SIM::get_pc() {
    return pc;
}

void SIM::set_startingAddr(string address) {
    if (toDec(address) < 0 || toDec(address) >= (64 * 1024))
        throw invalid_argument("Starting Address is not within memory range.");
    last_read = toDec(address);
    starting_address = toDec(address);
}

int SIM::get_startingAddr() {
    return starting_address;
}

void SIM::displayMem() {
    inst_memory.Display();
}

bool SIM::are_busy(string a) {

    if (a == "ADD") {
        for (instruction *element: ADD_stations)
            if (element == nullptr)
                return false;
    } else if (a == "BEQ") {
        for (instruction *element: BEQ_stations)
            if (element == nullptr)
                return false;

    } else if (a == "LW") {
        for (instruction *element: LW_stations)
            if (element == nullptr)
                return false;

    } else if (a == "JMP") {
        for (instruction *element: JMP_stations)
            if (element == nullptr)
                return false;

    } else if (a == "NAND") {
        for (instruction *element: NAND_stations)
            if (element == nullptr)
                return false;

    } else if (a == "MUL") {
        for (instruction *element: MUL_stations)
            if (element == nullptr)
                return false;

    } else if (a == "SW") {
        for (instruction *element: SW_stations)
            if (element == nullptr)
                return false;
    }
    return true;

}

bool SIM::dependent(instruction *one, instruction *two) {

    switch (two->number_operands()) {
        case 1:
            return false;
            break;
        case 2:
            if (one->get_operand1() == two->get_operand2())
                return true;
            break;
        case 3:
            if (one->get_operand1() == two->get_operand2() ||
                (one->get_operand1() == two->get_operand3()) && (two->get_name() != "LW" || two->get_name() != "SW"
                                                                 || two->get_name() != "ADDI" ||
                                                                 two->get_name() != "BEQ"))
                return true;
            break;
    }
    return false;

}

bool SIM::valid(instruction *inst) {

    if (inst->number_operands() == 1)
	    if (inst->get_name() == "JMP" || RAT.readData(inst->get_operand1()) == nullptr || (RAT.readData(inst->get_operand1())->isReady())) 
			return true;

    if (inst->number_operands() == 2)
        if (RAT.readData(inst->get_operand2()) == nullptr || RAT.readData(inst->get_operand2())->isReady())
            return true;
		else if ((RAT.readData(inst->get_operand2()) == inst) && inst->ops_ready()) return true;

	if (inst->number_operands() == 3 )
	{
		if (rtype(inst))
		{
			if ((RAT.readData(inst->get_operand2()) == nullptr && RAT.readData(inst->get_operand3()) == nullptr) ||
				RAT.readData(inst->get_operand2()) == nullptr && RAT.readData(inst->get_operand3())->isReady() ||
				RAT.readData(inst->get_operand2())->isReady() && RAT.readData(inst->get_operand3())->isReady() ||
				RAT.readData(inst->get_operand2())->isReady() && RAT.readData(inst->get_operand3()) == nullptr)
				return true;
			else if ((RAT.readData(inst->get_operand2()) == inst || RAT.readData(inst->get_operand3())) && inst->ops_ready()) return true;
		}
		else //if i-type
		{
			if (RAT.readData(inst->get_operand2()) == nullptr || RAT.readData(inst->get_operand2())->isReady()) return true;
			else  if ((RAT.readData(inst->get_operand2()) == inst) && inst->ops_ready()) return true;
		}
	}

    return false;
}

bool SIM::rtype(instruction* inst)
{
	return (inst->get_name() == "ADD" || inst->get_name() == "SUB" || inst->get_name() == "MUL" || inst->get_name() == "NAND");
}

void SIM::RAT_validate(int addr) {
    valid_bits[addr] = true;
}

void SIM::RAT_invalidate(int addr) {
    valid_bits[addr] = false;
}

void SIM::fill_station(instruction *inst) {

    if (inst->get_funcUnit() == "ADD") {
        for (int i = 0; i < 3;i++)
            if (ADD_stations[i] == nullptr) {
				ADD_stations[i] = inst;
                return;
            }

    } else if (inst->get_funcUnit() == "BEQ") {
        for (int i = 0; i < 2; i++)
            if (BEQ_stations[i] == nullptr) {
				BEQ_stations[i] = inst;
                return;
            }


    } else if (inst->get_funcUnit() == "LW") {
        for (int i = 0; i < 2; i++)
            if (LW_stations[i] == nullptr) {
				LW_stations[i] = inst;
                return;
            }


    } else if (inst->get_funcUnit() == "JMP") {
        for (int i = 0; i < 3; i++)
            if (JMP_stations[i] == nullptr) {
				JMP_stations[i] = inst;
                return;
            }


    } else if (inst->get_funcUnit() == "NAND") {
        for (int i = 0; i < 1; i++)
            if (NAND_stations[i] == nullptr) {
				NAND_stations[i] = inst;
                return;
            }


    } else if (inst->get_funcUnit() == "MUL") {
        for (int i = 0; i < 2; i++)
            if (MUL_stations[i] == nullptr) {
				MUL_stations[i] = inst;
                return;
            }


    } else if (inst->get_funcUnit() == "SW") {
        for (int i = 0; i < 2; i++)
            if (SW_stations[i] == nullptr) {
				SW_stations[i] = inst;
                return;
            }
    }

    throw invalid_argument("Error! No reservation station is free.");
}

void SIM::fill_RAT(instruction *inst) {

    RAT_invalidate(inst->get_operand1());
    RAT.storeData(inst->get_operand1(), inst);

}

instruction* SIM::get_RAT(int addr)
{
	return RAT.readData(addr);
}

void SIM::set_RAT(int addr, instruction *inst)
{
	RAT.storeData(addr, inst);
}


void SIM::fill_ROB(instruction *inst) {
    ROB.push(inst);
}

void SIM::fill_loadBuffer(instruction *inst) {
    // fill
}

/*
void SIM::reacquire_params(instruction *inst)
{
	if (inst->number_operands() == 1 && inst->get_name == "RET")
		if (RAT.readData(inst->get_operand1()) == nullptr) 
		else if (RAT.readData(inst->get_operand1())->isReady())

	if (inst->number_operands() == 2)
		if (RAT.readData(inst->get_operand2()) == nullptr || RAT.readData(inst->get_operand2())->isReady())

	if (inst->number_operands() == 3)
		if ((RAT.readData(inst->get_operand2()) == nullptr && RAT.readData(inst->get_operand3()) == nullptr) ||
			RAT.readData(inst->get_operand2())->isReady() && RAT.readData(inst->get_operand3())->isReady())
}
*/

void SIM::read_instr() {
    bool done = false;
    std::cout << "\n0. Enter Instruction" << endl << "1. Exit" << endl;
    std::cin >> done;
	int ID = 0;

    while (!done && last_read != (64 * 1024)) {
        std::string name;
        std::string whole_inst;
        instruction *inst;
        std::cout << "Instruction: ";

        cin.ignore();
        getline(cin, whole_inst);
        stringstream str(whole_inst);
        getline(str, name, ' ');

        if (name == "ADD")
            inst_memory.storeData(last_read, inst = new ADD(this));
        else if (name == "SUB")
            inst_memory.storeData(last_read, inst = new SUB);
        else if (name == "MUL")
            inst_memory.storeData(last_read, inst = new MUL);
        else if (name == "JMP")
            inst_memory.storeData(last_read, inst = new JMP);
        else if (name == "LW")
            inst_memory.storeData(last_read, inst = new LW);
        else if (name == "SW")
            inst_memory.storeData(last_read, inst = new SW);
        else if (name == "BEQ")
            inst_memory.storeData(last_read, inst = new BEQ);
        else if (name == "ADDI")
            inst_memory.storeData(last_read, inst = new ADDI);
        else if (name == "NAND")
            inst_memory.storeData(last_read, inst = new NAND);
        else if (name == "JALR")
            inst_memory.storeData(last_read, inst = new JALR);
        else if (name == "RET")
            inst_memory.storeData(last_read, inst = new RET);
        else
            throw invalid_argument("instruction at index " + to_string(last_read) + ": name is not supported.");

        inst->set_instruction(whole_inst, last_read);
		inst->set_ID(ID);
		ID++;
        last_read += 1;

        std::cout << "\n0. Enter Instruction" << endl << "1. Exit" << endl;
        std::cin >> done;
    }
}

void SIM::read_data() {
    bool done = false;
    int data;
    std::string address_hex;

    cout << "\n0. Enter Data" << endl << "1. Done" << endl;
    std::cin >> done;

    while (!done) {
        std::cout << "please enter the address at which you want to add a data entry (0x0000-0x7FFF)" << endl;
        std::cin >> address_hex;

        std::cout << "please enter data to be entered (decimal, integer value)" << endl;
        std::cin >> data;
        while (cin.fail() || data > 32767 || data < -32768) {
            if (std::cin.fail()) std::cout << "\nnot an integer in range, please re-enter \n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<int>::max(), '\n');
            std::cin >> data;
        }

        data_memory.storeData(toDec(address_hex), data);

        cout << "\n0. Enter Data" << endl << "1. Done" << endl;
        std::cin >> done;
    }
    //data_memory.Display();

}



