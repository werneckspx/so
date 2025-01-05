#ifndef ULA_HPP
#define ULA_HPP

#include <iostream>
#include "Opcode.hpp"

using namespace std;

class ULA {
public:
    int exec(int operador_1, int operador_2, Opcode operando);
};

#endif
