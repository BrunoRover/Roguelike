#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iostream>
#include <string>
#include <conio.h>
#include "GameElements.cpp"
using namespace std;

string buttons[countButtons] = {"ATACAR", "DEFENDER", "ITENS", "FUGIR"};

void generateEnemies(Npc enemies[]) {
    for (int i = 0; i < countEnemies; i++) {
        enemies[i] = {false, maxLifeNpc, 0, 0};
    }
}




















int randNumb() {
    return rand() % 10 + 1;
}

bool makeAttack(int attack, int defend) {
    int valueAttack = randNumb() + attack - defend;
    return (valueAttack > 5);
}

void sortCombatants(Combatant combatants[], int totalCombatants) {
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

void generateInitiatives(Combatant infoCombat[], Npc enemies[], int coutEnemie, Player player) {
    int totalCombatants = coutEnemie + 1;

    infoCombat[0].name = "Jogador";
    infoCombat[0].isNpc = false;
    infoCombat[0].initiative = randNumb();
    infoCombat[0].player = player;

    for (int i = 1; i < totalCombatants; i++) {
        infoCombat[i].name = ("Inimigo " + to_string(i));
        infoCombat[i].isNpc = true;
        infoCombat[i].initiative = randNumb();
        infoCombat[i].npc = enemies[i - 1];
    }

    sortCombatants(infoCombat, totalCombatants);
}

void actionNpc(Combatant& npc, Combatant& player) {
    if (npc.npc.life <= (maxLifeNpc / 2) && !npc.npc.healing) {
        addInfoCombat = "O inimigo se curou com uma poção.";
        npc.npc.life++;
        npc.npc.healing = true;
    } else {
        bool onHit = makeAttack(npc.npc.attack, player.player.defense);
        if (onHit) {
            player.player.life--;
            addInfoCombat = "O inimigo acertou um ataque.";
        } else {
            addInfoCombat = "O inimigo errou o ataque.";
        }
    }
}

void removeCombatant(Combatant infoCombat[], int& totalCombatants, int indexToRemove) {
    for (int i = indexToRemove; i < totalCombatants - 1; ++i) {
        infoCombat[i] = infoCombat[i + 1];
    }
    totalCombatants--;
}

void removeItemFromInventory(Combatant& combatant, string item) {
    int indexitem = 0;
    for (int i = 0; i <= 3; ++i) {
        if(combatant.player.inventory[i].name == item){
            indexitem = i;
            break;
        }
    }
    for (int i = indexitem; i < 3; ++i) {
        combatant.player.inventory[i] = combatant.player.inventory[i + 1];
    }
    combatant.player.inventory[3] = Item{};
}

void displayCombatInterface(int selectedOption, int indexCombat, Combatant infoCombat[], int totalCombatants,string buttonsLayout[countButtons]) {
    clearConsole();
    string infoCombatant = "";
    cout << padRight("==================== INICIATIVA ====================");
    for (int i = 0; i < totalCombatants; i++) {
        infoCombatant = ((i == indexCombat) ? "> " : "  ");
        infoCombatant += (infoCombat[i].name + " (Iniciativa: " + to_string(infoCombat[i].initiative) + ") Vida: " + to_string((infoCombat[i].isNpc) ? infoCombat[i].npc.life : infoCombat[i].player.life));
        cout << padRight(infoCombatant);   
    }
    cout << "===================================================\n";

    if (!infoCombat[indexCombat].isNpc) {
        cout << padRight("Use A (Esquerda) e D (Direita) para selecionar | ENTER para confirmar");
        string buttonsCombat = "";
        for (int i = 0; i < countButtons; i++) {
            if (buttonsLayout[i] != "") {
                if (i == selectedOption){
                    buttonsCombat += "[>>" + buttonsLayout[i] + "<<]   ";
                }else{
                    buttonsCombat += "[ " + buttonsLayout[i] + " ]   ";
                }
            }
        }
        cout << padRight(buttonsCombat);
        cout << padRight(playerInfoCombat);
        cout << padRight("=================================================");
        cout << padRight(addInfoCombat);
    } else {
        cout << padRight(addInfoCombat);
        cout << padRight("======================================================");
        cout << padRight(playerInfoCombat);
        cout << padRight("Precione ENTER para prosseguir o turno");
        cout << padRight("=================================================");
    }
}

void combatMenu(Combatant infoCombat[], int& totalCombatants, Player& player) {
    int selectedOption = 0;
    char key;
    bool endCombat = false;
    int indexCombat = 0;
    int actions = 2;
    int targetIndex = (indexCombat == 0) ? 1 : 0;
    string buttonsLayout[countButtons];
    copy(begin(buttons), end(buttons), begin(buttonsLayout));
    while (!endCombat && player.life > 0 && totalCombatants > 1) {
        if (infoCombat[indexCombat].isNpc) {
            actionNpc(infoCombat[indexCombat], infoCombat[0]);
        } else {
            addInfoCombat = "Você tem " + to_string(actions) + " ações restantes.";
        }

        displayCombatInterface(selectedOption, indexCombat, infoCombat, totalCombatants,buttonsLayout);

        key = _getch();
        if (!infoCombat[indexCombat].isNpc) {
            if (actions == 2) {
                playerInfoCombat = "";
                infoCombat[indexCombat].player.defense = player.defense;
                infoCombat[indexCombat].player.attack = player.attack;
            }

            if (key == 'a' || key == 'A') {
                (selectedOption > 0)?selectedOption--: selectedOption = countButtons - 1;
                while(buttonsLayout[selectedOption] == ""){
                    selectedOption--;
                }
            } else if (key == 'd' || key == 'D') {
                (selectedOption < (countButtons - 1))? selectedOption++ : selectedOption = 0;
                while(buttonsLayout[selectedOption] == ""){
                    if(selectedOption >= (countButtons - 1)){
                        selectedOption = 0;
                    }else{
                        selectedOption++;
                    }
                }
            } else if (key == '\r') {
                if (!exibirItens) {
                    switch (selectedOption) {
                        case 0: {
                            if (infoCombat[targetIndex].isNpc) {
                                bool onHit = makeAttack(infoCombat[indexCombat].player.attack, infoCombat[targetIndex].npc.defense);
                                if (onHit) {
                                    playerInfoCombat = "Você acertou o ataque";
                                    infoCombat[targetIndex].npc.life--;

                                    // Verifica se o NPC morreu
                                    if (infoCombat[targetIndex].npc.life <= 0) {
                                        playerInfoCombat = infoCombat[targetIndex].name + " foi derrotado!";
                                        removeCombatant(infoCombat, totalCombatants, targetIndex);
                                        if (indexCombat >= totalCombatants){
                                            indexCombat = 0;
                                        }
                                        if (totalCombatants == 1){
                                            endCombat = true;
                                        }
                                        continue;
                                    }

                                } else {
                                    playerInfoCombat = "Você errou o ataque";
                                }
                            }
                            actions--;
                        }
                        break;
                        case 1:
                            infoCombat[indexCombat].player.defense++;
                            playerInfoCombat = "Você aumentou sua defesa";
                            actions--;
                            break;
                        case 2:{
                            selectedOption = 0;
                            exibirItens = true;
                            int indexInventory = 0;
                            bool toBack = false;
                            for (int i = 0; i < countButtons; i++){
                                bool isPush = false;
                                while (isPush == false && indexInventory < 4){
                                    if(infoCombat[indexCombat].player.inventory[indexInventory].unicUse){
                                        isPush = true;
                                        buttonsLayout[i] = infoCombat[indexCombat].player.inventory[indexInventory].name;
                                    }
                                    indexInventory++;
                                }
                                if(isPush == false && toBack == false){
                                    toBack = true;
                                    buttonsLayout[i] = "Voltar";
                                }else if(isPush == false){
                                    buttonsLayout[i] = "";
                                }
                            }
                            
                        }
                        break;
                        case 3:
                            if (randNumb() == 10){
                                endCombat = true;
                            }else{
                                playerInfoCombat = "Você não conseguiu fugir.";
                            }
                            actions = 0;
                        break;
                    }
                }else{
                    if (buttonsLayout[selectedOption] == itens[0].name){
                        playerInfoCombat = "Você usou poção de cura";
                        infoCombat[indexCombat].player.life++;
                        actions--;
                        removeItemFromInventory(infoCombat[indexCombat],itens[0].name);
                    }else if(buttonsLayout[selectedOption] == itens[1].name){
                        playerInfoCombat = "Você usou Pergaminho de misseis mágicos";
                        infoCombat[targetIndex].npc.life--;
                        actions--;
                        removeItemFromInventory(infoCombat[indexCombat],itens[1].name);
                    }
                    selectedOption = 0;
                    exibirItens = false;
                    for (int i = 0; i < countButtons; i++) {
                        buttonsLayout[i] = buttons[i];
                    }
                }
            }

            if (actions == 0) {
                indexCombat = (indexCombat + 1) % totalCombatants;
                actions = 2;
                turn++;
            }

        } else {
            if (key == '\r') {
                indexCombat = (indexCombat + 1) % totalCombatants;
                actions = 2;
                turn++;
            }
        }
    }
    
    int playerIndex = -1;
    for (int i = 0; i < totalCombatants; ++i) {
        if (!infoCombat[i].isNpc) {
            playerIndex = i;
            break;
        }
    }

    if (playerIndex != -1) {
        player.life = infoCombat[playerIndex].player.life;

        for (int i = 0; i < 4; ++i) {
            player.inventory[i] = infoCombat[playerIndex].player.inventory[i];
        }
    }

    clearConsole();
    cout << padRight(((player.life <= 0) ? "Você foi derrotado!" : "Você venceu o combate!"));
    cout << padRight("Precione Enter para prosseguir");
    cout << padRight("");
    cout << padRight("");
    cout << padRight("");
    cout << padRight("");
    cout << padRight("");
    cout << padRight("");
    cout << padRight("");
    do {
        key = _getch();
    }while (key!= '\r');
}

