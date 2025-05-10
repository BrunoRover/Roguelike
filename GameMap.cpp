#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iostream>
#include "GameElements.cpp"
#include "Combat.cpp"
using namespace std;

int quantityOfItemCollected = 0;
string lastMessage = "";

GameElements elements;
Player player; 

struct GameMap{
    int tiles[25][105];
};

struct VisibleMap {
    bool visible[25][105] = {false}; 
};

struct GameItem {
    int x, y;
    char type;
    bool collected;
};

GameItem gameItems[MAX_ITEMS];
int itemCount = 0;

struct EnemySpawn {
    int id, x, y;
    char type;
    bool active;
    typeNpc typeNpc;
};

int enemyMoveCounter = 0;
const int enemyMoveDelay = 7;
int enemieCount = 0;

EnemySpawn enemySpawns[countVisibleEnemies];

//função responsavel por sortear a posição x e y dos inimigos
void initEnemies(GameMap& map) {

    for (int i = 0; i < countVisibleEnemies; i++) {
        int x, y;

        do {
            x = rand() % 105;
            y = rand() % 25;
        } while (map.tiles[y][x] != 0); // 1 = parede, 0 = caminho
        
        int typeCombat = rand() % 3;
        enemySpawns[i].x = x;
        enemySpawns[i].y = y;
        enemySpawns[i].active = true;
        enemySpawns[i].type = elements.enemy;
        enemySpawns[i].id = enemieCount;
        enemySpawns[i].typeNpc = typesNpc[typeCombat];
        
        enemieCount++;
    }

}

//função responsavel mostrar inimigo 
void drawEnemie(VisibleMap& vmap) {
    for (int i = 0; i < countVisibleEnemies; i++) {
        if (enemySpawns[i].active && vmap.visible[enemySpawns[i].y][enemySpawns[i].x]) {
            COORD coord;
            coord.X = (SHORT)enemySpawns[i].x;
            coord.Y = (SHORT)enemySpawns[i].y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            cout << enemySpawns[i].typeNpc.color + enemySpawns[i].type + "\033[0m";
        }
    }
}

//funcao para movimentar todos inimigos do mapa
void moveEnemiesRandomly(GameMap& map) {
    if (enemyMoveCounter >= enemyMoveDelay) {
        for (int i = 0; i < countVisibleEnemies; i++) {
            if (!enemySpawns[i].active) continue;
    
            int dx[8] = { -1, -1, -1,  0, 1, 1, 1,  0 };
            int dy[8] = { -1,  0,  1,  1, 1, 0, -1, -1 };
    
            int currentX = enemySpawns[i].x;
            int currentY = enemySpawns[i].y;
    
            // Tentativas máximas para se mover
            for (int attempts = 0; attempts < 8; attempts++) {
                int dir = rand() % 8;
                int newX = currentX + dx[dir];
                int newY = currentY + dy[dir];
    
                // Verifica limites e se é um caminho válido (0)
                if (newX >= 0 && newX < 105 && newY >= 0 && newY < 25 && map.tiles[newY][newX] == 0) {
                    enemySpawns[i].x = newX;
                    enemySpawns[i].y = newY;
                    break;
                }
            }
        }
        enemyMoveCounter = 0;
    } else {
        enemyMoveCounter++;
    }
}

//função responsavel por sortear a posição x e y dos itens e a % correspondente do type de cada item 
void initItems(GameMap& map) {
    itemCount = 0;

    if (map.tiles[2][2] == 4) {
        gameItems[itemCount].type = elements.key;
        gameItems[itemCount].x = 2;
        gameItems[itemCount].y = 2;
        gameItems[itemCount].collected = false;
        map.tiles[2][2] = 0;
        map.tiles[10][2] = 0; 
        itemCount++;
    } 
    else if (map.tiles[10][2] == 4) {
        gameItems[itemCount].type = elements.key;
        gameItems[itemCount].x = 2;
        gameItems[itemCount].y = 10;
        gameItems[itemCount].collected = false;
        map.tiles[10][2] = 0;
        map.tiles[2][2] = 0; 
        itemCount++;
    }

    if (map.tiles[5][87] == 3) {
        gameItems[itemCount].type = elements.bossMap;
        gameItems[itemCount].x = 87;  
        gameItems[itemCount].y = 5;   
        gameItems[itemCount].collected = false;
        map.tiles[5][87] = 0;
        itemCount++;
    } 
    else if (map.tiles[21][7] == 3) {
        gameItems[itemCount].type = elements.bossMap;
        gameItems[itemCount].x = 7;  
        gameItems[itemCount].y = 21;  
        gameItems[itemCount].collected = false;
        map.tiles[21][7] = 0;
        itemCount++;
    }

    for (int i = itemCount; i < MAX_ITEMS; i++) {
        int x, y;
        do {
            x = rand() % 105;
            y = rand() % 25;
        } while (map.tiles[y][x] != 0); // 1 = parede, 0 = caminho
        
        gameItems[i].x = x;
        gameItems[i].y = y;
        gameItems[i].collected = false;
        
        // 75% item, 25% armadilha (já que chave e bossMap são fixos)
        gameItems[i].type = (rand() % 4 == 0) ? elements.trap : elements.item;
    }
    
    itemCount = MAX_ITEMS; // Atualiza o contador total
}

