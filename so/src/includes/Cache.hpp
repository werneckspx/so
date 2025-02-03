#ifndef CACHE_HPP
#define CACHE_HPP

#include "Opcode.hpp"
#include "InstructionDecode.hpp"
#include <unordered_map>
#include <queue>
#include <iostream>
#include <list>

using namespace std;

struct DecodedInstructionHash {
    size_t operator()(const DecodedInstruction& instr) const {
        return hash<int>()(instr.opcode) ^ hash<int>()(instr.destiny) ^
               hash<int>()(instr.value1) ^ hash<int>()(instr.value2);
    }
};

class Cache {
private:

    unordered_map<DecodedInstruction, list<pair<DecodedInstruction, int>>::iterator, DecodedInstructionHash> cache;
    list<pair<DecodedInstruction, int>> lruList;
    int capacidade;

public:

    Cache();
    bool in_cache(const DecodedInstruction& instr, int& resultado);
    void add_cache(const DecodedInstruction& instr, int resultado);
    void print_cache() const;

};

#endif