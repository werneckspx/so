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
#include <unordered_set>
#include "Core.hpp"
#include "Barramento.hpp"

class Escalonador {
private:
    vector<Core> cores;
    vector<unique_ptr<mutex>> core_mutexes; // Mutexes para cada core
    mutex output_mutex; // Mutex para proteger a saída padrão
    Barramento barramento;
    vector<ThreadContext> thread_contexts;
    unordered_set<int> running_threads; // Conjunto de threads atualmente rodando// Mutexes para cada core
    mutex queue_mutex;

public:
    Escalonador(int num_cores, RAM& ram, Disco& disco,const vector<int>& instructionAddresses, Cache& cache);
    float tempo_simulado = 0; 
    float tempo_reduzido = 0;
    float cont_cache_hit = 0;
    
    void atualizarTempo(int quantum) {
        tempo_simulado += quantum; // Incrementa o tempo simulado
        tempo_reduzido = cont_cache_hit*0.6;
        tempo_simulado = tempo_simulado - tempo_reduzido;
        cont_cache_hit = 0;
    }
    string getTimestamp() {
        return "Tempo: " + to_string(tempo_simulado);
    }

    void run_thread(RAM& ram,int thread_id,const vector<int>& instructionAddresses);

};

#endif