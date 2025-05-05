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
bool isWin = false;

//reseta "todas" as variaveis
void Reset() {
    gameOver = false;
    isWin = false;
    player.life = 5;
    player.attack = 1;
    player.defense = 0;
    player.key = 0;
    player.inventoryCount = 0;
    player.bossMap = 0;
    player.inBossRoom = false;
    player.bossRoomFirstEntry = true;
    player.inventory[0];
    itemCount = 0;
    minutes = 0;
    seconds = 0;
    score = 0;
    enemieCount = 0;
    gameItems[MAX_ITEMS];
    quantityOfItemCollected = 0;
    mapFinal.tiles[5][12] = bossIcon;
    lastMessage = "";
}
// calcula o score
void Score(){
    score = kill * 5;
    score += quantityOfItemCollected;
    score += player.key;
    score -= minutes / 2;
}

//função que mostra a pontuação em geral do player no final
void drawInfoFinal(time_t start) {
    // Calcula o tempo total de jogo
    cout << "\033c";
    time_t end = time(nullptr);
    double total_seconds = difftime(end, start);
    minutes = static_cast<int>(total_seconds) / 60;
    seconds = static_cast<int>(total_seconds) % 60;

    // Mostra pontuação final
    Score();
    cout << "Sua pontuacao foi: " << score << endl << endl;
    cout << "Vida: " << player.life << " | Itens Coletados: " << player.inventoryCount << "/" << MAX_ITEMS << " | Chaves Coletadas: " << player.key << endl;
    cout << "\nTempo de Jogo: " << minutes << ":" << setw(2) << setfill('0') << seconds << " minutos\n\n";

    //cout << "Pressione 'Enter' para voltar ao Menu." << endl;
    //cin.get();
}

void Game() {

    //pagina1 e pagina2 é referente a história do jogo
    cout << pagina1;
    cin.get();
    clearConsole();
    cout << "\033c";
    cout << pagina2;
    cin.get();
    cout << "\033c";
    time_t start = time(nullptr); //inicia o tempo de jogo
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

    //sorteia uma das matrizes de jogo
    GameMap mapa = getRandomMap();
    VisibleMap vmap;
    int x = cols / 2, y = rows / 2;
    int bossX = 12, bossY = 12;
    char tecla;

    //gerear os inimigos para o combate
    generateEnemies(enemies);
    //sortear a posição x e y dos inimigos
    initEnemies(mapa);

    //inicia o itens
    initItems(mapa);
    vmap.visible[y][x] = true;
    GameElements elements;

    //logica do jogo
    while (!gameOver && !isWin) {
        if (player.life == 0){
            gameOver = true;
        }
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

            if (mapFinal.tiles[y][x] == bossIcon && !isWin && !gameOver) {
                if (player.life == 0){
                    gameOver = true;
                }
                // Iniciar combate com o boss, lógica de combate aqui
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
                    lastMessage = "Voce morreu!\n\n";
                    cout << "Tente novamente, quem sabe na proxima...";
                    cin.get();
                    cout << "\033c";
                }else if (returnCombat == 1){
                    lastMessage = "VOCE DERROTOU O BOSS\n\n";
                    cout << "VITORIA, apos derrotar o boss, voce encontra a Lina e consegue fugir da montanha...\n\n";
                    cin.get();
                    isWin = true;
                    cout << "\033c";
                }
                mapFinal.tiles[y][x] = 0;
                if (mapFinal.tiles[y][x] == 0){
                    isWin = true;
                }
            }
        } else {
            moveEnemiesRandomly(mapa); //funcao para movimentar todos inimigos do mapa
            checkItems(mapa, x, y); //funcao responsavel por coletar o item pelo jogador
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
            drawMap(mapa, vmap, x, y); //responsavel por desenhar o mapa e atualizar visibilidade
        }

        drawInfo(); // mostra as informações do jogador e do jogo em tempo real
        if (_kbhit()) {
            tecla = _getch();
            //lógica de movimentação do jogador na sala do boss
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
                    clearConsole();
                    gameOver = true;
                    drawInfoFinal(start);
                    cout << "Pressione 'Enter' para voltar ao Menu." << endl;
                    cin.get();

            }
        }
    }
    clearConsole();
    gameOver = true;
    drawInfoFinal(start);
    cout << "Pressione 'Enter' para voltar ao Menu." << endl;
    cin.get();

}

int main() {
    bool exit = false;
    RenderMenu(MenuItem);

    //enquanto o jogo não for encerrado, continua no menu
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
                        Difficulty();
                        Reset(); //toda vez que reiniciar o jogo, reseta as variaveis para evitar bugs e lixo de memoria
                        Game(); //inicia o jogo
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
