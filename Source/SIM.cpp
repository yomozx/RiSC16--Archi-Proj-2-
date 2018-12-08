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
			 starting_address(0), RAT(8) {}

SIM::~SIM() {}

void SIM::simulate() {
	read_file();
	execute_instr();
	cout << "Program finished executing instructions" << endl;
}

void SIM::read_file() {
	ifstream file1;
	string filename; 
	cout << "Please enter file name containing instructions: ";
	cin >> filename;
	file1.open(filename.c_str());
	string ReadLine = "";
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
				instruction* inst;
				string name;
				stringstream str(ReadLine);
				getline(str, name, ' ');

				if (name != "") {
					if (name == "ADD")
						inst_memory.storeData(last_read, inst = new ADD);
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
						throw invalid_argument("instruction at index " + to_string(last_read) + " name is not supported.");
					inst->set_instruction(ReadLine, last_read);
					last_read += 1;
				}
			}

			if (text == false && data == true && ReadLine != ".data") 
			{
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

void SIM::execute_instr() {
	while (inst_memory.readData(pc) != NULL)
	{
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
			decimal_value += (hex[i] - 48)*base;
			base *= 16;
		}

		if (hex[i] >= 'A' && hex[i] <= 'F') {
			decimal_value += (hex[i] - 55) *base;
			base *= 16;
		}
	}
	return decimal_value;
}

int SIM::datamem_rd(int addr)
{
	return data_memory.readData(addr);
}

int SIM::rf_rd(int addr)
{
	return registers.readData(addr);
}

void SIM::datamem_wr(int addr, int data)
{
	data_memory.storeData(addr, data);
}

void SIM::rf_wr(int addr, int data)
{
	registers.storeData(addr, data);
}

void SIM::set_pc(int addr)
{
	pc = addr;
}

int SIM::get_pc()
{
	return pc;
}

void SIM::set_startingAddr(string address)
{
	if (toDec(address) < 0 || toDec(address) >= (64 * 1024))
		throw invalid_argument("Starting Address is not within memory range.");
	last_read = toDec(address);
	starting_address = toDec(address);
}

int SIM::get_startingAddr()
{
	return starting_address;
}

void SIM::displayMem()
{
	inst_memory.Display();
}

void SIM::RAT_validate(int addr)
{
	valid_bits[addr] = true;
}

void SIM::RAT_invalidate(int addr)
{
	valid_bits[addr] = false;
}

void SIM::fill_station(instruction * inst)
{
	// fill
}

void SIM::fill_regRenamed(instruction * inst)
{
	// fill
}

void SIM::edit_regRenamed(instruction * inst)
{
	// fill
}

void SIM::fill_ROB(instruction * inst)
{
	// fill
}

void SIM::fill_loadBuffer(instruction * inst)
{
	// fill
}

void SIM::read_instr()
{
	bool done = false;
	std::cout << "\n0. Enter Instruction" << endl << "1. Exit" << endl;
	std::cin >> done;

	while (!done && last_read != (64 * 1024))
	{
		std::string name;
		std::string whole_inst;
		instruction* inst;
		std::cout << "Instruction: ";

		cin.ignore();
		getline(cin, whole_inst);
		stringstream str(whole_inst);
		getline(str, name, ' ');

		if (name == "ADD")
			inst_memory.storeData(last_read, inst = new ADD);
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
		last_read += 1;

		std::cout << "\n0. Enter Instruction" << endl << "1. Exit" << endl;
		std::cin >> done;
	}
}

void SIM::read_data()
{
	bool done = false;
	int data;
	std::string address_hex;

	cout << "\n0. Enter Data" << endl << "1. Done" << endl;
	std::cin >> done;

	while (!done)
	{
		std::cout << "please enter the address at which you want to add a data entry (0x0000-0x7FFF)" << endl;
		std::cin >> address_hex;

		std::cout << "please enter data to be entered (decimal, integer value)" << endl;
		std::cin >> data;
		while (cin.fail() || data > 32767 || data < -32768)
		{
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



