#include "../includes/RAM.hpp"

RAM::RAM() : memoria(tamanho, 0), instruction_memory(tamanho) {}

void RAM::write(int endereco, int valor) {
    if (endereco >= 0 && endereco < tamanho) {
        memoria[endereco] = valor;
    } else {
        std::cout << "Erro: Endereço fora da memória!" << std::endl;
    }
}

int RAM::read(int endereco) {
    if (endereco >= 0 && endereco < tamanho) {
        return memoria[endereco];
    } else {
        std::cout << "Erro: Endereço fora da memória!" << std::endl;
        return -1;
    }
}

void RAM::writeInstruction(int endereco, const Instruction& instr) {
    if (endereco >= 0 && endereco < tamanho) {
        instruction_memory[endereco] = instr;
    } else {
        std::cout << "Erro: Endereço inválido para instrução na RAM " << endereco << std::endl;
    }
}

Instruction RAM::fetchInstruction(int endereco) const {
    if (endereco >= 0 && endereco < tamanho) {
        return instruction_memory[endereco];
    }
    std::cout << "Erro: Endereço inválido para instrução na RAM " << endereco << std::endl;
    return Instruction(ADD, 0, 0, 0);
}

void RAM::display() const {
    for (int i = 0; i < tamanho; i++) {
        std::cout << "Endereço " << i << " -> " << memoria[i] << std::endl;
    }
}

void RAM::displayI() const {
    for (int i = 0; i < tamanho; i++) {
        std::cout << "Endereço " << i << " -> " << instruction_memory[i] << std::endl;
    }
}
