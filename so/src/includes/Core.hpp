#ifndef CORE_HPP
#define CORE_HPP

#include "Opcode.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include "ULA.hpp"
#include "UnidadeControle.hpp"
#include "RAM.hpp"
#include "InstructionDecode.hpp"
#include "Pipeline.hpp"
#include "Disco.hpp"
#include "Pipeline.hpp"
#include <thread>
#include <mutex>

struct ThreadContext {
    int start_address;   
    int end_address;     
    int current_pc;      
    int thread_id;
    int quantum;         // Quantum da thread
    int current_address; // Próxima instrução a ser executada
    int execution_time;  // Tempo total de execução da thread
    int priority;

    ThreadContext(int start, int end, int thread_id, int quantum, int priority) 
        : start_address(start), 
          end_address(end), 
          current_pc(0), 
          thread_id(thread_id),
          quantum(quantum), 
          current_address(start),
          execution_time(0),
          priority(priority) {}
};


class Core {
private:
    bool busy;
public:
    Registers regs;
    UnidadeControle uc;
    RAM& ram;
    Disco& disco;
    int Clock;

    Core(RAM& ram, Disco& disco);
    bool activate_with_context(ThreadContext& context, RAM& ram, mutex& output_mutex);
    bool is_busy() const;
};
#endif