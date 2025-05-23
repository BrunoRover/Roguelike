#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <iomanip> // Para setw e setfill, exibi��o do tempo de jogo.
#include <thread>  // para simular tempo de execução
#include <chrono>
#include "GameElements.cpp"
#include "iaMode.cpp"
#include "save.cpp"
#include "BossMap.cpp"
#include "GameMap.cpp"
#include "menu.cpp"
#include "ConfigLevel.cpp"

using namespace std;

int minutes = 0, seconds = 0, score = 0;
bool isWin = false;

//reseta "todas" as variaveis
void Reset() {
    gameOver = false;
    isWin = false;
    player.attack = 0;
    player.defense = 0;
    player.key = 0;
    player.inventoryCount = 0;
    player.bossMap = 0;
    player.phase = 1;
    player.colorText = 7;
    player.inBossRoom = false;
    player.bossRoomFirstEntry = true;
    player.inPhaseRoom = false;
    player.fase2Iniciada = false;
    player.phaseResetDone = false;
    player.phaseRoomFirstEntry = true;
    player.xp = 0;
    player.level = 0;
    player.inventory[0];
    itemCount = 0;
    minutes = 0;
    seconds = 0;
    score = 0;
    gameItems[MAX_ITEMS];
    quantityOfItemCollected = 0;
    mapFinal.tiles[5][12] = bossIcon;
    lastMessage = "";
}

void mostrarTempo(int segundos) { // Exibe o tempo de jogo em tempo real.
    int minutos = segundos / 60;
    int segundos_restantes = segundos % 60;
    cout << "\033[30;30H";
    cout << "\rTempo decorrido: "
              << setfill('0') << setw(2) << minutos << ":"
              << setfill('0') << setw(2) << segundos_restantes
              << flush;
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

    // Salvar relatório
    saveReport(score, player.life, player.inventoryCount, MAX_ITEMS, player.key, minutes, seconds);
}

void Game() {

    //variaveis para a IA
    int sizeCommands;
    char* commandsPlayer = generateCommands(sizeCommands);
    int steps = 0;

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
    player.phase = 1;
    GameMap mapa = getPhaseMap(player.phase);
    VisibleMap vmap;
    int x = cols / 2, y = rows / 2;
    int bossX = 12, bossY = 12;
    char tecla;

    //ajusta a dificuldade do jogo
    player.life -= difficulty;
    countVisibleEnemies += (difficulty * 3);
    itensSpaw -= (difficulty * 3);

    //gerear os inimigos para o combate
    generateEnemies(enemies);
    //sortear a posição x e y dos inimigos
    initEnemies(mapa);

    //inicia o itens
    initItems(mapa, player.phase);
    vmap.visible[y][x] = true;
    GameElements elements;

    bool reset = false;
    bool isReset = false;
    //logica do jogo
    while (!gameOver && !isWin) {
        int thisXp = player.xp / levelUp;

        if (player.life == 0){
            gameOver = true;
        }

        time_t now = time(nullptr);                             //tempo atual
        int elapsed = static_cast<int>(difftime(now, start));        //calcula a diferença do tempo que iniciou e o atual
        mostrarTempo(elapsed);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        if(thisXp != player.level){
            levelToUp(thisXp, player);
        }else{

            if(player.phase == 2){
                mapa = getPhaseMap(2);
            }

            if (player.phase == 2 && !reset) {
                mapa = getPhaseMap(2);
                initItems(mapa, player.phase);
                initEnemies(mapa);
                resetPlayerAndVision(vmap, x, y, rows, cols);
                reset = true;
            }
            if(player.phase == 3){
                mapa = getPhaseMap(3);
            }
            if (player.phase == 3 && !isReset) {
                mapa = getPhaseMap(3);
                initItems(mapa, player.phase);
                initEnemies(mapa);
                resetPlayerAndVision(vmap, x, y, rows, cols);
                isReset = true;
            }

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
            bool input = false;
            if (player.modeIA) {
                if (steps < sizeCommands) {
                    tecla = commandsPlayer[steps++];
                    input = true;
                    Sleep(0); // delay da IA
                } else {
                    gameOver = true;
                    continue;
                }
            } else if (_kbhit()) {
                tecla = _getch();
                input = true;
            }

            if (input) {
                switch (tecla) {
                    case 72: case 'w': case 'W':
                        if (player.inBossRoom) {
                            if (y > 0 && mapFinal.tiles[y-1][x] != 1) y--;
                        } else {
                            if (y > 0 && mapa.tiles[y-1][x] == 0) y--;
                        }
                        break;
                    case 80: case 's': case 'S':
                        if (player.inBossRoom) {
                            if (y < 24 && mapFinal.tiles[y+1][x] != 1) y++;
                        } else {
                            if (y < 24 && mapa.tiles[y+1][x] == 0) y++;
                        }
                        break;
                    case 75: case 'a': case 'A':
                        if (player.inBossRoom) {
                            if (x > 0 && mapFinal.tiles[y][x-1] != 1) x--;
                        } else {
                            if (x > 0 && mapa.tiles[y][x-1] == 0) x--;
                        }
                        break;
                    case 77: case 'd': case 'D':
                        if (player.inBossRoom) {
                            if (x < 24 && mapFinal.tiles[y][x+1] != 1) x++;
                        } else {
                            if (x < 104 && mapa.tiles[y][x+1] == 0) x++;
                        }
                        break;
                    case 105: case 'I':
                        drawInventory();
                        break;
                    case 27:
                        clearConsole();
                        gameOver = true;
                        drawInfoFinal(start);
                        cout << "Pressione 'Enter' para voltar ao Menu." << endl;
                        cin.get();
                }
            }

            if (player.life == 0){
                gameOver = true;
            }
        }
    }
    if(player.modeIA){
        drawInfoFinal(start);
    }
    clearConsole();
    gameOver = true;
    drawInfoFinal(start);
    cout << "Pressione 'Enter' para voltar ao Menu." << endl;
    cin.get();
}

int main() {
    bool exit = false;
    RenderMenu(MenuItem,player);

    //enquanto o jogo não for encerrado, continua no menu
    while (!exit) {

        if (_kbhit()) {
            int key = _getch();
            switch (key) {
                case 72: case 'w': case 'W':
                    if (MenuItem == 0){
                        MenuItem = N_OPCOES-1;
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
                    if (MenuItem == 0) { //Jogar
                        system("cls");
                        Reset(); //toda vez que reiniciar o jogo, reseta as variaveis para evitar bugs e lixo de memoria
                        Game(); //inicia o jogo
                        system("cls");
                        RenderMenu(MenuItem,player);
                    } else if (MenuItem == 1) { //Como Jogar
                        instructiongame();
                    } else if (MenuItem == 2) { //Dificuldade
                        Difficulty(player);
                    } else if (MenuItem == 3) { //Itens
                        Itens();
                    } else if (MenuItem == 4) { //Historico
                        system("cls");
                        cout << "\033c";
                        showReport();
                        cout << "pressione Enter para voltar ao Menu.";
                        cin.get();
                        cout << "\033c";
                    } else if (MenuItem == 5) { //Jogo automatico
                        changeAutoPlay(player);
                    } else if (MenuItem == 6) { //Sair
                        cout << "Saindo...\n";
                        exit = true;
                    }

                    break;
                }
            RenderMenu(MenuItem,player);
        }

    }
    return 0;
}
