#pragma once
using namespace std;
#include <string>
#include <windows.h>
#define MAX_ITEMS 15


const int countButtons = 4;
const int maxLifeNpc = 2;
const int maxCombatants = 10;
const int baseLength = 120;
const int maxCountVisibleEnemies = 10;
const int coutMaxItens = 7;
const int itemXp = 10;
const int npcXp = 80;
const int levelUp = 100;
const int countEnemiesBoss = 100;

int countVisibleEnemies = 5;
int  itensSpaw = 15;

int  kill = 0;
int  difficulty = 0;
int  rangeVision = 3;

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
    char bossIcon = char(37);
};

Item itens[coutMaxItens] = {
    {1, "Pocao de cura", "Uma pocao feita por um grande alquimista ao usa-la ganha +1 de vida", true, 1},
    {2, "Pergaminho de misseis magicos", "Retira 1 de vida do inimigo sem precisar de acerto", true, 1},
    {3, "Espada do sol", "Uma espada feita com uma sentelha divina do Deus Tyr. Aumenta +2 de acerto permanente", false, 2},
    {4, "Escudo da pureza", "Um escudo onde protege o portador de verdadeira alma pura. Aumenta +2 a defesa permanente", false, 2},
    {5, "Tocha Eterna de Everbright", "Uma tocha mágica que ilumina até os cantos mais escuros. Aumenta o campo de visao do jogador", false, 3},
    {6, "Bomba arcana", "Uma bomba completamente instavel da de 0 a 3 de dano instantaneo ao inimigo", true, 3},
    {7, "Fio de prata", "Ao chegar em 0 de vida, sobrevive com 1 de vida, apos isso o fio se rompe", false, 1}
};

struct Player {
    int life = 5;
    int attack = 0;
    int defense = 0;
    int key = 0;
    Item inventory[coutMaxItens];
    int inventoryCount = 0;
    int bossMap = 0;
    bool inBossRoom = false;
    bool bossRoomFirstEntry = true;
    int xp = 0;
    int level = 0;
};

struct Npc {
    bool healing;
    int life;
    int attack;
    int defense;
    bool isBoss;
};

struct Combatant {
    string name;
    int initiative;
    Npc npc;
    Player player;
    bool isNpc;
};

struct typeNpc {
    string type;
    int bonusLife;
    int bonusAttack;
    int bonusDefense;
    string color;
};

typeNpc typesNpc[3] = {
    {"Guerreiro",2,1,2,"\033[0;32m"},
    {"Mago",0,3,0,"\033[0;34m"},
    {"Ladino",1,2,0,"\033[0;33m"},
};

string padRight(const string& text) {
    if (text.length() >= baseLength) return text.substr(0, baseLength);
    return text + string(baseLength - text.length(), ' ') + "\n";
}

string titleGame = "ROGUELIKE MENU";

void clearConsole() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

string pagina1 =
"  Voce e Leon, um jovem nobre, cavaleiro da luz, e fiel devoto do deus Helm.\n"
"\n"
"  Na manha dourada de um dia calmo, Voce aproveitava um piquenique nas colinas\n"
"  verdejantes da Planicie de Miralys, ao lado da estonteante princesa Lina.\n"
"\n"
"  Era um momento perfeito. As flores dancavam com o vento, os passaros cantavam,\n"
"  e o sorriso dela fazia o tempo desacelerar...\n"
"\n"
"  Mas a paz foi traida pela escuridao.\n"
"\n"
"  De repente, uma dor aguda perfura suas costas. Suas pernas falham, seu corpo cede.\n"
"  Seus olhos, ainda abertos, veem sombras se aproximando... cinco drows, elfos\n"
"  sombrios das profundezas. Rapidos como serpentes, eles imobilizam Voce e levam\n"
"  Lina desesperada para as sombras.\n"
"\n"
"  Voce tenta gritar... mas o veneno nao permite. Entao... tudo se apaga.\n";

string pagina2 =
"  Horas se passam... e Voce desperta.\n"
"\n"
"  Seu corpo ainda treme, mas seu espirito queima com furia. Voce se levanta,\n"
"  cambaleando, e ve pegadas arrastadas seguindo ate uma abertura escura entre\n"
"  rochas a entrada de uma caverna ancestral.\n"
"\n"
"  Acima do portal, um simbolo sagrado de Helm brilha fracamente,\n"
"  como se reconhecesse seu juramento silencioso.\n"
"\n"
"  Voce se ajoelha, coloca a mao sobre o peito, e murmura:\n"
"\n"
"     \"Pela luz de Helm, por minha honra e nobreza, eu juro:\n"
"      vou resgatar Lina, mesmo que isso custe minha propria vida.\"\n"
"\n"
"  Uma brisa quente toca seu rosto...\n"
"  Voce sente que Helm esta com Voce.\n"
"\n"
"  E assim, sem hesitar, Voce adentra a escuridao,\n"
"  onde Lina aguarda... e o destino sera decidido.\n";

string buffCombat[3] = {
    "Vida",
    "Ataque",
    "Defesa"
};

const int countEnemies = 1;
Npc enemies[countEnemies];
