#pragma once
using namespace std;
#include <string>
#include <windows.h>

const int countButtons = 4;
const int maxLifeNpc = 2;
const int maxCombatants = 10;
const int baseLength = 100;
const int countVisibleEnemies = 5;
const int coutMaxItens = 4;
int  kill = 0;

string addInfoCombat = "";
string playerInfoCombat = "";
int turn = 0;
bool exibirItens = false;

struct Item {
    int buffId;
    string name;
    string description;
    bool unicUse;
    int value;
};

struct GameElements {
    char person = char(36);  
    char wall = char(219);   
    char path = ' '; 
    char dark = '.';
    char door = char(35);
    char bossMap = char(33); 
    char key = char(42); //*
    char item = char(38); //&
    char trap = char(63); //?
    char enemy = char(64); //@
};

Item itens[coutMaxItens] = {
    {1, "Poção de cura", "Uma poção feita por um grande alquimista ao usa-la ganha +1 de vida", true, 1},
    {2, "Pergaminho de misseis mágicos", "Retira 1 de vida do inimigo sem precisar de acerto", true, 1},
    {3, "Espada do sol", "Uma espada feita com uma sentelha divina do Deus Tyr. Aumenta +2 de acerto permanente", false, 2},
    {4, "Escudo da pureza", "Um escudo onde protege o portador de verdadeira alma pura. Aumenta +2 a defesa permanente", false, 2}
};

struct Player {
    int life = 5;
    int attack = 1;
    int defense = 0;
    int key = 0;
    Item inventory[coutMaxItens];
    int inventoryCount = 0;
    int bossMap = 0; 
    bool inBossRoom = false;
    bool bossRoomFirstEntry = true;
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

string padRight(const string& text) {
    if (text.length() >= baseLength) return text.substr(0, baseLength);
    return text + string(baseLength - text.length(), ' ') + "\n";
}

void clearConsole() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

const int countEnemies = 1;
Npc enemies[countEnemies];
