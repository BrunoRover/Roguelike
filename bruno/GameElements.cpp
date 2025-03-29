#pragma once

struct GameElements {
    char person = char(36);  
    char wall = char(219);   
    char path = ' '; 
    char door = 'D';
    char dark = '.'; 
    
    
    char key = char(42); //*
    char potion = char(38); //&
    char trap = char(63); //?

};
struct ElementPerson{
    int life = 5; 
    int attack = 1;
    int defense = 0;
    int key = 0;
};






// aquelaCarinha = "( ͡° ͜ʖ ͡°)"; 
// dead = "×͜×";

