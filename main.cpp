#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int countButtons = 4;
const string buttons[countButtons] = {"ATACAR", "DEFENDER", "ITENS", "FUGIR"};
const int maxLifeNpc = 2;
string addInfoCombate = "";
int turn = 0;
int enemyCount = 0;


struct Item{
    int buffId;
    string name;
    string description;
    int value;
};

struct Player {
    int life;
    int attack;
    int defense;
    Item inventory[3];
};

struct Npc {
    bool healing;
    int life;
    int attack;
    int defense;
};

struct Combatant {
    string name;
    int initiative;
    Npc npc;
    Player player;
    bool isNpc;
};

void clearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);
}

void generateEnemies( Npc* enemies, int quantity) {
    enemyCount = quantity;
    for (int i = 0; i < quantity; i++){
        enemies[i] = {false,maxLifeNpc,0,0};
    }
}

int randNumb() {
    return rand() % 10 + 1;
}

bool makeAttack(int attack, int defend){
    int valueAttack = randNumb() + attack - defend;

    if(valueAttack > 5 ){
        return true;
    }

    return false;
}

void sortCombatants(Combatant* combatants, int totalCombatants) {
    for (int i = 0; i < totalCombatants - 1; i++) {
        for (int j = 0; j < totalCombatants - i - 1; j++) {
            if (combatants[j].initiative < combatants[j + 1].initiative) {
                Combatant temp = combatants[j];
                combatants[j] = combatants[j + 1];
                combatants[j + 1] = temp;
            }
        }
    }
}

void generateInitiatives(Combatant* infoCombat, Npc* enemies, int coutEnemie, Player* player) {
    
    int totalCombatants = coutEnemie + 1; // Total = NPCs + 1 jogador
    
    infoCombat[0].name = "Jogador";
    infoCombat[0].isNpc = false;
    infoCombat[0].initiative = randNumb();
    infoCombat[0].player = *player;
    
    for (int i = 1; i < totalCombatants; i++) {
        infoCombat[i].name = ("Inimigo " + to_string(i));
        infoCombat[i].isNpc = true;
        infoCombat[i].initiative = randNumb();
        infoCombat[i].npc = enemies[i - 1];
    }

    sortCombatants(infoCombat, totalCombatants);

}

void actionNpc(Combatant* combatant,Player* player){
    if(combatant->npc.life <= (maxLifeNpc/2) && !combatant->npc.healing){
        addInfoCombate = "O drown se curou com uma poção.";
        combatant->npc.life += 1;
        combatant->npc.healing = true;
    }else{
        bool onHit = makeAttack(combatant->npc.attack,player->defense);
        if (onHit){
            player->life -= 1;
            addInfoCombate = "O drown Acertou um ataque.";
        }else{
            addInfoCombate = "O drown Errou o ataque.";
        }
    }
}
    
void displayCombatInterface(int selectedOption,Combatant* infoCombat,int coutEnemie) {
    clearConsole();

    cout << "==================== INICIATIVA ====================" << endl;

    for (int i = 0; i < coutEnemie + 1; i++) {
        cout << ((i == turn) ? "> ": "  ") << infoCombat[i].name << " (Iniciativa: " << infoCombat[i].initiative << ")" << " Vida: " << ((infoCombat[i].isNpc) ? ( infoCombat[i].npc.life) : ( infoCombat[i].player.life) ) << endl;
    }
    cout << "===================================================" << endl;

    if(!infoCombat[turn].isNpc){
        cout << "Use A (Esquerda) e D (Direita) para selecionar | ENTER para confirmar\n" << endl;
    
        for (int i = 0; i < countButtons; i++) {
            if (i == selectedOption) {
                cout << "[>>" << buttons[i] << "<<]   ";
            } else {
                cout << "[ " << buttons[i] << " ]   ";
            }
        }
        cout << "\n=================================================" << endl;
    }else{
        cout << addInfoCombate << endl;
        cout << "\n=================================================" << endl;
        cout << "Precione ENTER para prosseguir o turno\n" << endl;
        cout << "\n=================================================" << endl;
    }
}

void combatMenu(Combatant* infoCombat,int coutEnemie,Player* player) {
    int selectedOption = 0;
    char key;
    bool endCombat = false;
    int indexCombat = 0;

    while (coutEnemie != 0 || player->life != 0 ) {
        if(infoCombat[indexCombat].isNpc){
            actionNpc(&infoCombat[indexCombat],player);
        }

        displayCombatInterface(selectedOption,infoCombat,coutEnemie);

        key = _getch();
        if(!infoCombat[indexCombat].isNpc){
            if (key == 'a' || key == 'A') {
                if (selectedOption > 0) selectedOption--;
            } else if (key == 'd' || key == 'D') {
                if (selectedOption < 3) selectedOption++;
            } else if (key == '\r') {
                switch (selectedOption){
                    case 0:
                        bool onHit = makeAttack(infoCombat[indexCombat].player.attack,infoCombat[indexCombat].npc.defense);
                        
                    break;
                    case 1:

                    break;
                    case 2:

                    break;
                    case 3:

                    break;
                }
            }
        }else{
            if (key == '\r') {
                if(indexCombat == coutEnemie){
                    indexCombat = 0;
                }else{
                    indexCombat += 1;
                }
                turn += 1;
            }
        }


        // displayCombatInterface(selectedOption,infoCombat,coutEnemie);
    }
}

int main() {
    srand(time(0));
    Player player = {3,1,1,{}};
    int coutEnemie = 1;
    Npc enemies[coutEnemie];
    generateEnemies(enemies,coutEnemie);
    Combatant infoCombat[coutEnemie + 1];
    generateInitiatives(infoCombat, enemies, coutEnemie, &player);
    combatMenu(infoCombat,coutEnemie,&player);
    return 0;
}
