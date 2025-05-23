#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iostream>
#include <string>
#include <conio.h>
#include "GameElements.cpp"
using namespace std;
// int  kill = 0;
bool gameOver = false;

string buttons[countButtons] = {"ATACAR", "DEFENDER", "ITENS", "FUGIR"};

//usado para gerear os inimigos para o combate
void generateEnemies(Npc enemies[],bool isBoss = false) {
    if(isBoss){
        for (int i = 0; i < countEnemies; i++) {
            enemies[i] = {false, ((maxLifeNpc * 2) + difficulty), (2 + difficulty), (2 + difficulty),true};
        }
    }else{
        for (int i = 0; i < countEnemies; i++) {
            enemies[i] = {false, (maxLifeNpc + difficulty), (0 + difficulty), (0 + difficulty),false};
        }
    }
}

//funcao usada de base para gerar numero aleatorio ate 10
int randNumb() {
    return rand() % 10 + 1;
}

// funcao usada para tentar fazer o ataque com base nos dados
bool makeAttack(int attack, int defend) {
    int valueAttack = randNumb() + attack - defend;
    return (valueAttack > 5);
}

//muda a ordenação do combate
void sortCombatants(Combatant combatants[], int totalCombatants, int i = 0, int j = 0) {
    // Caso base: terminou o array
    if (i >= totalCombatants - 1) return;

    // Loop interno recursivo
    if (j < totalCombatants - i - 1) {
        if (combatants[j].initiative < combatants[j + 1].initiative) {
            Combatant temp = combatants[j];
            combatants[j] = combatants[j + 1];
            combatants[j + 1] = temp;
        }
        sortCombatants(combatants, totalCombatants, i, j + 1); // continua comparando
    } else {
        sortCombatants(combatants, totalCombatants, i + 1, 0); // passa para o próximo "round" de bolhas
    }
}

//gera a iniciativa de todos combatentes incluindo o jogador
void generateInitiatives(Combatant infoCombat[], Npc enemies[], int coutEnemie, Player player,typeNpc typeCombat) {
    int totalCombatants = coutEnemie + 1;

    infoCombat[0].name = "Jogador";
    infoCombat[0].isNpc = false;
    infoCombat[0].initiative = randNumb();
    infoCombat[0].player = player;

    for (int i = 1; i < totalCombatants; i++) {
        infoCombat[i].npc = enemies[i - 1];
        infoCombat[i].name = ("Inimigo " + to_string(i) + "(" + typeCombat.type + ")");
        infoCombat[i].npc.life += typeCombat.bonusLife;
        infoCombat[i].npc.attack += typeCombat.bonusAttack;
        infoCombat[i].npc.defense += typeCombat.bonusDefense;
        infoCombat[i].isNpc = true;
        infoCombat[i].initiative = randNumb();
    }

    sortCombatants(infoCombat, totalCombatants);
}

//gera a iniciativa de todos combatentes incluindo o jogador usado para o boss
void generateInitiatives(Combatant infoCombat[], Npc enemies[], int coutEnemie, Player player) {
    int totalCombatants = coutEnemie + 1;

    infoCombat[0].name = "Jogador";
    infoCombat[0].isNpc = false;
    infoCombat[0].initiative = randNumb();
    infoCombat[0].player = player;

    for (int i = 1; i < totalCombatants; i++) {
        infoCombat[i].npc = enemies[i - 1];
        infoCombat[i].name = ("Inimigo " + to_string(i) + "(Boss)");
        infoCombat[i].isNpc = true;
        infoCombat[i].initiative = randNumb();
    }

    sortCombatants(infoCombat, totalCombatants);
}


