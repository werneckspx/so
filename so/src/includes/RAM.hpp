#ifndef RAM_HPP
#define RAM_HPP

#include <iostream>
#include <vector>
#include "Instruction.hpp"
#include "InstructionDecode.hpp"

class RAM {
private:

public:
    vector<int> memoria;
    vector<Instruction> instruction_memory;
    static const int tamanho = 32;

    RAM();

    void write(int endereco, int valor);
    int read(int endereco);

    void writeInstruction(int endereco, const Instruction& instr);
    Instruction fetchInstruction(int endereco) const;

    void display() const;
    void displayI() const;
};

#endif
