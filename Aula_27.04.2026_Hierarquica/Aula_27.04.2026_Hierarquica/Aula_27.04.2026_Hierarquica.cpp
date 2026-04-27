// Paginação Hierárquica (Multinível)
// Simular a inserção de páginas (mapear Página Lógica para Frame Físico).
// Simular a busca de uma página lógica para encontrar o "Frame" na memória física (retornando o Frame ou um "Page Fault").


#include <iostream>
#include <vector>

using namespace std;

// Tamanho da página/frame: 4KB (simplificado para o exemplo)
const int PAGE_SIZE = 1024;
const int ENTRIES_PER_TABLE = 1024;

// Estrutura de um Frame na Memória Física (RAM)
struct Frame {
    int frameNumber;
    bool valid;
};

// Entrada da tabela de páginas
struct PageTableEntry {
    int frameNumber;
    bool valid;
};

// Simulação de Memória
// Memória física
vector<Frame> physicalMemory;
// Tabela de páginas nível 2 (cada entrada do nível 1 aponta pra um vetor)
vector<PageTableEntry>* pageTableLevel2[ENTRIES_PER_TABLE];

// Inicialização
void initializeSystem() {
    physicalMemory.resize(100); // 100 frames

    for (int i = 0; i < 100; i++) {
        physicalMemory[i].frameNumber = i;
        physicalMemory[i].valid = false;
    }

    // nível 1 começa vazio
    for (int i = 0; i < ENTRIES_PER_TABLE; i++) {
        pageTableLevel2[i] = nullptr;
    }
}

// Encontra um frame livre
int getFreeFrame() {
    for (int i = 0; i < physicalMemory.size(); i++) {
        if (!physicalMemory[i].valid) {
            physicalMemory[i].valid = true;
            return i;
        }
    }
    return -1; // memória cheia
}

// INSERÇÃO (mapear página → frame)
void mapPage(int virtualAddress) {
    int p1 = (virtualAddress >> 22) & 0x3FF;
    int p2 = (virtualAddress >> 12) & 0x3FF;

    // Cria tabela nível 2 se não existir
    if (pageTableLevel2[p1] == nullptr) {
        pageTableLevel2[p1] = new vector<PageTableEntry>(ENTRIES_PER_TABLE);

        for (int i = 0; i < ENTRIES_PER_TABLE; i++) {
            (*pageTableLevel2[p1])[i].valid = false;
        }
    }

    PageTableEntry& entry = (*pageTableLevel2[p1])[p2];

    if (entry.valid) {
        cout << "Página já mapeada!\n";
        return;
    }

    int frame = getFreeFrame();

    if (frame == -1) {
        cout << "Memoria cheia! Nao ha frames livres.\n";
        return;
    }

    entry.frameNumber = frame;
    entry.valid = true;

    cout << "Mapeado: Pagina (" << p1 << "," << p2 << ") -> Frame " << frame << endl;
}

// BUSCA (tradução de endereço)
void accessPage(int virtualAddress) {
    int p1 = (virtualAddress >> 22) & 0x3FF;
    int p2 = (virtualAddress >> 12) & 0x3FF;
    int offset = virtualAddress & 0xFFF;

    cout << "\nAcessando endereco virtual: " << hex << virtualAddress << dec << endl;

    // Verifica nível 1
    if (pageTableLevel2[p1] == nullptr) {
        cout << "PAGE FAULT (nivel 1 inexistente)\n";
        return;
    }

    PageTableEntry& entry = (*pageTableLevel2[p1])[p2];

    // Verifica nível 2
    if (!entry.valid) {
        cout << "PAGE FAULT (pagina nao carregada)\n";
        return;
    }

    int frame = entry.frameNumber;
    int physicalAddress = frame * PAGE_SIZE + offset;

    cout << "Frame encontrado: " << frame << endl;
    cout << "Endereco fisico: " << physicalAddress << endl;
}

// MAIN
int main() {
    initializeSystem();

    cout << "Sistema de paginacao hierarquica simulado\n";

    int addr1 = 0x12345678;
    int addr2 = 0xABCDEF12;

    // Tenta acessar antes de mapear → PAGE FAULT
    accessPage(addr1);

    // Mapeia páginas
    mapPage(addr1);
    mapPage(addr2);

    // Acessa novamente → sucesso
    accessPage(addr1);
    accessPage(addr2);

    return 0;
}