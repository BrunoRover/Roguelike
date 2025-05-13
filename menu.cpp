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
    cout << ">> Como Jogar <<\n\n1 Movimento e Controles:\nUse as teclas 'W''A''S''D' para mover seu personagem. \nUse o 'A' e 'D' para mover o cursor, e 'Enter' para selecionar a acao.\n";
    cout << "\n2- Acoes de Batalha: \nAo iniciar uma batalha, voce tera 4 opcoes: \n'Atacar' (gera um ataque no inimigo). \n'Defender' (defende do inimigo). \n'Fugir' (sai do combate). \n'Itens' (abre inventario, para utilizar algum item coletado).\n";
    cout << "\n3- Mecanica da Morte Permanente: \nRoguelike segue o sistema de Permadeath , o que significa que, se seu personagem morrer, \nvoce tera que recomecar a partida desde o inicio.\n";
    cout << "\n4- Sistema de Pontuacao: \n+5 pontos para cada inimigo eliminado. \n+1 ponto para cada item ou chave coletado.\n-1 pontos a cada 2 minutos de jogo corrido.\n";
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

const int N_OPCOES = 5, N_DIFFICULTY = 4;
int selectedMenuItem = 0, MenuItem = 0, Menu = 0;
string Opcoes[N_OPCOES] = {"   Jogar   ", "Como Jogar ","Dificuldade", "   Itens   ", "   Sair    "};
string OpcoesDifficulty[N_DIFFICULTY] = {"  Facil  ", "  Medio  ", "  Dificil", "  Voltar "};

//renderiza o menu, limpando e reescrevendo com "> ".
void RenderMenu(int MenuItem){
    _kbhit();
    ClearConsole();
    cout << "=== " + titleGame + " ===\n";
    cout << "|=================|\n";
    for (int i = 0; i < N_OPCOES; i++){
            if (MenuItem == i){
            cout << "|>> " << Opcoes[i] << " <<|" << endl;
            }else{
            cout << "|   " << Opcoes[i] << "   |" << endl;
        }   cout << "|=================|\n";
    }
}

void RenderDifficult(int difficulty){
    _kbhit();
    ClearConsole();
    cout << "= DIFICULDADE DO JOGO =\n";
    cout << "|================|\n";
    for (int i = 0; i < N_DIFFICULTY; i++){
            if (difficulty == i){
            cout << "|>> " << OpcoesDifficulty[i] << "  <<|" << endl;
            }else{
            cout << "|   " << OpcoesDifficulty[i] << "    |" << endl;
        }   cout << "|================|\n";
    }
}

void Difficulty(Player &player){
    bool exit = false;
    int bkDifficulty = difficulty;
    cout << "\033c";
    RenderDifficult(difficulty);

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
                    if (OpcoesDifficulty[difficulty] == "  Facil  "){
                        cout << "\nDificuldade Facil selecionada.\n\nPresssione 'Enter' para voltar ao Menu.";
                        cin.get();
                        cout << "\033[2J\033[H";
                        exit = true;
                    } else if (OpcoesDifficulty[difficulty] == "  Medio  ") {
                        cout << "\nDificuldade Medio selecionada.\n\nPresssione 'Enter' para voltar ao Menu.";
                        cin.get();
                        cout << "\033[2J\033[H";
                        exit = true;
                    } else if (OpcoesDifficulty[difficulty] == "  Dificil") {
                        cout << "\nDificuldade Dificil selecionada.\n\nPresssione 'Enter' para voltar ao Menu.";
                        cin.get();
                        cout << "\033[2J\033[H";
                        exit = true;
                    } else if (OpcoesDifficulty[difficulty] == "  Voltar ") {
                        difficulty = bkDifficulty;
                        cout << "Saindo...\n";
                        exit = true;
                        cout << "\033c";
                    }
                    break;
            }
            RenderDifficult(difficulty);
        }
    }
}