//função responsavel mostrar itens 
void drawItems(VisibleMap& vmap) {
    for (int i = 0; i < itemCount; i++) {
        if (!gameItems[i].collected && vmap.visible[gameItems[i].y][gameItems[i].x]) { 
            COORD coord;
            coord.X = (SHORT)gameItems[i].x;
            coord.Y = (SHORT)gameItems[i].y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            cout << gameItems[i].type;
        }
    }
}

//função responsavel coletar o item pelo jogador e a logica de cada item
void checkItems(GameMap& map, int playerX, int playerY) {

    for (int i = 0; i < itemCount; i++) {
        if (!gameItems[i].collected && gameItems[i].x == playerX && gameItems[i].y == playerY) {
            gameItems[i].collected = true;

            quantityOfItemCollected++; // itens coletados

            // item para o inventario
            if (gameItems[i].type == elements.item) { 
                int newIndex = rand() % (coutMaxItens - 1);
                Item newItem = itens[newIndex];
                bool haveItem = false;

                for (int i = 0; i <= player.inventoryCount; i++){
                    if(player.inventory[i].buffId == newItem.buffId){
                        haveItem = true;
                        break;
                    }
                }
                
                if(haveItem) {
                    lastMessage = "Voce achou o item " + newItem.name + ",mas ja tem esse item!";
                    player.xp += itemXp;
                    break;
                }else{
                    player.inventory[player.inventoryCount] = newItem;
                    player.inventoryCount++;
                    lastMessage = "Voce achou o item " + newItem.name + ",ele foi adicionado ao seu inventario!";
                }
            }

            // logica para as armadilhas
            else if (gameItems[i].type == elements.trap) {
                int effect = rand() % 5; 

                switch(effect) {
                    case 0: 
                        player.life -= 1;
                        lastMessage = "Voce pisou numa armadilha! -1 de vida!";
                        break;
                    case 1: 
                        player.life -= 2;
                        lastMessage = "Armadilha perigosa! -2 de vida!";
                        break;
                    case 2: 
                        player.life += 1;
                        lastMessage = "Armadilha inofensiva! +1 de vida!";
                        break;
                    case 3: 
                        lastMessage = "Voce escapou da armadilha sem dano!";
                        break;
                    case 4: 
                        if (player.key > 0) {
                            player.key -= 1;
                            lastMessage = "A armadilha fez Voce perder uma chave!";
                        } else {
                            lastMessage = "A armadilha tentou pegar uma chave, mas Voce nao tem nenhuma!";
                        }
                        break;
                }
            } 
            // logica para as chaves 
            else if (gameItems[i].type == elements.key) {
                player.key += 1;
                lastMessage = "Você pegou uma chave! +1 chave!";

                // logica para abrir as portas quando tiver chave
                if(player.key > 0) {
                    for (int y = 0; y < 25; y++) {
                        for (int x = 0; x < 105; x++) {
                            if (map.tiles[y][x] == 2) { // 2 = porta
                                map.tiles[y][x] = 0;   // transforma em caminho
                                lastMessage = "Voce abriu as portas! Voce pode passar pelo icone: #";
                            }
                        }
                    }
                } 
            }
            //logica para entrar na area do boss
            else if (gameItems[i].type == elements.bossMap) {
                player.bossMap += 1;  
                lastMessage = "Você encontrou o mapa do boss! Agora você sabe onde ele está!";
                
                if(player.bossMap > 0) {
                    for (int y = 0; y < 25; y++) {
                        for (int x = 0; x < 105; x++) {
                            if (map.tiles[y][x] == 3) { // 3 = bossMap
                                map.tiles[y][x] = 0;   // transforma em caminho
                                lastMessage = "Voce abriu as portas! Voce pode passar pelo icone: #";
                            }
                        }
                    }
                } 
            }
        }
    }

    for (int i = 0; i < countVisibleEnemies; i++) {
        if (enemySpawns[i].active && enemySpawns[i].x == playerX && enemySpawns[i].y == playerY) {
            clearConsole();
            Combatant infoCombat[maxCombatants];
            typeNpc typeCombat = enemySpawns[i].typeNpc;
            generateInitiatives(infoCombat, enemies, countEnemies, player, typeCombat);
            int totalCombatants = countEnemies + 1;
    
            // inicia o combate contra o inimigo
            int returnCombat = combatMenu(infoCombat, totalCombatants, player);
            lastMessage = "Voce derrotou um inimigo!";
            //verifica o retorno do combate
            if(returnCombat == 0){
                //avisa a morte e termina o jogo
                player.life = 0;
                gameOver = true; 
                lastMessage = "Voce morreu!";
            }else if (returnCombat == 1){
                //caso de fulga teleporta o inimigo para outra parte do mapa
                const int mapHeight = 25;
                const int mapWidth = 105;
    
                int newx = 0;
                int newy = 0;
                do {
                    newx = rand() % mapWidth;
                    newy = rand() % mapHeight;  // garante newy < 25
                } while (map.tiles[newy][newx] != 0);
    
                enemySpawns[i].x = newx;
                enemySpawns[i].y = newy;
    
                lastMessage = "Voce fugiu do inimigo!";
            }else if (returnCombat == 2){
                //caso de vitoria
                lastMessage = "Voce derotou o inimigo";
                enemySpawns[i].active = false;
            }
            break;
        }
    }
}

