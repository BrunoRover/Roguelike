#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "GameElements.cpp"

using namespace std;



int selectedMenuItem = 0;

enum EGameState
{
    Menu
};

enum EGameState currentGameState = EGameState::Menu;

vector<string> menuItems =
{
        "Iniciar",
        "Como Jogar",
        "Sair"
};

enum EMenuItem
{
    Iniciar,
    ComoJogar,
    Sair
};

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

void Game(){
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
            }
         }
    }

}

void ClearConsole() {
    cout << "\033c";
}

void RenderMenu()
{
    ClearConsole();

    for (int i = 0; i < menuItems.size(); i++)
    {
        const string& currentMenuItem = menuItems[i];

        if (selectedMenuItem == i)
            printf("> ");

        printf("%s \n", currentMenuItem.c_str());
    }

}

void OnKeyUp()
{
    if (currentGameState == EGameState::Menu)
    {
         if (selectedMenuItem == 0)
            selectedMenuItem = menuItems.size() - 1;
        else
            selectedMenuItem -= 1;

        RenderMenu();
    }
}

void OnKeyDown()
{
    if (currentGameState == EGameState::Menu)
    {
        if (selectedMenuItem == menuItems.size() - 1)
            selectedMenuItem = 0;
        else
            selectedMenuItem += 1;

        RenderMenu();
    }
}

void OnEnterKeyPressed()
{
    if (currentGameState == EGameState::Menu)
    {
        EMenuItem currentMenuItem = static_cast<EMenuItem>(selectedMenuItem);

        switch (currentMenuItem)
        {
            case EMenuItem::Iniciar:
                Game();
                break;

            case EMenuItem::ComoJogar:
                cout << "como jogar selecionado";

                break;

            case EMenuItem::Sair:
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

            if (first == 13)
                OnEnterKeyPressed();

            if (first == 224)
            {
                int second = _getch();
                if (second == 72)
                    OnKeyUp();
                else if (second == 80)
                    OnKeyDown();
            }

        }
    }

    return 0;
}
