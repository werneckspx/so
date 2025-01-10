#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <fstream>
#include <sstream>
#include "Core.hpp"
#include "Barramento.hpp"
#include <unordered_set>

class Escalonador {
private:
    vector<Core> cores;
    vector<unique_ptr<mutex>> core_mutexes; // Mutexes para cada core
    mutex output_mutex; // Mutex para proteger a saída padrão
    Barramento barramento;
    unordered_set<int> running_threads; // Conjunto de threads atualmente rodando// Mutexes para cada core
    mutex queue_mutex;

    struct CompareThreadCost {
        const vector<ThreadContext>& thread_contexts; // Referência para os contextos das threads

        CompareThreadCost(const vector<ThreadContext>& contexts) : thread_contexts(contexts) {}

        bool operator()(const int& thread_id1, const int& thread_id2) const {
            return thread_contexts[thread_id1].remaining_cost > thread_contexts[thread_id2].remaining_cost;
        }
    };

    priority_queue<int, vector<int>, CompareThreadCost> waiting_threads;

public:
    vector<int> threadCosts;
    vector<ThreadContext> thread_contexts;
    Escalonador(int num_cores, RAM& ram, Disco& disco,const vector<int>& instructionAddresses);
    int tempo_simulado = 0; 

    void atualizarTempo(int quantum) {
        tempo_simulado += quantum; // Incrementa o tempo simulado
    }
    string getTimestamp() {
        return "Tempo: " + std::to_string(tempo_simulado);
    }

    void run_thread(RAM& ram,int thread_id,const vector<int>& instructionAddresses);

};

#endif