#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int countButtons = 4;
const int maxLifeNpc = 2;
string buttons[countButtons] = {"ATACAR", "DEFENDER", "ITENS", "FUGIR"};
string addInfoCombate = "";
int turn = 0;
int enemyCount = 0;


struct Item{
    int buffId;
    string name;
    string description;
    bool unicUse;
    int value;
};

Item itens[4] = {
    {1,"Poção de cura","Uma poção feita por um grande alquimista ao usa-la ganha +1 de vida",true,1},
    {2,"Espada do sol","Uma espada feita com uma sentelha divina do Deus Tyr.Aumenta +2 de acerto permanente",false,2},
    {3,"Escudo da pureza","Um escudo onde protege o portador de verdadeira alma pura.Aumenta +2 a defesa permanente",false,2},
    {4,"Pergaminho de misseis magicos","Um pergaminho que retira 1 de vida do inimigo sem precisar de acerto",true,1}
};


struct Player {
    int life;
    int attack;
    int defense;
    Item inventory[4];
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

void actionNpc(Combatant* combatant1,Combatant* combatant2){
    if(combatant1->npc.life <= (maxLifeNpc/2) && !combatant1->npc.healing){
        addInfoCombate = "O drown se curou com uma poção.";
        combatant1->npc.life += 1;
        combatant1->npc.healing = true;
    }else{
        bool onHit = makeAttack(combatant1->npc.attack,combatant2->player.defense);
        if (onHit){
            combatant2->player.life -= 1;
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
            if(buttons[i] != ""){
                if (i == selectedOption) {
                    cout << "[>>" << buttons[i] << "<<]   ";
                } else {
                    cout << "[ " << buttons[i] << " ]   ";
                }
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
            actionNpc(&infoCombat[indexCombat],&infoCombat[(indexCombat == 0 ? 1 :0)]);
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
                        bool onHit = makeAttack(infoCombat[indexCombat].player.attack,infoCombat[(indexCombat == 0 ? 1 :0)].npc.defense);
                        if(onHit){
                            infoCombat[(indexCombat == 0 ? 1 :0)].npc.life -= 1;
                        }
                    break;
                    case 1:
                        infoCombat[indexCombat].player.defense += 1;
                    break;
                    case 2:
                        int indexButton = 0;
                        for (int i = 0; i < 4; i++){
                            Item item = infoCombat[indexCombat].player.inventory[i];
                            if(item.unicUse){
                                buttons[indexButton] = item.name;
                                indexButton += 1; 
                            }
                        }

                        buttons[countButtons] = "Voltar";
                        indexButton += 1; 
                        while (indexButton <= 3){
                            buttons[indexButton] = "";
                        }
                    
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
    Player player = {3,1,1,{itens[0],itens[1],itens[2],itens[3]}};
    int coutEnemie = 1;
    Npc enemies[coutEnemie];
    generateEnemies(enemies,coutEnemie);
    Combatant infoCombat[coutEnemie + 1];
    generateInitiatives(infoCombat, enemies, coutEnemie, &player);
    combatMenu(infoCombat,coutEnemie,&player);
    return 0;
}
