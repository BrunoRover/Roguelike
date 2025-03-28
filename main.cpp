#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

// Definition of the button array
const string buttons[4] = {"ATTACK", "DEFEND", "ITEM", "ESCAPE"};

// Structure to store each combatant
struct Combatant {
    string name;
    int initiative;
};

// List of combatants (player + enemies)
vector<Combatant> combatants;

// Index of the combatant currently taking action
int currentTurn = 0;

// Function to set the CMD to full screen properly
void setFullScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set a buffer large enough to allow full screen
    COORD bufferSize = {120, 40};  // Defines a buffer of 120 columns x 40 rows
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Adjust display mode to full screen
    SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, 0);
}

// Function to clear the screen without flickering
void clearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);
}

// Function to generate initiative for a combatant
int generateInitiative() {
    return rand() % 10 + 1; // Number from 1 to 10
}

// Function to compare initiatives (to sort from highest to lowest)
bool compareInitiative(Combatant a, Combatant b) {
    return a.initiative > b.initiative;
}

// Function to initialize initiatives
void initializeInitiatives() {
    srand(time(0)); // Initialize the random number generator

    combatants.push_back({"Player", generateInitiative()});
    combatants.push_back({"Enemy 1", generateInitiative()});
    combatants.push_back({"Enemy 2", generateInitiative()}); // You can add more enemies

    // Sort combatants by initiative (highest to lowest)
    sort(combatants.begin(), combatants.end(), compareInitiative);
}

// Function to display the full interface (initiative + buttons)
void displayCombatInterface(int selectedOption) {
    clearConsole();

    // Display initiative list vertically
    cout << "==================== INITIATIVE ====================" << endl;
    for (size_t i = 0; i < combatants.size(); i++) {
        if (i == currentTurn) {
            cout << "> " << combatants[i].name << " (Init: " << combatants[i].initiative << ")" << endl;
        } else {
            cout << "  " << combatants[i].name << " (Init: " << combatants[i].initiative << ")" << endl;
        }
    }
    cout << "===================================================" << endl;

    // Display combat menu
    cout << "Use A (left) and D (right) to navigate | ENTER to confirm\n" << endl;

    for (int i = 0; i < 4; i++) {
        if (i == selectedOption) {
            cout << "[>>" << buttons[i] << "<<]   "; // Active button
        } else {
            cout << "[ " << buttons[i] << " ]   ";  // Normal button
        }
    }
    cout << "\n=================================================" << endl;
}

// Function to manage player choice
void combatMenu() {
    int selectedOption = 0; // 0 = Attack, 1 = Defend, 2 = Item, 3 = Escape
    char key;

    displayCombatInterface(selectedOption);

    while (true) {
        key = _getch(); // Captures keyboard input without needing Enter

        if (key == 'a' || key == 'A') { // Move left
            if (selectedOption > 0) selectedOption--;
        } else if (key == 'd' || key == 'D') { // Move right
            if (selectedOption < 3) selectedOption++;
        } else if (key == '\r') { // Enter key pressed
            clearConsole();
            cout << "=================================================" << endl;
            cout << combatants[currentTurn].name << " chose " << buttons[selectedOption] << "!" << endl;
            cout << "=================================================" << endl;
            Sleep(2000);
            break;
        }

        // Update interface without screen flickering
        displayCombatInterface(selectedOption);
    }
}

int main() {
    setFullScreen(); // Now properly sets full screen
    initializeInitiatives(); // Sets the initiatives
    combatMenu(); // Starts the combat menu
    return 0;
}
