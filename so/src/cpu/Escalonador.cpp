#include "../includes/Escalonador.hpp"

// Construtor da classe Escalonador
Escalonador::Escalonador(int num_cores, RAM& ram, Disco& disco, const vector<int>& instructionAddresses) 
    : barramento(instructionAddresses.size()),waiting_threads(CompareThreadCost(thread_contexts)) {
    // Inicializa os núcleos
    for (int i = 0; i < num_cores; ++i) {
        cores.emplace_back(ram, disco); // Adiciona um novo core
        core_mutexes.emplace_back(make_unique<mutex>()); // Adiciona um mutex para o core
    }

    // Cria os contextos das threads
    for (size_t i = 0; i < instructionAddresses.size(); ++i) {
        int start_addr = (i > 0) ? instructionAddresses[i-1] : 0; // Define o endereço de início
        int end_addr = instructionAddresses[i]; // Define o endereço final
        thread_contexts.emplace_back(start_addr, end_addr, i,2); // Cria o contexto da thread
    }
}

void Escalonador::run_thread(RAM& ram, int thread_id, const vector<int>& instructionAddresses) {
    {
        unique_lock<mutex> lock(queue_mutex);
        waiting_threads.push(thread_id); // Adiciona a thread à fila de prioridade
    }

    while (!barramento.all_threads_completed()) {
        int current_thread_id = -1;
        size_t core_index = -1;

        {
            unique_lock<mutex> lock(queue_mutex);
            if (!waiting_threads.empty()) {
                for (size_t i = 0; i < cores.size(); ++i) {
                    if (core_mutexes[i]->try_lock()) {
                        if (!cores[i].is_busy()) {
                            current_thread_id = waiting_threads.top(); // Pega a thread com menor custo
                            waiting_threads.pop(); // Remove a thread da fila
                            core_index = i;
                            running_threads.insert(current_thread_id);
                            lock.unlock();

                            ThreadContext& context = thread_contexts[current_thread_id];

                            cout << "[" << getTimestamp() << "] "
                                 << "Thread " << current_thread_id 
                                 << " custo atual " << context.remaining_cost
                                 << " usando Core " << core_index 
                                 << " com range: [" << context.start_address 
                                 << ", " << context.end_address << "]" << endl;

                            break;
                        } else {
                            core_mutexes[i]->unlock();
                        }
                    }
                }
            }
        }

        if (current_thread_id != -1 && core_index != -1) {
            ThreadContext& context = thread_contexts[current_thread_id];
            bool thread_completed = cores[core_index].activate_with_context(context, ram, output_mutex);

            atualizarTempo(context.quantum);

            {
                unique_lock<mutex> lock(queue_mutex);
                running_threads.erase(current_thread_id);
                if (thread_completed) {
                    barramento.mark_thread_completed(current_thread_id);
                    cout << "[" << getTimestamp() << "] "
                         << "Thread " << current_thread_id << " marcada como concluída." << endl;
                } else {
                    waiting_threads.push(current_thread_id); // Reinsere a thread na fila de prioridade
                }
            }

            core_mutexes[core_index]->unlock();
        } else {
            this_thread::yield();
        }
    }
}