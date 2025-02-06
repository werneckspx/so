#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Disco.hpp"
#include "Opcode.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "RAM.hpp"
#include "Cache.hpp"
#include "InstructionDecode.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>

class Pipeline
{
private:
    ULA ula;

public:
    Pipeline();
    void PipelineProcess(Registers& regs, RAM& ram, int& PC, int instructionAdress, const string& regsFilename, Disco& Disco, 
    int& Clock, int& instructions_executed, int& quantum_remaing,int& remaing_cost, Cache& cache, float& cont_cach_hit);
    Instruction InstructionFetch(RAM& ram, int endereco);
    void Wb(const DecodedInstruction& decoded, int& resultado, RAM& ram, Disco& disco, int& Clock);
    void MemoryAccess(const DecodedInstruction& decoded, int resultado, Registers& regs, int& Clock);
    void setRegistersFromFile(Registers& regs, const std::string& regsFilename);
    void Execute(const DecodedInstruction& decoded, Registers& regs, RAM& ram, int& PC, Disco& Disco, int& Clock, int& resultado);
};

#endif