//acoes automaticas do npc
void actionNpc(Combatant& npc, Combatant& player) {
    //verifica se ele ja curou a vida e se esta abaixo da metade
    if (npc.npc.life <= (maxLifeNpc / 2) && !npc.npc.healing) {
        addInfoCombat = "O inimigo se curou com uma pocao.";
        npc.npc.life++;
        npc.npc.healing = true;
    } else {
        bool onHit = makeAttack(npc.npc.attack, player.player.defense);
        if (onHit) {
            player.player.life--;
            if (player.player.life < 0) player.player.life = 0;
            addInfoCombat = "O inimigo acertou um ataque.";
        } else {
            addInfoCombat = "O inimigo errou o ataque.";
        }
    }
}

//retira o npc que foi morto
void removeCombatant(Combatant infoCombat[], int& totalCombatants, int indexToRemove) {
    for (int i = indexToRemove; i < totalCombatants - 1; ++i) {
        infoCombat[i] = infoCombat[i + 1];
    }
    totalCombatants--;
}

//retira o item do inventario ao ser usado
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

// renderiza a intereface de combate
void displayCombatInterface(int selectedOption, int indexCombat, Combatant infoCombat[], int totalCombatants,string buttonsLayout[countButtons]) {
    clearConsole();
    string infoCombatant = "";
    cout << padRight("==================== INICIATIVA ====================");
    for (int i = 0; i < totalCombatants; i++) {
        infoCombatant = ((i == indexCombat) ? "> " : "  ");
        infoCombatant += (infoCombat[i].name + " (Iniciativa: " + to_string(infoCombat[i].initiative) + ") Vida: " + to_string((infoCombat[i].isNpc) ? infoCombat[i].npc.life : infoCombat[i].player.life));
        cout << padRight(infoCombatant);
    }
    cout << padRight("====================================================");

    //acoes esclusivas do jogador
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
        cout << padRight("====================================================");
        cout << padRight(addInfoCombat);
    } else {
        cout << padRight(addInfoCombat);
        cout << padRight("====================================================");
        cout << padRight(playerInfoCombat);
        cout << padRight("Precione ENTER para prosseguir o turno");
        cout << padRight("====================================================");
    }
    for (int i = 0; i <= 19; i++){
        cout << padRight("");
    }
}

