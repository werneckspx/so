#include "../includes/Barramento.hpp"

// Método que marca uma thread como concluída
void Barramento::mark_thread_completed(int thread_id) {
    unique_lock<mutex> lock(mtx); // Bloqueia o mutex
    thread_completed[thread_id] = true; // Marca a thread como concluída
    cout << "Thread " << thread_id << " marcada como concluída." << endl;
    cv.notify_all(); // Notifica todas as threads
}

// Método que verifica se todas as threads estão concluídas
bool Barramento::all_threads_completed() const {
    return all_of(thread_completed.begin(), thread_completed.end(), [](bool v) { return v; }); // Verifica se todas as threads estão concluídas
}