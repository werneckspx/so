#include "../includes/InstructionDecode.hpp"

DecodedInstruction InstructionDecode(const Instruction& instr, const Registers& regs) {
    DecodedInstruction decoded;
    decoded.opcode = instr.op;
    decoded.destiny = instr.Destiny_Register;
    decoded.value1 = regs.get(instr.Register_1);
    decoded.value2 = regs.get(instr.Register_2);
    return decoded;
}

bool operator==(const DecodedInstruction& lhs, const DecodedInstruction& rhs) {
    return lhs.opcode == rhs.opcode &&
           lhs.destiny == rhs.destiny &&
           lhs.value1 == rhs.value1 &&
           lhs.value2 == rhs.value2;
}