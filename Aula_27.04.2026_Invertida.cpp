// Tabelas de Páginas Invertidas
// Simular a inserção de páginas (mapear Página Lógica para Frame Físico).
// Simular a busca de uma página lógica para encontrar o "Frame" na memória física (retornando o Frame ou um "Page Fault").

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// Representa uma entrada na Tabela de Páginas Invertida
struct IPTEntry {
    int pid;
    int logicalPageNumber;
    bool valid;
};

class InvertedPageTable {
private:
    vector<IPTEntry> table;
    // Hash map para buscar: {hash(PID, PaginaLogica) -> FrameIndex}
    unordered_map<long long, int> hashMap;

    long long generateKey(int pid, int page) {
        return ((long long)pid << 32) | page;
    }

    // Encontra um frame livre
    int getFreeFrame() {
        for (int i = 0; i < table.size(); i++) {
            if (!table[i].valid) {
                return i;
            }
        }
        return -1; // memória cheia
    }

public:
    InvertedPageTable(int numFrames) {
        // Inicializa com frames livres
        table.resize(numFrames);
        for (int i = 0; i < numFrames; i++) {
            table[i] = { -1, -1, false };
        }
    }

    // INSERÇÃO (mapear página → frame)
    void insert(int pid, int logicalPage) {
        long long key = generateKey(pid, logicalPage);

        if (hashMap.find(key) != hashMap.end()) {
            cout << "Pagina ja mapeada!\n";
            return;
        }

        int frame = getFreeFrame();

        if (frame == -1) {
            cout << "Memoria cheia! (sem frames livres)\n";
            return;
        }

        table[frame] = { pid, logicalPage, true };
        hashMap[key] = frame;

        cout << "Mapeado: PID " << pid
            << ", Pagina " << logicalPage
            << " -> Frame " << frame << endl;
    }

    // BUSCA (Tradução de endereço (Simulação))
    int translate(int pid, int logicalPage) {
        long long key = generateKey(pid, logicalPage);

        if (hashMap.find(key) != hashMap.end()) {
            return hashMap[key]; // Retorna o número do frame físico
        }

        return -1; // Page Fault
    }
};

int main() {
    InvertedPageTable ipt(5); // poucos frames pra forçar teste

    // Inserções
    ipt.insert(10, 5);
    ipt.insert(10, 6);
    ipt.insert(20, 1);

    // Busca válida
    int frame = ipt.translate(10, 5);
    if (frame != -1) {
        cout << "Encontrado no frame: " << frame << endl;
    }
    else {
        cout << "PAGE FAULT!\n";
    }

    // Busca inválida (não mapeada)
    frame = ipt.translate(10, 99);
    if (frame != -1) {
        cout << "Encontrado no frame: " << frame << endl;
    }
    else {
        cout << "PAGE FAULT!\n";
    }

    return 0;
}