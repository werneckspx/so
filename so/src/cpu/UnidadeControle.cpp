#include "../includes/UnidadeControle.hpp"

void UnidadeControle::executarInstrucao(Registers& regs, RAM& ram, int& relative_PC, int end_address, const string& regsFilename, 
    Disco& disco, int& Clock, int& instructions_executed, int& quantum_remaing) {
    pipeline.PipelineProcess(
            regs, 
            ram, 
            relative_PC, 
            end_address, // Passa o endereço final em vez do endereço de instrução
            "data/setRegisters.txt", 
            disco, 
            Clock,
            instructions_executed, 
            quantum_remaing
    );
}
