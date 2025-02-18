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
#include <bitset>

class Escalonador {
private:
    vector<Core> cores;
    vector<unique_ptr<mutex>> core_mutexes; // Mutexes para cada core
    mutex output_mutex; // Mutex para proteger a saída padrão
    Barramento barramento;
    unordered_set<int> running_threads; // Conjunto de threads atualmente rodando
    mutex queue_mutex;

    struct CompareThreadCost {
        const vector<ThreadContext>& thread_contexts; // Referência para os contextos das threads

        CompareThreadCost(const vector<ThreadContext>& contexts) : thread_contexts(contexts) {}

        bool operator()(const pair<string, int>& a, const pair<string, int>& b) const {
            return a.second > b.second; 
        }
    };

    priority_queue<pair<string, int>, vector<pair<string, int>>, CompareThreadCost> waiting_threads;

public:
    vector<int> threadCosts;
    vector<ThreadContext> thread_contexts;
    Escalonador(int num_cores, RAM& ram, Disco& disco, const vector<int>& instructionAddresses, Cache& cache);
    float tempo_simulado = 0; 
    float tempo_reduzido = 0;
    float cont_cache_hit = 0;
    int cont_mov = 0;

    void atualizarTempo(float quantum) {
        tempo_simulado += quantum; // Incrementa o tempo simulado
        tempo_reduzido = cont_cache_hit * 0.6;
        tempo_simulado = tempo_simulado - tempo_reduzido;
        cont_cache_hit = 0;
    }

    string getTimestamp() {
        return "Tempo: " + std::to_string(tempo_simulado);
    }

    void run_thread(RAM& ram, int thread_id, const vector<int>& instructionAddresses);
};

#endif