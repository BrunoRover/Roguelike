#include <fstream>
#include <iostream>

using namespace std;

void salvarRelatorio(int pontuacao, int vida, int itens, int maxItens, int chaves, int minutos, int segundos) {
    ofstream arquivo("relatorio_partida.txt", ios::app); 
    if (arquivo.is_open()) {
        arquivo << "--- Relatorio de Partida ---\n";
        arquivo << "Pontuacao: " << pontuacao << "\n";
        arquivo << "Vida Final: " << vida << "\n";
        arquivo << "Itens Coletados: " << itens << "/" << maxItens << "\n";
        arquivo << "Chaves Coletadas: " << chaves << "\n";
        arquivo << "Tempo de Jogo: " << minutos << ":" << (segundos < 10 ? "0" : "") << segundos << " minutos\n";
        arquivo << "-----------------------------\n\n";
        arquivo.close();
        cout << "Relatorio salvo em 'relatorio_partida.txt'\n";
    } else {
        cout << "Erro ao salvar o relatorio.\n";
    }
}

void mostrarRelatorio() {
    ifstream arquivo("relatorio_partida.txt");
    if (arquivo.is_open()) {
        string linha;
        cout << "\n=== Historico de Partidas ===\n\n";
        while (getline(arquivo, linha)) {
            cout << linha << endl;
        }
        cout << "\n=============================\n";
        arquivo.close();
    } else {
        cout << "Nenhum relatorio encontrado.\n";
    }
}
