#ifndef UNIDADECONTROLE_HPP
#define UNIDADECONTROLE_HPP

#include <iostream>
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "RAM.hpp"
#include "InstructionDecode.hpp"
#include "Disco.hpp"
#include "Pipeline.hpp"

using namespace std;

class UnidadeControle {
private:
    Pipeline pipeline;

public:
    void executarInstrucao(Registers& regs, RAM& ram, int& relative_PC, int end_address, const string& regsFilename, Disco& disco, 
    int& Clock, int& instructions_executed, int& quantum_remaing);
};

#endif
