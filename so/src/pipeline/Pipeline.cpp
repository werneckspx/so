#include "../includes/Pipeline.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

Pipeline::Pipeline() {} // Construtor da classe Pipeline

// Método que processa o pipeline
void Pipeline::PipelineProcess(Registers& regs, RAM& ram, int& relative_PC, int end_address, const string& regsFilename, Disco& disco, 
    int& Clock, int& instructions_executed, int& quantum_remaing, Cache& cache, float& cont_cach_hit) {
    //cout << "Iniciando o processamento do pipeline para Thread com limite: " << end_address << endl;

    // Inicializa os registradores a partir do arquivo
    setRegistersFromFile(regs, regsFilename);

    // Valida o endereço da instrução
    if (end_address <= 0) {
        cerr << "Erro: Endereço de instrução inválido." << endl;
        return;
    }

    try {
        // Calcula o endereço de memória real
        int mem_address = relative_PC; // Usa relative_PC diretamente como índice da instrução

        // Busca a instrução
        Instruction instr = InstructionFetch(ram, mem_address);
        cout << "Buscando instrução na posição " << mem_address << endl;
        Clock++; // Incrementa o clock

        // Decodifica a instrução
        DecodedInstruction decodedInstr = InstructionDecode(instr, regs);
        int resultado=0;

        /*if (cache.in_cache(decodedInstr, resultado)) {
            cout << "[CACHE HIT]: Resultado: " << resultado << endl;
            cont_cach_hit = cont_cach_hit + 1;
            regs.set(decodedInstr.destiny, resultado);
        } else {*/
            Execute(decodedInstr, regs, ram, relative_PC, disco, Clock,resultado);
        /*    cache.add_cache(decodedInstr, resultado);
            cout << "[CACHE MISS]: Executando instrucao e armazenando na cache." << endl;
        }*/

        Clock++; // Incrementa o clock
        
        cout << "[ID]: "
             << "Opcode: " << decodedInstr.opcode
             << ", Destino: R" << decodedInstr.destiny
             << ", Operando 1: " << decodedInstr.value1
             << ", Operando 2: " << decodedInstr.value2 << endl;
        cache.print_cache();
        // Executa a instrução
        //Execute(decodedInstr, regs, ram, relative_PC, disco, Clock,resultado);
        
        // Move para a próxima instrução
        ++relative_PC; // Incrementa em 1, pois estamos usando o índice da instrução
        --quantum_remaing; // Decrementa o quantum restante
        ++instructions_executed;

        cout << "Clock: " << Clock << " | Quantum restante: " << quantum_remaing 
             << " | Instruções executadas: " << instructions_executed << endl;

    } catch (const std::exception& e) {
        cerr << "Erro durante a execução da instrução: " << e.what() << endl; 
    }

}

Instruction Pipeline::InstructionFetch(RAM& ram, int endereco) {
    if (endereco >= 0 && endereco < ram.tamanho) {
        return ram.instruction_memory[endereco];
    }
    cout << "Erro: Endereço inválido para instrução na RAM " << endereco << endl;
    return Instruction(ADD, 0, 0, 0);
}

void Pipeline::Wb(const DecodedInstruction& decoded, int& resultado, RAM& ram, Disco& disco, int& Clock) {
    ram.write(decoded.value1, resultado); 
    Clock++;
}

void Pipeline::MemoryAccess(const DecodedInstruction& decoded, int resultado, Registers& regs, int& Clock) {
    regs.set(decoded.destiny, resultado);
    Clock++;
}

