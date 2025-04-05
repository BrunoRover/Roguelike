#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#define GAMEELEMENTS_H
using namespace std;


struct GameElements{
    char person = char(36);
    char wall = char(219);
    char path = ' ';
    char key = char(42);
    char potion = char(38);
    char door = 'D';
    char trap = char(63);

};

// aquelaCarinha = "( ͡° ͜ʖ ͡°)";
// dead = "×͜×";

void generateMap(vector<vector<int>> &m, int rows, int cols) {

    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                m[i][j] = 1;
            } else {
            if (rand() % 6 == 0) {
                m[i][j] = 1;
            } else {
                m[i][j] = 0;
            }
            }
        }
    }

}

void Game(){

    cout << "\033c" << "Historia do jogo\n" << "Pressione Enter para seguir.\n";
    cin.get();
    cout << "\033c" << "mais historia bla bla\n" << "Pressione Enter para seguir.\n";;
    cin.get();
    cout << "\033c";

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
setlocale(LC_ALL, "Portuguese");

    short int CX=0, CY=0;https://github.com/BrunoRover/Roguelike.git
    COORD coord;
    coord.X = CX;
    coord.Y = CY;

    srand(time(NULL));
    int rows = 10 + rand() % 11;  // Linhas entre 10 e 20
    int cols = 10 + rand() % 11;  // Colunas entre 10 e 20

    vector<vector<int>> m(rows, vector<int>(cols));
    generateMap(m, rows, cols);

    int x=5, y=5;
    char key;

    while(true){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        GameElements elements;

        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                if(i == y && j == x){
                    cout<<char(36);
                } else {
                    switch (m[i][j]){
                        case 0: cout<<elements.path; break;
                        case 1: cout<<elements.wall; break;
                        default: cout<<"-";
                    }
                }
            }
            cout<<"\n";
        }

         if ( _kbhit() ){
            key = getch();
            switch(key)
            {
                case 72: case 'w':
                    if (m[y-1][x] == 0) y--;
                break;
                case 80: case 's':
                    if (m[y+1][x] == 0) y++;
                break;
                case 75:case 'a':
                    if(m[y][x-1] == 0) x--;
                break;
                case 77: case 'd':
                    if(m[y][x+1] == 0) x++;
                break;
            }if (key == 27) {  // 27 � o c�digo da tecla ESC
                cout << "\033c" << "Tecla ESC pressionada, jogo encerrado\n Pressione Enter para voltar ao Menu" << endl;
                cin.get();
                cout << "\033c";
                cout << "> Jogar\n  Como Jogar\n  Itens\n  Sair\n";
                break;  // Sai do jogo e volta pro menu.
            }
         }
    }
}

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
