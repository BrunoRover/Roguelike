#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iostream>
#include <string>
#include <conio.h>
#include "GameElements.cpp"

using namespace std;

void levelToUp(int thisXp,Player &player) {
    int selectedOption = 0;
    char key;
    int randBuff = rand() % 3;

    //garante que ele sofreu o level up
    while(thisXp != player.level){
        clearConsole();
        //renderiza a interface do Level up
        cout << padRight("==================== Level UP ==========================================");
        cout << padRight("");
        cout << padRight("Use A (Esquerda) e D (Direita) para selecionar | ENTER para confirmar");
        cout << padRight("");
        string buttonsUp = "";
        for (int i = 0; i < 3; i++) {
            if (i == selectedOption){
                buttonsUp += "[>>" + buffCombat[i] + "<<]   ";
            }else{
                buttonsUp += "[ " + buffCombat[i] + " ]   ";
            }
        }

        cout << padRight(buttonsUp);
        cout << padRight("");
        cout << padRight("========================================================================");
        
        for (int i = 0; i < 22; i++) {
            cout << padRight("");
        }

        //garante que a tecla seja valida
        if (player.modeIA) {
            Sleep(800);
            key = '\r';
            if(randBuff > 0){
                key = 'a';
                randBuff--;
            }
        } else {
            do {
                key = _getch();
            } while (key != 'a' && key != 'A' && key != 'd' && key != 'D' && key != '\r');
        }

        //move o "botao"
        if (key == 'a' || key == 'A') {
            (selectedOption == 0)? selectedOption = 2 : selectedOption--;
        } else if (key == 'd' || key == 'D') {
            (selectedOption == 2)? selectedOption = 0 : selectedOption++;
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
            
            player.level++;
        }
    }
}