void Pipeline::Execute(const DecodedInstruction& decoded, Registers& regs, RAM& ram, int& PC, Disco& disco, int& Clock, int& resultado) {
    switch (decoded.opcode) {
        case ADD: {
            resultado = ula.exec(decoded.value1, decoded.value2, ADD);
            Clock++;
            MemoryAccess(decoded, resultado, regs, Clock);
            
            cout << "ADD R" << decoded.destiny << " = " << decoded.value1 << " + " << decoded.value2 << " -> " << regs.get(decoded.destiny) << endl;
            break;
        }
        case SUB: {
            resultado = ula.exec(decoded.value1, decoded.value2, SUB);
            Clock++;
            MemoryAccess(decoded, resultado, regs, Clock);
            
            cout << "SUB R" << decoded.destiny << " = " << decoded.value1 << " - " << decoded.value2 << " -> " << regs.get(decoded.destiny) << endl;
            break;
        }
        case AND: {
            resultado = ula.exec(decoded.value1, decoded.value2, AND);
            Clock++;
            MemoryAccess(decoded, resultado, regs, Clock);
            
            cout << "AND R" << decoded.destiny << " = " << decoded.value1 << " & " << decoded.value2 << " -> " << regs.get(decoded.destiny) << endl;
            break;
        }
        case OR: {
            resultado = ula.exec(decoded.value1, decoded.value2, OR);
            Clock++;
            MemoryAccess(decoded, resultado, regs, Clock);
            
            cout << "OR R" << decoded.destiny << " = " << decoded.value1 << " | " << decoded.value2 << " -> " << regs.get(decoded.destiny) << endl;
            break;
        }
        case LOAD: {
            resultado = ram.read(decoded.value1);
            Clock++;
            MemoryAccess(decoded, resultado, regs, Clock);
            cout << "LOAD R" << decoded.destiny << " = RAM[" << decoded.value1 << "] -> " << regs.get(decoded.destiny) << endl;
            break;
        }
        case STORE: {
            resultado = regs.get(decoded.destiny);
            Clock++;
            Wb(decoded, resultado, ram, disco, Clock);
            cout << "STORE RAM[" << decoded.value1 << "] = R" << decoded.destiny << " -> " << resultado << endl;
            disco.write(resultado);
            //cout << "STORE DISK[" << valor << "]" << endl;
            break;
        }
        case MULT: {
            resultado = ula.exec(decoded.value1, decoded.value2, MULT);
            Clock++;
            MemoryAccess(decoded, resultado, regs, Clock);
            
            cout << "MULT R" << decoded.destiny << " = " << decoded.value1 << " * " << decoded.value2 << " -> " << regs.get(decoded.destiny) << endl;
            break;
        }
        case DIV: {
            if (decoded.value2 != 0) {
                resultado = ula.exec(decoded.value1, decoded.value2, DIV);
                Clock++;
                MemoryAccess(decoded, resultado, regs, Clock);
                
                cout << "DIV R" << decoded.destiny << " = " << decoded.value1 << " / " << decoded.value2 << " -> " << regs.get(decoded.destiny) << endl;
            } else {
                cerr << "Erro: Divisão por zero!" << endl;
            }
            break;
        }
        case IF_igual: {
            resultado = (decoded.value1 == decoded.value2) ? 1 : 0;
            Clock++;
            MemoryAccess(decoded, resultado, regs, Clock);
            
            cout << "IF_igual " << decoded.destiny << " = R" << decoded.value1 << " == R" << decoded.value2 << " -> " << regs.get(decoded.destiny) << endl;
            break;
        }
        case ENQ: {
            resultado = decoded.value1;
            cout << "OPERADOR 1: " << decoded.value1 << endl;
            cout << "OPERADOR 2: " << decoded.value2 << endl;
            
            while (resultado != decoded.value2) {
                if (resultado < decoded.value2) {
                    resultado = ula.exec(resultado, 1, ADD);
                    Clock++;
                    MemoryAccess(decoded, resultado, regs, Clock);
                } else {
                    resultado = ula.exec(resultado, 1, SUB);
                    Clock++;
                    MemoryAccess(decoded, resultado, regs, Clock);
                }
            }
            cout << "ENQ " << decoded.destiny << " = R" << decoded.value1 << " enquanto R" << decoded.value2 << " -> " << regs.get(decoded.destiny) << endl;
            break;
        }
        default:
            cerr << "Opcode desconhecido: " << decoded.opcode << endl;
    }
}

void Pipeline::setRegistersFromFile(Registers& regs, const string& regsFilename) {
    ifstream regsFile(regsFilename);
    if (!regsFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de registradores: " << regsFilename << endl;
        return;
    }

    string line;
    while (getline(regsFile, line)) {
        int regNum, regValue;
        char virgula;
        stringstream ss(line);

        ss >> regNum >> virgula >> regValue;
        regs.set(regNum, regValue); 
    }
    regsFile.close();
}