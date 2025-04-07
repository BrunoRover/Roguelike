#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>   
#include <ctime>
#include "GameElements.cpp"  
#include "GameMap.cpp"
#include "Combat.cpp"

using namespace std;

int main() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; 
    SetConsoleCursorInfo(out, &cursorInfo);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    short int CX=0, CY=0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;

    srand(time(NULL));
    int rows = 25;  
    int cols = 105;

    GameMap mapa = getRandomMap();
    VisibleMap vmap;
    int x = cols / 2, y = rows / 2;
    char tecla;

    initItems(mapa);
    vmap.visible[y][x] = true;
    GameElements elements;  

    while (true) {

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        checkItems(mapa, x, y);
        drawMap(mapa, vmap, x, y);
        // bossRoom(mapFinal,  x, y); //falta implementar a logica do jogo na sala do boss

        drawInfo();
        if (_kbhit()) {
            tecla = _getch();
            switch (tecla) {
                case 72: case 'w': 
                    if (y > 0 && mapa.tiles[y-1][x] == 0) y--; 
                break;
                case 80: case 's': 
                    if (y < rows-1 && mapa.tiles[y+1][x] == 0) y++; 
                break;
                case 75: case 'a': 
                    if (x > 0 && mapa.tiles[y][x-1] == 0) x--; 
                break;
                case 77: case 'd': 
                    if (x < cols-1 && mapa.tiles[y][x+1] == 0) x++; 
                break;
                case 27: return 0;  
            }
        }

    }

















    // Player player = {3, 1, 1, {itens[0], itens[1], itens[2], itens[3]}};
    // int coutEnemie = 1;
    // Npc enemies[coutEnemie];
    // generateEnemies(enemies, coutEnemie);
    // Combatant infoCombat[maxCombatants];
    // generateInitiatives(infoCombat, enemies, coutEnemie, player);
    // int totalCombatants = coutEnemie + 1;

    // combatMenu(infoCombat, totalCombatants, player);
    // return 0;













    return 0;
}