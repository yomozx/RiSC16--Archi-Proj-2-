cmake_minimum_required(VERSION 3.10)
project(RISC16--Archi-Proj-2-)

set(CMAKE_CXX_STANDARD 17)

add_executable(RISC16--Archi-Proj-2- Source/Main.cpp Source/ADD.h Source/ADDI.h Source/BEQ.h Source/Inst1OP.h
        Source/Inst1OP.cpp Source/Inst2OP.h Source/Inst2OP.cpp Source/Inst3OP.h Source/Inst3OP.cpp Source/Instruction.h
        Source/Instruction.cpp Source/JALR.h  Source/JMP.h Source/LW.h Source/Memory.h Source/MUL.h Source/NAND.h
        Source/RET.h Source/SIM.cpp Source/SIM.h Source/SUB.h Source/SW.h )