//função responsavel por mostrar informações na tela
void drawInfo() {
    COORD coord;
    coord.X = 0;
    coord.Y = 26; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << padRight("Vida: " + to_string(player.life) + " | Itens Coletados: " + to_string(quantityOfItemCollected) + "/" + to_string(MAX_ITEMS) + " | Chaves Coletadas: " + to_string(player.key));
    cout << "\n";
    cout << padRight("Mensagens: " + lastMessage);  
}

//função responsavel por sortear a matriz de jogo
GameMap getRandomMap(){
    GameMap map1 = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
        1,0,4,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    };
    GameMap map2 = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
        1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
        1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
        1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
        1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,
        1,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,
        1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,4,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,
        1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,
        1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,
        1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
        1,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
    };

    srand(time(NULL));
    if(rand() % 2 == 0){
        return map1;
    } else {
        return map2;
    }
}

//função responsavel por desenhar o mapa e atualizar visibilidade do mapa
void drawMap(GameMap& map, VisibleMap& vmap, int playerX, int playerY) {
    int rows = 25;  
    int cols = 105;

    // atualizar visibilidade do mapa

    int visionRadius = rangeVision;

    for (int i = 0; i < player.inventoryCount; i++){
        if(player.inventory[i].buffId == 5){
            visionRadius += player.inventory[i].value;
            break;
        }
    }
    

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (abs(i - playerY) <= visionRadius && abs(j - playerX) <= visionRadius) {
                vmap.visible[i][j] = true;
            }
        }
    }

    // desenhar mapa 
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == playerY && j == playerX) {
                cout << elements.person;
            } else if (vmap.visible[i][j]) {
                bool hasItem = false;  // verificar se há um item nesta posição
                for (int k = 0; k < itemCount; k++) {
                    if (!gameItems[k].collected && 
                        gameItems[k].x == j && 
                        gameItems[k].y == i) {
                        cout << gameItems[k].type;
                        hasItem = true;
                        break;
                    }
                }

                if (!hasItem) {
                    bool hasEnemy = false;
                    for (int e = 0; e < countVisibleEnemies; e++) {
                        if (enemySpawns[e].active && enemySpawns[e].x == j && enemySpawns[e].y == i) {
                            cout << enemySpawns[e].type; // Inimigo visível
                            hasEnemy = true;
                            break;
                        }
                    }

                    if(!hasEnemy){
                        if (map.tiles[i][j] == 1) {
                            cout << elements.wall; // parede
                        } else if (map.tiles[i][j] == 2) {
                            cout << elements.door; // porta
                        } else {
                            cout << elements.path; // caminho
                        }
                    }
                }
            } else {
                cout << elements.dark; // espaço escuro
            }
        }
        cout << '\n';
    }

    drawItems(vmap); //função responsavel mostrar itens sendo invocada na hora de desenhar o mapa
    drawEnemie(vmap); //função responsavel mostrar inimigo sendo invocada na hora de desenhar o mapa
}

