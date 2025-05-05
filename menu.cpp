#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "GameElements.cpp"
using namespace std;

//função que limpa a tela do console
void ClearConsole() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(out, pos);
}

// função que mostra para o jogardo como jogar
void instructiongame(){
    ClearConsole();
    cout << "\033c";
    cout << ">> Como Jogar <<\n\n1 Movimento e Controles:\nUse as teclas 'W''A''S''D' para mover seu personagem. Use o 'A' e 'D' para mover o cursor, e 'Enter' para selecionar a acao.\n";
    cout << "\n2- Acoes de Batalha: \nAo iniciar uma batalha, voce tera 4 opcoes: 'Atacar' (gera um ataque no inimigo), 'Defender' (defende do inimigo), 'Fugir' (sai do combate), 'Itens' (abre inventario, para utilizar algum item coletado).\n";
    cout << "\n3- Mecanica da Morte Permanente: \nRoguelike segue o sistema de Permadeath , o que significa que, se seu personagem morrer, voce tera que recomecar a partida desde o inicio.\n";
    cout << "\nPressione 'Enter' para voltar ao Menu.";
    cin.get();
    cout << "\033c"; // Sai do jogo e volta pro menu.
}

// função que mostra os itens para o jogador
void Itens(){
    cout << "\033c";
    cout << ">>>>> ITENS <<<<<\n\n";
    for (int i=0;i<4;i++){
    cout << itens[i].name << ": " << itens[i].description << endl << endl;
    }
    cout << "pressione Enter para voltar ao Menu.";
    cin.get();
    cout << "\033c";
 }

const int N_OPCOES = 4, N_DIFFICULTY = 4;
int selectedMenuItem = 0, MenuItem = 0, Menu = 0, difficulty = 0;
string Opcoes[N_OPCOES] = {"  Jogar   ", "Como Jogar", "  Itens   ", "  Sair    "};
string OpcoesDifficulty[N_DIFFICULTY] = {"  Easy   ", "  Medium ", "  Hard   ", "  Voltar "};

//renderiza o menu, limpando e reescrevendo com "> ".
void RenderMenu(int MenuItem){
    _kbhit();
    ClearConsole();
    cout << "= " + titleGame + " =\n";
    cout << "|================|\n";
    for (int i = 0; i < N_OPCOES; i++){
            if (MenuItem == i){
            cout << "|>> " << Opcoes[i] << " <<|" << endl;
            }else{
            cout << "|   " << Opcoes[i] << "   |" << endl;
        }   cout << "|================|\n";
    }
}

void RenderDifficult(int difficulty){
    _kbhit();
    ClearConsole();
    cout << "= DIFICULDADE DO JOGO =\n";
    cout << "|================|\n";
    for (int i = 0; i < N_DIFFICULTY; i++){
            if (difficulty == i){
            cout << "|>> " << OpcoesDifficulty[i] << " <<|" << endl;
            }else{
            cout << "|   " << OpcoesDifficulty[i] << "   |" << endl;
        }   cout << "|================|\n";
    }
}

void Difficulty(){
    bool exit = false;
    while (!exit) {

        if (_kbhit()) {
            int key = _getch();
            switch (key) {
                case 72: case 'w': case 'W':
                    if (difficulty == 0){
                        difficulty = 3;
                    } else {
                        difficulty -= 1;
                    }
                    break;
                case 80: case 's': case 'S':
                    if (difficulty == N_DIFFICULTY-1){
                        difficulty = 0;
                    } else {
                        difficulty +=1;
                    }
                    break;
                case 13: case 32:
                    if (OpcoesDifficulty[difficulty] == "  Easy   ") {

                    } else if (OpcoesDifficulty[difficulty] == "  Medium ") {

                    } else if (OpcoesDifficulty[difficulty] == "  Hard   ") {
                    } else if (OpcoesDifficulty[difficulty] == "  Voltar ") {
                        cout << "Saindo...\n";
                        exit = true;
                    }
                    break;
            }
            RenderDifficult(difficulty);
        }
    }
}
