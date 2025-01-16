#include "../includes/Escalonador.hpp"

// Construtor da classe Escalonador
Escalonador::Escalonador(int num_cores, RAM& ram, Disco& disco, const vector<int>& instructionAddresses) 
    : barramento(instructionAddresses.size()) {
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
        barramento.waiting_threads.push(thread_id);
    }

    while (!barramento.all_threads_completed()) {
        int current_thread_id = -1;
        size_t core_index = -1;

        {
            unique_lock<mutex> lock(queue_mutex);
            if (!barramento.waiting_threads.empty()) {
                for (size_t i = 0; i < cores.size(); ++i) {
                    if (core_mutexes[i]->try_lock()) {
                        if (!cores[i].is_busy()) {
                            current_thread_id = barramento.waiting_threads.front();
                            barramento.waiting_threads.pop();
                            core_index = i;
                            running_threads.insert(current_thread_id);
                            lock.unlock();

                            // Declara a variável context aqui
                            ThreadContext& context = thread_contexts[current_thread_id];

                            // Mensagem com timestamp
                            cout << "[" << getTimestamp() << "] "
                                 << "Thread " << current_thread_id 
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

            // Atualiza o tempo simulado com o valor do quantum
            atualizarTempo(context.quantum);
            cout << endl;

            {
                unique_lock<mutex> lock(queue_mutex);
                running_threads.erase(current_thread_id);
                if (thread_completed) {
                    barramento.mark_thread_completed(current_thread_id);
                    // Mensagem com timestamp
                    cout << "[" << getTimestamp() << "] "
                         << "Thread " << current_thread_id << " marcada como concluída." << endl;
                } else {
                    barramento.waiting_threads.push(current_thread_id);
                }
            }

            core_mutexes[core_index]->unlock();
        } else {
            this_thread::yield();
        }
    }
}