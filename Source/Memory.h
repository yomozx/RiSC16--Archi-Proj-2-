#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <sstream>
#include <iostream>
#include "Instruction.h"


using namespace std;


template<class Type>
class memory {
private:
    Type *arr;
    int Size;

public:
    memory(int size) : arr(new Type[size]), Size(size) {
        for (int i = 0; i < size; i++)
            arr[i] = 0;
    }

    ~memory() {
        delete[] arr;
    };

    Type readData(int address) {
        if (address < 0 || address >= Size)
            throw std::invalid_argument("Address being read from is not within memory range.");
        else
            return arr[address];
    };

    void storeData(int address, Type data) {
        if (address < 0 || address >= Size)
            throw std::invalid_argument("Address being written to is not within memory range.");
        else
            arr[address] = data;
        if (Size == 8)
            arr[0] = 0;
    };

    void Display() const {
        for (int i = 0; i < Size; i++) {
			if (Size == 8)
				cout << "Register " << dec <<  i << ": ";
            cout << arr[i] << endl;
        }
    };

};


// template specialization
template<>
class memory<instruction*> {
	friend class instruction;
	private:
		instruction * arr[64 * 1024];

	public:
		memory() {
			for (int i = 0; i < 64 * 1024; i++)
				arr[i] = NULL;
		}
		~memory() {
			for (int i = 0; i < 64 * 1024; i++)
				if (arr[i] != NULL)
					delete arr[i];
		}
		instruction* readData(int address) {
			if (arr[address] == NULL)
				return nullptr;
			else
				return arr[address];
		}

		void storeData(int address, instruction* inst)
		{
			arr[address] = inst;
		}

		void Display() const
		{
			for (int i = 0; i < 64 * 1024; i++)
				if (arr[i] != NULL) {
					cout << "\n0x" << hex << i << ": ";
					arr[i]->display();
				}
		}	
	};

#endif