//toda logica de como e executado o combate acoes ordenacao e fim
int combatMenu(Combatant infoCombat[], int& totalCombatants, Player& player) {
    int selectedOption = 0;
    char key;
    bool isWin = false;
    bool leaveCombat = false;
    int indexCombat = 0;
    int indexPlayer = 0;
    int actions = 2;
    int targetIndex = 0;
    string buttonsLayout[countButtons];
    copy(begin(buttons), end(buttons), begin(buttonsLayout));
    //executa o combate ate o jogador ou o inimigos morrerem
    while (!leaveCombat && !isWin && player.life > 0 && totalCombatants > 1) {
        if (infoCombat[indexPlayer].player.life <= 0){
            cout << "\033c";
            cout << "VOCE FOI DERROTADO";
            player.life = 0;
            cin.get();
            gameOver = true;
            return 0;
        }
        targetIndex = -1;
        for (int i = 0; i < totalCombatants; i++) {
            if (infoCombat[i].isNpc && i != indexCombat) {
                targetIndex = i;
            }else if(!infoCombat[i].isNpc && i != indexCombat){
                indexPlayer = i;
            }
        }

        //executa a acao do npc
        if (infoCombat[indexCombat].isNpc) {
            actionNpc(infoCombat[indexCombat], infoCombat[indexPlayer]);
            if (infoCombat[indexPlayer].player.life <= 0) {
                infoCombat[indexPlayer].player.life = 0; // Evita vida negativa
                cout << "\033c";
                player.life = 0;
                gameOver = true;
                break; // Sai do while e finaliza combate
            }
        } else {
            addInfoCombat = "Voce tem " + to_string(actions) + " acoes restantes.";
        }

        //renderiza a interface do combate
        displayCombatInterface(selectedOption, indexCombat, infoCombat, totalCombatants,buttonsLayout);
        playerInfoCombat = "";

        //verifica se e a vez do jogador
        if (!infoCombat[indexCombat].isNpc) {
            //garante que a tecla seja valida
            if (player.modeIA) {
                Sleep(800);
                key = '\r'; // IA sempre "pressiona" Enter
            } else {
                do {
                    key = _getch();
                } while (key != 'a' && key != 'A' && key != 'd' && key != 'D' && key != '\r');
            }

            //rezeta os buffs
            if (actions == 2) {
                playerInfoCombat = "";
                infoCombat[indexCombat].player.defense = player.defense;
                infoCombat[indexCombat].player.attack = player.attack;
                //verifica se tem buffs de itens passivos
                for (int i = 0; i < player.inventoryCount; i++) {
                    if(player.inventory[i].unicUse == false){
                        if(player.inventory[i].buffId == 3){
                            infoCombat[indexCombat].player.attack +=  player.inventory[i].value;
                        }else if(player.inventory[i].buffId == 4){
                            infoCombat[indexCombat].player.defense +=  player.inventory[i].value;
                        }
                    }
                }
            }
            //move o "botao"
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
            }//selecionou uma opção
            else if (key == '\r') {
                if (!exibirItens) {
                    switch (selectedOption) {
                        //executa o ataque
                        case 0: {
                            if (infoCombat[targetIndex].isNpc) {
                                bool onHit = makeAttack(infoCombat[indexCombat].player.attack, infoCombat[targetIndex].npc.defense);
                                if (onHit) {
                                    playerInfoCombat = "Voce acertou o ataque";
                                    infoCombat[targetIndex].npc.life--;

                                    // Verifica se o NPC morreu
                                    if (infoCombat[targetIndex].npc.life <= 0) {
                                        playerInfoCombat = infoCombat[targetIndex].name + " foi derrotado!";
                                        removeCombatant(infoCombat, totalCombatants, targetIndex);
                                        kill = kill + 1;
                                        if (indexCombat >= totalCombatants){
                                            indexCombat = 0;
                                        }
                                        if (totalCombatants == 1){
                                            isWin = true;
                                        }
                                        break;
                                    }

                                } else {
                                    playerInfoCombat = "Voce errou o ataque";
                                }
                            }
                            actions--;
                        }
                        break;
                        //aumenta a defesa
                        case 1:
                            infoCombat[indexCombat].player.defense++;
                            playerInfoCombat = "Voce aumentou sua defesa";
                            actions--;
                            break;
                        //seleciona um item
                        case 2: {
                            selectedOption = 0;
                            exibirItens = true;

                            // Limpa layout anterior
                            for (int i = 0; i < countButtons; i++) {
                                buttonsLayout[i] = "";
                            }

                            int botaoAtual = 0;

                            // Só preenche até (countButtons - 1) slots com itens
                            for (int i = 0; i < player.inventoryCount && botaoAtual < countButtons - 1; i++) {
                                if (infoCombat[indexCombat].player.inventory[i].unicUse) {
                                    buttonsLayout[botaoAtual] = infoCombat[indexCombat].player.inventory[i].name;
                                    botaoAtual++;
                                }
                            }

                            // Último botão sempre é "Voltar"
                            buttonsLayout[countButtons - 1] = "Voltar";
                        }
                        break;
                        //tenta fugir do combate e zera as ações
                        case 3:
                            if(infoCombat[targetIndex].npc.isBoss){
                                playerInfoCombat = "Nao e possivel fugir do boss!";
                            }else{
                                if (randNumb() == 10){
                                    leaveCombat = true;
                                    playerInfoCombat = "Voce fugiu do combate!";
                                } else {
                                    playerInfoCombat = "Voce nao conseguiu fugir.";
                                }
                                actions = 0;
                            }
                        break;
                    }
                }else{

                    //logica para uso dos itens
                    if (buttonsLayout[selectedOption] == itens[0].name){
                        //cura o player
                        playerInfoCombat = "Voce usou " + itens[0].name;
                        infoCombat[indexCombat].player.life++;
                        actions--;
                         // muda cor para verde (cura)
                        player.colorText = 10;
                        //remove o item dps de usado
                        removeItemFromInventory(infoCombat[indexCombat],itens[0].name);
                    }else if(buttonsLayout[selectedOption] == itens[1].name){
                        //aplica dano no inimigo
                        playerInfoCombat = "Voce usou " + itens[1].name;
                        infoCombat[targetIndex].npc.life--;
                        actions--;
                        // muda cor para vermelho (ataque mágico)
                        player.colorText = 12;
                        //remove o item dps de usado
                        removeItemFromInventory(infoCombat[indexCombat],itens[1].name);
                    }else if(buttonsLayout[selectedOption] == itens[5].name){
                        //aplica dano no inimigo
                        playerInfoCombat = "Voce usou " + itens[5].name;
                        infoCombat[targetIndex].npc.life -= rand() % (itens[5].value + 1);
                        actions--;
                        // muda cor para vermelho (ataque mágico)
                        player.colorText = 12;
                        //remove o item dps de usado
                        removeItemFromInventory(infoCombat[indexCombat],itens[5].name);
                    }
                    selectedOption = 0;
                    exibirItens = false;
                    for (int i = 0; i < countButtons; i++) {
                        buttonsLayout[i] = buttons[i];
                    }
                }
            }
            // se o player nao tem mais acão muda o turno e mudo para o priximo inimigo
            if (actions == 0) {
                indexCombat = (indexCombat + 1) % totalCombatants;
                actions = 2;
                turn++;
            }

        } else {
            //garante que a tecla seja valida
            if (player.modeIA) {
                Sleep(800);
                key = '\r'; // IA sempre "pressiona" Enter
            } else {
                do {
                    key = _getch();
                } while (key != '\r');
            }

            //caso tenha confirmado vai para proxima acao
            if (key == '\r') {
                indexCombat = (indexCombat + 1) % totalCombatants;
                actions = 2;
                turn++;
            }
        }

        //verifica se o player morreu
        if (infoCombat[indexPlayer].player.life <= 0){
            cout << "\033c";
            cout << "VOCE FOI DERROTADO";
            bool reLife = false;
                    
            for (int i = 0; i < player.inventoryCount; i++){
                if(player.inventory[i].buffId == 7){
                    reLife = true;
                    break;
                }
            }

            //verifica se tem o item que revive
            if(reLife){
                cout << "MAS O " + itens[6].name + "SALVOU SUA VIDA E SE ROMPEU";
                player.life = 1;
                player.colorText = 10;
                removeItemFromInventory(infoCombat[indexPlayer],itens[6].name);
            }else{
                player.life = 0;
                cin.get();
                gameOver = true;
                return 0;
            }

        }
    }
    // atualiza os index
    int playerIndex = -1;
    for (int i = 0; i < totalCombatants; ++i) {
        if (!infoCombat[i].isNpc) {
            playerIndex = i;
            break;
        }
    }

    //atualiza a vida do player
    if (playerIndex != -1) {
        player.life = infoCombat[playerIndex].player.life;
        player.xp += npcXp;
        for (int i = 0; i < 4; ++i) {
            player.inventory[i] = infoCombat[playerIndex].player.inventory[i];
        }
    }

    string strCombat = "";
    playerInfoCombat = "";
    int returnCombat = 0;
    //define a mensagem de combate e o retorno para o combate
    if (player.life <= 0) {
        strCombat = "Voce foi derrotado!";
    } else if (leaveCombat) {
        returnCombat = 1;
        strCombat = "Voce fugiu do combate!";
    } else {
        returnCombat = 2;
        strCombat = "Voce venceu o combate!";
    }

    clearConsole();
    cout << padRight(strCombat);
    cout << padRight("Precione Enter para prosseguir");
    //garante que nao tenha lixo de memoria na tela
    for (int i = 0; i <= 6; i++){
        cout << padRight("");
    }
    
    if (player.modeIA) {
        Sleep(800);
        key = '\r'; 
    } else {
        do {
            key = _getch();
        } while (key != '\r');
    }

    clearConsole();
    system("cls");

    return returnCombat;
}