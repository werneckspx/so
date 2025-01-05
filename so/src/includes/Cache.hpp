#ifndef CACHE_HPP
#define CACHE_HPP

#include <unordered_map>
#include <queue>
#include <iostream>

using namespace std;

class Cache {
private:
    struct CacheLine {
        int data;
        bool dirty;  
    };

    unordered_map<int, CacheLine> cache;
    queue<int> fifoQueue;
    int capacidade;

public:
    Cache(int capacidade) : capacidade(capacidade) {}

    bool contains(int address) {
        return cache.find(address) != cache.end();
    }

    int get(int address) {
        if (contains(address)) {
            return cache[address].data;
        } else {
            cerr << "Erro: Endereço " << address << " não está no cache!" << endl;
            return -1;
        }
    }

    void set(int address, int value, bool writeBack = false) {
        if (cache.size() >= capacidade) {
            evict();
        }
        
        cache[address] = {value, writeBack};
        fifoQueue.push(address);
    }

    void evict() {
        if (fifoQueue.empty()) return;

        int oldAddress = fifoQueue.front();
        fifoQueue.pop();

        if (cache[oldAddress].dirty) {
            cout << "Escrevendo dado modificado de R" << oldAddress << " na memória principal (Write-back)." << endl;
        }
        
        cache.erase(oldAddress);
        cout << "Removendo R" << oldAddress << " do cache." << endl;
    }

    void markDirty(int address) {
        if (contains(address)) {
            cache[address].dirty = true;
        }
    }
};

#endif