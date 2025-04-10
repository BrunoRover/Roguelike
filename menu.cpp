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
    cout << ">> Como Jogar <<\n\n1 Movimento e Controles:\nUse as teclas 'W''A''S''D' para mover seu personagem. Use o 'A' e 'D' para mover o cursor em uma batalha, e 'Enter' para selecionar a ação.\n";
    cout << "\n2- Ações de Batalha: \nAo iníciar uma batalha, você terá 4 opções: 'Atacar' (gera um ataque no inimigo), 'Defender' (defende do inimigo), 'Fugir' (sai do combate), 'Itens' (abre inventário, para utilizar algum ítem coletado).\n";
    cout << "\n3- Mecânica da Morte Permanente: \nRoguelike segue o sistema de morte permanente , o que significa que, se você morrer, você terá que recomeçar a partida desde o início.\n";
    cout << "\nPressione 'Enter' para voltar ao Menu.";
    cin.get();
    cout << "\033c";
    cout << "  Jogar\n> Como Jogar\n  Itens\n  Sair\n";// Sai do jogo e volta pro menu.
}

 void Itens(){
    cout << "\033c";
    cout << ">> ITENS <<\n\n";
    cout << "1- Poção de cura: Uma poção feita por um grande alquimista ao usa-la ganha +1 de vida\n\n";
    cout << "2- Pergaminho de misseis mágicos: Retira 1 de vida do inimigo sem precisar de acerto.\n\n";
    cout << "3- Espada do sol: Uma espada feita com uma sentelha divina do Deus Tyr. Aumenta +2 de acerto permanente.\n\n";
    cout << "4- Escudo da pureza: Um escudo onde protege o portador de verdadeira alma pura. Aumenta +2 a defesa permanente.\n\n";
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
    cout << "= ROGUELIKE MENU =\n";
    cout << "|================|\n";
    for (int i = 0; i < N_OPCOES; i++){
            if (MenuItem == i){
            cout << "|>> " << Opcoes[i] << " <<|" << endl;
            }else{
            cout << "|   " << Opcoes[i] << "   |" << endl;
        }   cout << "|================|\n";
    }
}
