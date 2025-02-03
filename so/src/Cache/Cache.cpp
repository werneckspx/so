#include "../includes/Cache.hpp"

Cache::Cache() : capacidade(5) {}

bool Cache::in_cache(const DecodedInstruction& instr, int& resultado) {
    auto it = cache.find(instr);
    if (it != cache.end()) {
        // Move o elemento acessado para o início da lista
        lruList.splice(lruList.begin(), lruList, it->second);
        resultado = it->second->second; // Obtém o resultado armazenado
        return true; // Encontrado na cache
    }
    return false; // Não encontrado na cache
}

void Cache::add_cache(const DecodedInstruction& instr, int resultado) {
    if (cache.find(instr) != cache.end()) {
        auto it = cache[instr];
        it->second = resultado; // Atualiza o valor
        lruList.splice(lruList.begin(), lruList, it); // Move para o início
        return;
    }

    if (cache.size() >= capacidade) {
        auto last = lruList.back(); // Obtém o menos recentemente usado
        cache.erase(last.first);   // Remove do mapa
        lruList.pop_back();        // Remove da lista
    }

    // Adiciona a nova instrução e resultado
    lruList.emplace_front(instr, resultado);
    cache[instr] = lruList.begin();
}

void Cache::print_cache() const {
    cout << "Cache Content:" << endl;
    for (const auto& pair : lruList) {
        const DecodedInstruction& instr = pair.first;
        int resultado = pair.second;
        cout << "Opcode: " << instr.opcode << ", Destiny: " << instr.destiny
             << ", Value1: " << instr.value1 << ", Value2: " << instr.value2
             << " -> Result: " << resultado << endl;
    }
}
