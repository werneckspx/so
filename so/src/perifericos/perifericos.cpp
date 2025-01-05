#include "../includes/perifericos.hpp"

void Perifericos::estadoPeriferico(const string& nomePeriferico, bool estado) {
    if (perifericos.find(nomePeriferico) != perifericos.end()) {
        perifericos[nomePeriferico] = estado;
        cout << "O periférico " << nomePeriferico << " foi " << (estado ? "conectado" : "desconectado") << "." << endl;
    } else {
        cout << "Periférico " << nomePeriferico << " não encontrado." << endl;
    }
}

void Perifericos::eventoPeriferico(const string& nomePeriferico) {
    if (perifericos.find(nomePeriferico) != perifericos.end() && perifericos[nomePeriferico]) {
        cout << "Sinal enviado: O periférico " << nomePeriferico << " foi utilizado." << endl;
    } else {
        cout << "O periférico " << nomePeriferico << " não está ativo." << endl;
    }
}
