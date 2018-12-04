#include <iostream>
#include "SIM.h"
#include <string>
using namespace std;

int main() {
	try {
		SIM simulator;
		bool manual;
		string starting_address = "0x0";
		cout << "Enter the starting address (in hex): ";
		cin >> starting_address;
		simulator.set_startingAddr(starting_address);
		std::cout << "choose mode of entry for program:" << endl << "0. text file" << endl << "1. manual" << endl;
		cin >> manual;
		if (manual)
		{
			simulator.read_instr();
			simulator.read_data();
		}
		else
		{
			simulator.read_file();
		}
		simulator.displayMem();
	}
	catch (const std::invalid_argument& e) {
		cout << e.what() << endl;
	}
	catch (string& s) {
		cout << s << endl;
	}


	system("pause");
	return 0;
}