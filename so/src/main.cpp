#include "includes/Opcode.hpp"
#include "includes/Instruction.hpp"
#include "includes/Registers.hpp"
#include "includes/ULA.hpp"
#include "includes/UnidadeControle.hpp"
#include "includes/RAM.hpp"
#include "includes/InstructionDecode.hpp"
#include "includes/Pipeline.hpp"
#include "includes/Core.hpp"
#include "includes/perifericos.hpp"
#include "includes/Escalonador.hpp"
#include <numeric> 
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <iostream>
#include <mutex>
#include <filesystem>
#include <map>
#define clusterizacao_ativa false

using namespace std;
namespace fs = filesystem;

map<Opcode, int> operationWeights = {
    {ADD, 7}, {SUB, 7}, {AND, 7}, {OR, 7},
    {STORE, 8}, {LOAD, 7}, {MULT, 8}, {DIV, 9},
    {IF_igual, 7}, {ENQ, 10} // Exemplo, ajuste o peso do ENQ conforme o custo por iteração
};

void thread_function(RAM& ram, vector<int>& instructionAddresses, int thread_id, int&instructionAdress,vector<int>& threadCosts);
int loadInstructionsFromFile(RAM& ram, const string& instrFilename, int& instructionAdress,int& totalCost);
void threads_function_escalonador(Escalonador& escalonador, RAM& ram, vector<int>& instructionAddresses, int thread_id,vector<int>& threadCosts);

vector<vector<string>> findClusters(const map<string, unordered_set<string>>& fileInstructionsMap) {
    map<string, vector<pair<string, int>>> similarityGraph;
    
    vector<string> filenames;
    for (const auto& [filename, _] : fileInstructionsMap) {
        filenames.push_back(filename);
    }

    int numFiles = filenames.size();

    // Criar um grafo de similaridade baseado no número de instruções iguais
    for (int i = 0; i < numFiles; ++i) {
        for (int j = i + 1; j < numFiles; ++j) {
            const string& file1 = filenames[i];
            const string& file2 = filenames[j];

            int commonCount = 0;
            for (const auto& instr : fileInstructionsMap.at(file1)) {
                if (fileInstructionsMap.at(file2).count(instr)) {
                    commonCount++;
                }
            }

            // Apenas adicionamos uma aresta no grafo se há pelo menos 2 similaridade
            if (commonCount > 1) {
                similarityGraph[file1].emplace_back(file2, commonCount);
                similarityGraph[file2].emplace_back(file1, commonCount);
            }
        }
    }

    vector<vector<string>> clusters;
    unordered_set<string> visited;

    function<void(const string&, vector<string>&)> dfs = [&](const string& file, vector<string>& cluster) {
        visited.insert(file);
        cluster.push_back(file);
        
        for (const auto& [neighbor, weight] : similarityGraph[file]) {
            if (!visited.count(neighbor)) {
                dfs(neighbor, cluster);
            }
        }
    };

    for (const auto& [file, _] : similarityGraph) {
        if (!visited.count(file)) {
            vector<string> cluster;
            dfs(file, cluster);
            clusters.push_back(cluster);
        }
    }

    return clusters;
}

map<string, vector<string>> fileInstructionsMap; 

