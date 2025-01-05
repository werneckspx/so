#ifndef INSTRUCTIONDECODE_HPP
#define INSTRUCTIONDECODE_HPP

#include "Instruction.hpp"
#include "Registers.hpp"

struct DecodedInstruction {
    int value1;
    int value2;
    int destiny;
    Opcode opcode;
};

DecodedInstruction InstructionDecode(const Instruction& instr, const Registers& regs);

#endif
