#ifndef PERIFERICOS_HPP
#define PERIFERICOS_HPP

#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class Perifericos {
    private:
        unordered_map<string, bool> perifericos = {
            {"teclado", false},
            {"mouse", false}
        };
    public:
        void estadoPeriferico(const string& nomePeriferico, bool estado);
        void eventoPeriferico(const string& nomePeriferico);
};

#endif
