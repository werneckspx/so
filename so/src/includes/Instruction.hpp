#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Opcode.hpp"
#include <ostream>

struct Instruction {
    Opcode op;
    int Destiny_Register;
    int Register_1;
    int Register_2;

    Instruction(Opcode opc, int rd, int r1, int r2);
    Instruction();

    friend std::ostream& operator<<(std::ostream& os, const Instruction& instr);
};

#endif
