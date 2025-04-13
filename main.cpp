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

using namespace std;

int minutes = 0, seconds = 0, score = 0;

void Score(){
    // score = kill * 5;
    score += quantityOfItemCollected;
    score += player.key;
    score -= minutes / 2;
}

void Game() {

    cout << pagina1;
    cin.get();
    clearConsole();
    cout << "\033c";
    cout << pagina2;
    cin.get();
    cout << "\033c";
    time_t start = time(nullptr);
    // kill = 0;

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

    generateEnemies(enemies);
    initEnemies(mapa);

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
            bool isWin = false;
            bossRoom(mapFinal, x, y);
            lastMessage = "x=" + to_string(x) + "y=" + to_string(y) + to_string(mapFinal.tiles[(y / 10)][x]);

            if (mapFinal.tiles[y][x] == bossIcon && !isWin && !gameOver) {
                // Iniciar combate com o boss
                // lógica de combate aqui
                lastMessage = "Voce encontrou o Chefao! Prepare-se para lutar!";
                clearConsole();
                Combatant infoCombatBoss[2];
                int countEnemiesBoss = 1;
                Npc enemiesBoss[countEnemiesBoss];
                generateEnemies(enemiesBoss,true);
                generateInitiatives(infoCombatBoss, enemiesBoss, countEnemiesBoss, player);
                int totalCombatantsBoss = countEnemiesBoss + 1;
                int returnCombat = combatMenu(infoCombatBoss, totalCombatantsBoss, player);
    
                if(returnCombat == 0){
                    player.life = 0;
                    gameOver = true; 
                    lastMessage = "Voce morreu!";
                }else if (returnCombat == 2){
                    lastMessage = "Voce derrotou o Boss";
                    isWin = true;
                    //colocar aq algo para vitoria final
                }
                mapFinal.tiles[y][x] = 0;
            }
        } else {
            moveEnemiesRandomly(mapa);
            checkItems(mapa, x, y);
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
            drawMap(mapa, vmap, x, y);
        }


        drawInfo();
        if (_kbhit()) {
            tecla = _getch();
            switch (tecla) {
                case 72: case 'w':
                    if (player.inBossRoom) {
                        if (y > 0 && mapFinal.tiles[y-1][x] != 1) y--;
                    } else {
                        if (y > 0 && mapa.tiles[y-1][x] == 0) y--;
                    }
                    break;
                case 80: case 's':
                    if (player.inBossRoom) {
                        if (y < 24 && mapFinal.tiles[y+1][x] != 1) y++;
                    } else {
                        if (y < 24 && mapa.tiles[y+1][x] == 0) y++;
                    }
                    break;
                case 75: case 'a':
                    if (player.inBossRoom) {
                        if (x > 0 && mapFinal.tiles[y][x-1] != 1) x--;
                    } else {
                        if (x > 0 && mapa.tiles[y][x-1] == 0) x--;
                    }
                    break;
                case 77: case 'd':
                    if (player.inBossRoom) {
                        if (x < 24 && mapFinal.tiles[y][x+1] != 1) x++;
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
                    cout << "Vida: " << player.life << " | Itens Coletados: " << player.inventoryCount << "/" << MAX_ITEMS << " | Chaves Coletadas: " << player.key << endl;
                    cout << "\nTempo de Jogo: " << minutes << ":" << setw(2) << setfill('0') << seconds << " minutos\n\n";
                    cout << "Pressione 'Enter' para voltar ao Menu." << endl;
                    cin.get();
                    cout << "\033c";
                    return;

            }
        }
    }
}

int main() {
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
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