int main() {
    auto start = chrono::high_resolution_clock::now();
    RAM ram;
    Disco disco;
    Cache cache;
    Perifericos periferico;

    periferico.estadoPeriferico("teclado", true);
    periferico.estadoPeriferico("mouse", true);
    
    string directory = "data/"; // Caminho para a pasta
    int fileCount = 0;

    try {
        if (fs::exists(directory) && fs::is_directory(directory)) {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    ++fileCount;
                }
            }
        } else {
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Erro: " << e.what() << endl;
    }

    const int num_cores = 2;
    const int num_threads = fileCount-1;
    int instructionAdress = 0;

    vector<int> instructionAddresses(num_threads, 0);
    vector<thread> threads;
    vector<thread> threads_escalonador;
    vector<int> threadCosts(num_threads, 0);
    vector<int> indices(num_threads);

    Barramento barramento(num_threads);
    
    // Carregar as instruções dos arquivos em paralelo
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, ref(ram), ref(instructionAddresses), i, ref(instructionAdress), ref(threadCosts));
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    for (auto& t : threads) {
        t.join();
    }

    map<string, unordered_set<string>> instructionSets;
    for (const auto& [file, instructions] : fileInstructionsMap) {
        instructionSets[file] = unordered_set<string>(instructions.begin(), instructions.end());
    }

    // Encontramos os grupos
    vector<vector<string>> clusters = findClusters(instructionSets);

    // Exibimos os clusters
    cout << "Grupos de arquivos similares:\n";
    for (const auto& cluster : clusters) {
        cout << "[ ";
        for (const auto& file : cluster) {
            cout << file << " ";
        }
        cout << "]\n";
    }

    vector<int> ordemExecucao;
    map<string, int> fileIndexMap;

    // Mapeamos cada arquivo para seu índice original
    for (int i = 0; i < instructionAddresses.size(); ++i) {
        string filename = "data/data" + to_string(i + 1) + ".txt";
        fileIndexMap[filename] = i;
    }

    // Adicionamos os arquivos na ordem dos clusters
    for (const auto& cluster : clusters) {
        for (const auto& file : cluster) {
            if (fileIndexMap.find(file) != fileIndexMap.end()) {
                ordemExecucao.push_back(fileIndexMap[file]);
            }
        }
    }

    Escalonador escalonador(num_cores, ram, disco, instructionAddresses,cache); 

    cout << endl ;
    if(clusterizacao_ativa){
        for (int thread_id : ordemExecucao) {
            threads_escalonador.emplace_back(threads_function_escalonador, ref(escalonador), ref(ram), ref(instructionAddresses), thread_id, ref(threadCosts));
        }
    }else{
        iota(indices.begin(), indices.end(), 0); 

        sort(indices.begin(), indices.end(), [&threadCosts](int a, int b) {
            return threadCosts[a] < threadCosts[b];
        });
        
        for (int i : indices) {
            escalonador.thread_contexts[i].remaining_cost = threadCosts[i];
            cout << "Iniciando thread " << i << " com custo " << threadCosts[i] << endl;
        }
        for (int i : indices) {
            threads_escalonador.emplace_back(threads_function_escalonador, ref(escalonador), ref(ram), ref(instructionAddresses), i, ref(threadCosts));
        }
    }

    for (auto& t : threads_escalonador) {
        t.join();
    }
    
    ram.display();
    /*for (int i = 0; i < instructionAddresses.size(); i++)
    {
        cout << "instructinoaisjdoia; " << instructionAddresses[i] << endl;
    }*/
       auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;

    cout << "Tempo de execução: " << elapsed.count() << " segundos" << endl;


    return 0;
}

void threads_function_escalonador(Escalonador& escalonador, RAM& ram, vector<int>& instructionAddresses, int thread_id, vector<int>& threadCosts){
    escalonador.run_thread(ram,thread_id,instructionAddresses);
}

void thread_function(RAM& ram, vector<int>& instructionAddresses, int thread_id, int& instructionAdress, vector<int>& threadCosts) {
    string filename = "data/data" + to_string(thread_id + 1) + ".txt";
    cout << "Thread " << thread_id << " carregando o arquivo: " << filename << endl;

    int totalCost = 0;

    instructionAdress = loadInstructionsFromFile(ram, filename, instructionAdress, totalCost);
    if (instructionAdress == -1) {
        cerr << "Falha ao carregar instruções para thread " << thread_id << endl;
    } else {
        cout << "Thread " << thread_id << " carregou instruções até o endereço: " << instructionAdress << endl;
        instructionAddresses[thread_id] = instructionAdress;
        threadCosts[thread_id] = totalCost;
    }
}


int loadInstructionsFromFile(RAM& ram, const string& instrFilename, int& instructionAdress,int& totalCost) {
    ifstream file(instrFilename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de instruções: " << instrFilename << endl;
        return -1;
    }

    string line;

    while (getline(file, line)) {
        string opcodeStr;
        int rd, reg2, reg3;
        char virgula;

        stringstream ss(line);

        getline(ss, opcodeStr, ',');

        opcodeStr.erase(remove_if(opcodeStr.begin(), opcodeStr.end(), ::isspace), opcodeStr.end());

        ss >> rd >> virgula >> reg2 >> virgula >> reg3;

        Opcode opcode;
        if (opcodeStr == "ADD") opcode = ADD;
        else if (opcodeStr == "SUB") opcode = SUB;
        else if (opcodeStr == "AND") opcode = AND;
        else if (opcodeStr == "OR") opcode = OR;
        else if (opcodeStr == "STORE") opcode = STORE;
        else if (opcodeStr == "LOAD") opcode = LOAD;
        else if (opcodeStr == "ENQ") opcode = ENQ;
        else if (opcodeStr == "IF_igual") opcode = IF_igual;
        else {
            cerr << "Erro: Instrução inválida no arquivo: " << opcodeStr << endl;
            continue;
        }

        // Criar a string completa da instrução
        string instrString = opcodeStr + "," + to_string(rd) + "," + to_string(reg2) + "," + to_string(reg3);
        
        // Adicionar a instrução ao mapa auxiliar
        fileInstructionsMap[instrFilename].push_back(instrString);

        Instruction instr(opcode, rd, reg2, reg3);

        if (instructionAdress < ram.tamanho) {
            ram.instruction_memory[instructionAdress++] = instr;

            // Adiciona o peso da instrução ao custo total
            totalCost += operationWeights[opcode];
        } else {
            cerr << "Erro: memória RAM cheia, não é possível carregar mais instruções." << endl;
            break;
        }
    }
    file.close();
    return instructionAdress;
}
