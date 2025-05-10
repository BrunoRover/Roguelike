#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

void levelToUp(thisXp) {
    int buffLevelSelected = 0;

    //garante que ele sofreu o level up
    while(thisXp != player.level){
        //garante que a tecla seja valida
        do {
            key = _getch();
        } while (key != 'a' && key != 'A' && key != 'd' && key != 'D' && key != '\r');

        //renderiza a interface do combate
        displayCombatInterface(selectedOption, indexCombat, infoCombat, totalCombatants,buttonsLayout);
        playerInfoCombat = "";

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
            switch (selectedOption) {
                //aumenta a life do player
                case 0: {
                    player.life ++;
                }
                break;

                //aumenta o ataque do player
                case 1:{
                    player.attack ++;
                }
                break;

                //aumenta a defesa do player
                case 2:{
                    player.defense ++;
                }
                break;
            }
            
            player.level++
        }
    }
}