#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "GameElements.cpp"

using namespace std;



int selectedMenuItem = 0, currentMenuItem = 0, Menu = 0, currentGameState = 0;
const int Jogar = 0, ComoJogar = 1, Itens = 2, Sair = 3;

vector<string> menuItems =
{
        "Jogar",
        "Como Jogar",
        "Itens",
        "Sair"
};

void ClearConsole() {
    cout << "\033c";
}

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

};

void Jogo(){

    cout << "Historia do jogo\n" << "Pressione Enter para seguir.\n";
    cin.get();
    cout << "mais historia bla bla\n" << "Pressione Enter para seguir.\n";;
    cin.get();
    cout << "\033c";

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    short int CX=0, CY=0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;

    srand(time(NULL));
    int rows = 10 + rand() % 11;  // Linhas entre 10 e 20
    int cols = 10 + rand() % 11;  // Colunas entre 10 e 20

    vector<vector<int>> m(rows, vector<int>(cols));
    generateMap(m, rows, cols);

    int x=5, y=5;
    char tecla;

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
            tecla = getch();
            switch(tecla)
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
            }if (tecla == 27) {  // 27 é o código da tecla ESC
                cout << "Tecla ESC pressionada, jogo encerrado\n Pressione Enter para voltar ao Menu" << endl;
                cin.get();
                ClearConsole();
                cout << "> Jogar\n  Como Jogar\n  Itens\n  Sair\n";
                break;  // Sai do jogo e volta pro menu.
            }
         }
    }

}

void Instrucaojogo (){
    ClearConsole();
    cout << "   Como Jogar \n1 Movimento e Controles:\nUse as teclas 'W''A''S''D' para mover seu personagem. Inserir controles adicionais aqui, se necessário: por exemplo, Use o mouse para mirar e atacar.";
    cout << "\n2 Gestão de Inventário:\nA cada novo item encontrado, você poderá acessá-lo através do seu inventário, instruções exemplo: I ou Esc \n";
    cout << "3 Mecânica da Morte Permanente: \nRoguelike segue o sistema de Permadeath , o que significa que, se seu personagem morrer, você terá que começar uma nova partida desde o início.\n";
    cout << "4 Durante o Jogo: \nVocê poderá encontrar itens, que te ajudaram a avançar de fase e subir de nível, o jogo só finaliza quando você matar o boss final.";
    cout << "\npressione Enter para voltar";
    cin.get();
    ClearConsole();
    cout << "  Jogar\n> Como Jogar\n  Itens\n  Sair\n";// Sai do jogo e volta pro menu.
}

void RenderMenu() //renderiza o menu, limpando e reescrevendo com "> ".
{
    ClearConsole();

    for (int i = 0; i < menuItems.size(); i++)
    {
        const string& currentMenuItem = menuItems[i];

        if (selectedMenuItem == i)
            printf("> ");
        else
            printf("  ");
        printf("%s \n", currentMenuItem.c_str());
    }

}

void OnKeyUp() //lógica aplicada quando é pressionado a tecla 'W' ou seta pra cima.
{
    if (currentGameState == Menu)
    {
         if (selectedMenuItem == 0)
            selectedMenuItem = menuItems.size() - 1;
        else
            selectedMenuItem -= 1;

        RenderMenu();
    }
}

void OnKeyDown() //lógica aplicada quando é pressionado S ou seta pra baixo.
{
    if (currentGameState == Menu)
    {
        if (selectedMenuItem == menuItems.size() - 1)
            selectedMenuItem = 0;
        else
            selectedMenuItem += 1;

        RenderMenu();
    }
}

void OnEnterKeyPressed() //Menu do Jogo.
{
    if (currentGameState == Menu)
    {
        currentMenuItem = selectedMenuItem;

        switch (currentMenuItem)
        {
            case Jogar: //Será iniciado o jogo se selecionado.
                Jogo();
                break;

            case ComoJogar: //Será exibido as instruções do jogo se for selecionado.
                Instrucaojogo();


                break;

            case Itens: //Será exibido os itens do jogo.
                ClearConsole();
                cout << "paçoca, colher, batata\n";
                cout << "pressione Enter para voltar";
                cin.get();
                ClearConsole();
                cout << "  Jogar\n  Como Jogar\n> Itens\n  Sair\n";
                break;  // Sai do jogo e volta pro menu.

            case Sair: //Encerrará o programa.
                cout << "encerrado.";
                exit(0);
                break;
        }
    }

}

int main()
{
    RenderMenu();

    while (true) {
        if (_kbhit()) {

            int first = _getch();

            // Verifica se é a tecla Enter (código 13) Ou Espaço (código 32).
            if (first == 13 || first == 32)
                OnEnterKeyPressed();

            // Verifica se é uma tecla de seta ou "W"/"S".
            if (first == 224) {
                int second = _getch();
                if (second == 72) // Seta para cima.
                    OnKeyUp();
                else if (second == 80) // Seta para baixo.
                    OnKeyDown();
            } else if (first == 'W' || first == 'w') { // Tecla "W".
                OnKeyUp();
            } else if (first == 'S' || first == 's') { // Tecla "S".
                OnKeyDown();
            }
        }
    }

    return 0;
}
