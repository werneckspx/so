#include "../includes/Core.hpp"

// Construtor da classe Core, inicializa a RAM e o disco
Core::Core(RAM& ram, Disco& disco) : ram(ram), disco(disco), Clock(0),busy(false) {
    cout << "Core inicializado com RAM\n"; // Mensagem de inicialização
}

// Método que ativa o core com o contexto da thread
bool Core::activate_with_context(ThreadContext& context, RAM& ram, mutex& output_mutex) {
    lock_guard<mutex> lock(output_mutex); // Bloqueia o mutex para evitar condições de corrida

    int instructions_executed = 0;
    int quantum_remaing = context.quantum;

    cout << "Core ativado para Thread " << context.thread_id 
         << " com range: [" << context.start_address 
         << ", " << context.end_address << "]" << endl;

    // Reseta os registradores e outros estados necessários antes da execução
    regs = Registers(); // Reseta os registradores

    // Inicializa o contador de programa relativo ao endereço de início
    int relative_PC = context.start_address;

    // Executa apenas as instruções dentro do intervalo da thread
    while (relative_PC < context.end_address && quantum_remaing > 0) {
        try {
            uc.executarInstrucao(regs, ram, relative_PC, context.end_address,"data/setRegisters.txt", disco, Clock, instructions_executed, quantum_remaing);
            if (relative_PC >= context.end_address) {
                //cout << "PipelineProcess concluído: todas as instruções foram executadas." << endl;
                return true;
            } else if (quantum_remaing == 0) {
                //cout << "PipelineProcess encerrado: quantum restante atingiu zero." << endl;
                context.start_address = relative_PC;
                //quantum_remaing = context.quantum;
                return false;
            }          
        }
        catch (const exception& e) {
            cerr << "Erro na execução da Thread " << context.thread_id 
                << ": " << e.what() << endl; // Mensagem de erro
            return false;
        }
    }

    context.current_address = relative_PC; // Atualiza o progresso da thread
    context.execution_time += instructions_executed; // Incrementa o tempo de execução

    return relative_PC >= context.end_address;  
}

// Implementação do método is_busy() (Core.cpp)
bool Core::is_busy() const {
    // Retorne true se o core estiver ocupado, false caso contrário
    return busy; // Supondo que 'busy' seja um membro booleano da classe Core
}