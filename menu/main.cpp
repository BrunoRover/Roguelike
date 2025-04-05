#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "menu.cpp"
#include <locale.h>
using namespace std;

int main(){
    setlocale(LC_ALL, "Portuguese");
    bool exit = false;
    RenderMenu(MenuItem);
    while (!exit) {

        if (_kbhit()) { // Verifica se uma tecla foi pressionada.
            int key = _getch(); // Captura a tecla.
            switch (key) {
                case 72: case 'w': case 'W': // Seta para cima ou W.
                    if (MenuItem == 0){
                        MenuItem = 3;
                        }else{
                        MenuItem -= 1;
                        }
                    break;
                case 80: case 's': case 'S': // Seta para baixo ou S.
                    if (MenuItem == N_OPCOES-1){
                        MenuItem = 0;
                        }else {
                        MenuItem +=1;
                        }
                    break;
                case 13: case 32: // Detecta se foi pressionado a tecla Enter ou Espaço, e exibe na tela o menu do item selecionado.
                    if (Opcoes[MenuItem] == "  Jogar   "){
                    Game();
                    }if (Opcoes[MenuItem] == "Como Jogar"){
                    instructiongame();
                    }if (Opcoes[MenuItem] == "  Itens   "){
                    Itens();
                    }if (Opcoes[MenuItem] == "  Sair    ") {
                    cout << "Saindo...\n";
                    exit = true;
                    }break;
                }RenderMenu(MenuItem);
        }
    }return 0;
}
