#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

void ClearConsole() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(out, pos);
}

void instructiongame(){
    ClearConsole();
    cout << "\033c";
    cout << ">> Como Jogar <<\n\n1 Movimento e Controles:\nUse as teclas 'W''A''S''D' para mover seu personagem. Use o 'A' e 'D' para mover o cursor, e 'Enter' para selecionar a ação.\n";
    cout << "\n2- Ações de Batalha: \nAo iníciar uma batalha, você terá 4 opções: 'Atacar' (gera um ataque no inimigo), 'Defender' (defende do inimigo), 'Fugir' (sai do combate), 'Itens' (abre inventário, para utilizar algum ítem coletado).\n";
    cout << "\n3- Mecânica da Morte Permanente: \nRoguelike segue o sistema de Permadeath , o que significa que, se seu personagem morrer, você terá que recomeçar a partida desde o início.\n";
    cout << "\nPressione 'Enter' para voltar ao Menu.";
    cin.get();
    cout << "\033c";
    cout << "  Jogar\n> Como Jogar\n  Itens\n  Sair\n";// Sai do jogo e volta pro menu.
}
 void Itens(){
    cout << "\033c";
    cout << ">> ITENS <<\n\n";
    cout << "1- Poção de Vida:\nAo consumir a poção, você recebe 2 Vidas.\n\n";
    cout << "2- Poção de Força:\nAo consumir  a poção, você recebe força, causando mais dano ao inimigo.\n\n";
    cout << "3- escudo:\nAo enfiar o escudo no cu, você recebe mais defesa.\n\n";
    cout << "pressione Enter para voltar ao Menu.";
    cin.get();
    cout << "\033c";
    cout << "  Jogar\n  Como Jogar\n> Itens\n  Sair\n"; // Sai do jogo e volta pro menu.
 }

const int N_OPCOES = 4;
int selectedMenuItem = 0, MenuItem = 0, Menu = 0;
string Opcoes[N_OPCOES] = {"  Jogar   ", "Como Jogar", "  Itens   ", "  Sair    "};

void RenderMenu(int MenuItem){ //renderiza o menu, limpando e reescrevendo com "> ".
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