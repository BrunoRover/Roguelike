#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>   
#include <ctime>
#include <iomanip> // Para setw e setfill, exibi��o do tempo de jogo.
#include "GameElements.cpp"  
#include "BossMap.cpp"  
#include "GameMap.cpp"
#include "menu.cpp"
#include "Combat.cpp"

using namespace std;

int minutes = 0, seconds = 0, score = 0;

void Score(){
    score = kill * 5;
    score += quantityOfItemCollected;
    score += player.key;
    score -= minutes / 2;
}

void Game() {

    cout << "\033c" << "Historia do jogo\n\n" << "Pressione 'Enter' para seguir.\n";
    cin.get();
    cout << "\033c" << "mais historia bla bla\n\n" << "Pressione 'Enter' para seguir.\n";;
    cin.get();
    cout << "\033c";
    time_t start = time(nullptr);
    kill = 0;

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
    int rows = 25;  
    int cols = 105;

    GameMap mapa = getRandomMap();
    VisibleMap vmap;
    int x = cols / 2, y = rows / 2;
    int bossX = 12, bossY = 12;
    char tecla;

    initItems(mapa);
    vmap.visible[y][x] = true;
    GameElements elements;  
    

    while (true) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    
        if (player.bossMap && !player.inBossRoom) {
            // Transição para a sala do boss
            player.inBossRoom = true;
            x = bossX;
            y = bossY;
        }

        if (player.inBossRoom) {
            //Necessário fazer isso para não ficar piscando a tela
            if (player.bossRoomFirstEntry) {
                system("cls");
                player.bossRoomFirstEntry = false;
            }

            bossRoom(mapFinal, x, y);
            if (x == 12 && y == 12) {
                // Iniciar combate com o boss
                // lógica de combate aqui
                lastMessage = "Você encontrou o Chefão! Prepare-se para lutar!";
            }
        } else {
            checkItems(mapa, x, y);
            drawMap(mapa, vmap, x, y);
        }
        
        
        drawInfo();
        if (_kbhit()) {
            tecla = _getch();
            switch (tecla) {
                case 72: case 'w': 
                    if (player.inBossRoom) {
                        if (y > 0 && mapFinal.tiles[y-1][x] == 0) y--;
                    } else {
                        if (y > 0 && mapa.tiles[y-1][x] == 0) y--;
                    }
                    break;
                case 80: case 's': 
                    if (player.inBossRoom) {
                        if (y < 24 && mapFinal.tiles[y+1][x] == 0) y++;
                    } else {
                        if (y < 24 && mapa.tiles[y+1][x] == 0) y++;
                    }
                    break;
                case 75: case 'a': 
                    if (player.inBossRoom) {
                        if (x > 0 && mapFinal.tiles[y][x-1] == 0) x--;
                    } else {
                        if (x > 0 && mapa.tiles[y][x-1] == 0) x--;
                    }
                    break;
                case 77: case 'd': 
                    if (player.inBossRoom) {
                        if (x < 24 && mapFinal.tiles[y][x+1] == 0) x++;
                    } else {
                        if (x < 104 && mapa.tiles[y][x+1] == 0) x++;
                    }
                    break;
                case 27: 
                    cout << "\033c" << "Tecla 'ESC' pressionada, jogo encerrado.\n\n";
                    time_t end = time(nullptr);
                    double total_seconds = difftime(end, start);
                    minutes = static_cast<int>(total_seconds) / 60;
                    seconds = static_cast<int>(total_seconds) % 60;
                    Score();
                    cout << "Sua pontuacao foi: " << score << endl << endl;
                    cout << "Vida: " << player.life << " | Itens Coletados: " << quantityOfItemCollected << "/" << MAX_ITEMS << " | Chaves Coletadas: " << player.key << endl;
                    cout << "\nTempo de Jogo: " << minutes << ":" << setw(2) << setfill('0') << seconds << " minutos\n\n";
                    cout << "Pressione 'Enter' para voltar ao Menu." << endl;
                    cin.get();
                    cout << "\033c";
                    return;
                
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
}

int main() {
    bool exit = false;
    RenderMenu(MenuItem);
    
    while (!exit) {
        if (_kbhit()) {
            int key = _getch();
            switch (key) {
                case 72: case 'w': case 'W':
                    if (MenuItem == 0){
                        MenuItem = 3;
                    } else {
                        MenuItem -= 1;
                    }
                    break;
                case 80: case 's': case 'S':
                    if (MenuItem == N_OPCOES-1){
                        MenuItem = 0;
                    } else {
                        MenuItem +=1;
                    }
                    break;
                case 13: case 32:
                    if (Opcoes[MenuItem] == "  Jogar   ") {
                        system("cls");
                        Game();
                        system("cls");
                        RenderMenu(MenuItem);
                    } else if (Opcoes[MenuItem] == "Como Jogar") {
                        instructiongame();
                    } else if (Opcoes[MenuItem] == "  Itens   ") {
                        Itens();
                    } else if (Opcoes[MenuItem] == "  Sair    ") {
                        cout << "Saindo...\n";
                        exit = true;
                    }
                    break;
            }
            RenderMenu(MenuItem);
        }
    }
    return 0;
}