#include "mcigraph.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include "Figure.h"
using namespace std;

enum GState {
    GAME_OVER,
    GAME_WIN
};

bool isMonstersEmpty(const vector<Monster> &mo){
    return mo.empty();
}
bool checkMonstersExisted(const vector<Monster>& monsters) {
    // Check if the monsters vector has any members
    return !monsters.empty();
}

/// MAIN
int main(int argc, char* argv[]) {
    srand(time(0));     // srand is initialized with time() to generate a random-like value
    set_delay(50);

    bool counterStarted = false;
    int gameCounter = 0;
    bool counterRunning = false;
    bool monstersExisted = false;
    GState gameOUT; // Initialize the outcome of the game


    // Definition of the MAP
        const int size_x = 64;
        const int size_y = 48;
        int currentMAP = 1;
        int *map;
        int *accessible;
        // Define MAP 1
        int map1[size_x*size_y] = {0};
        int accessible1[size_x*size_y] = {0};
        // Define MAP 2
        int map2[size_x*size_y] = {11};
        int accessible2[size_x*size_y] = {0};

    // Write into the map array
        int house_enterX = 45;
        int house_enterY = 15;
        Mapwrite mapwriter; // Instance of Mapwrite
        mapwriter.Flowers(size_x,size_y, map1);
        mapwriter.Lake(5,25,30,35,map1,accessible1);
        mapwriter.house(house_enterX,house_enterY,map1, accessible1);
        mapwriter.Field(size_x, size_y, map2);

    // Monster Definitions
        vector<Monster> monsters;
        const int maxMonsters = 30; // Maximum number of monsters
        int numMonsters = 0;       // Current number of monsters
        bool maxMonstersReached = false;

    // Player Definition
        Player c(32, 24, "tiles/char1.bmp");
        int attackStrengthDefault = 20;
        int attackStrength = attackStrengthDefault;
        bool isAttacking = false;  // Flag to track if the player is attacking

    // Power Ups
        vector<Object> powerUp;
        const int maxPowerUp = 4;    // Total number of power ups
        int numPowerUp = 0;     // Current number of power ups
        int powerAttack = 100;



/// START PAGE LOOP
while(running()){
    drawStartpage();
    present();
    if(is_pressed(SDL_SCANCODE_S)){
        break;
    } else if(is_pressed(SDL_SCANCODE_E)){
        return 0;
    }
}


/// GAME LOOP
  while (running()) {
        if (currentMAP == 1){
            map = map1;
            accessible = accessible1;
        } else if (currentMAP == 2){
            map = map2;
            accessible = accessible2;
        }

// Game Counter
    if (counterStarted){
        gameCounter++;
    }

// Stop the Counter - PowerUps for higher AttackStrength
    if( gameCounter >= 120){    // Time that a power of is
        counterStarted = false;
        gameCounter = 0;
        attackStrength = attackStrengthDefault;
    }

// Draw the MAP
    mapwriter.draw_map(map);

// Move the character
    int left = is_pressed(KEY_LEFT);
    int right = is_pressed(KEY_RIGHT);
    int up = is_pressed(KEY_UP);
    int down = is_pressed(KEY_DOWN);
    c.checkMovement(left, right, up, down, accessible);


// Create the monsters
    if (maxMonstersReached == false){
        if (numMonsters < maxMonsters && rand() % 8 == 0) {
            // Function of Random Monsters
            Figure monsterFigure = randomFigure("tiles/monster.bmp",accessible);
            /* This Function generates a random position, this is saved in the
             object monsterFigure */
            monsters.emplace_back(monsterFigure.getX(), monsterFigure.getY(), monsterFigure.getImg());
            numMonsters++;  // sets the monsters counter
            if (numMonsters == maxMonsters){maxMonstersReached = true;}
        }
    } else {/* DON'T GENERATE MONSTERS ANY MORE, WHEN THE MAXUM AMMOUNT OF MONSTERS IS REACHED*/ }
    monstersExisted = checkMonstersExisted(monsters);    // Sets the value of the monostersExisted to true if the vector istn't empty

// Create Power Ups
    if (numPowerUp < maxPowerUp && rand() % 20 == 0){
        Figure powerupfig = randomFigure("tiles/14_7.bmp",accessible);  // Creates a PowerUp of the figure type with the function randomFigure

        powerUp.emplace_back(powerupfig.getX(),powerupfig.getY(),powerupfig.getImg(),true); //
        numPowerUp++;
    }

// Draw Power Ups
    for (auto& pu : powerUp){   // Cycle through the Vector powerUp
        pu.drawPU();
    }

// power up collision with Player
    auto it = powerUp.begin();  // initialize an interator
    while (it != powerUp.end()) {   // iterate through the vector if the end is not reached
        if (it->isCollectible() && c.checkCollision(*it)) { // Check if the current power-up of the iterator is collectible and collides with the player
            it = powerUp.erase(it);  // Erase and update the iterator
            numPowerUp--;

            // Update the Attack Strength when the player picks up the power up
            attackStrength = powerAttack;

            // Start the counter
            counterStarted = true;
        } else {
            ++it;  // Move to the next element
        }
    }

    // Move and draw monsters
    for (auto& monster : monsters) {
        monster.drawMonster();
        monster.moveRandom(accessible);
    }

    // Check collision with monsters
    for (auto& monster : monsters) {
        if (c.checkCollision(monster)) {
            c.decreaseHealth(5);
        }
    }

// Attacking of the monsters
    int playerX = c.getX();  // Get player's X position
    int playerY = c.getY();  // Get player's Y position
    int attackDirX = 0;
    int attackDirY = 0;
    const int maxBeamRange = 10;

    if(is_pressed(KEY_LEFT)){
        attackDirX = -1;    // beam left
    }   else if (is_pressed(KEY_RIGHT)){
        attackDirX = 1;     // beam right
    }   else if (is_pressed(KEY_UP)){
        attackDirY = -1;    // beam up
    }   else if (is_pressed(KEY_DOWN)){
        attackDirY = 1;     // beam down
    }

    if (is_pressed(KEY_SPACE) && !isAttacking) {
        isAttacking = true;  // Set the attacking flag
        // Draw the attack beam in the desired direction

        for (int i = 1; i <= maxBeamRange; i++) {
            int beamX = playerX + (i * attackDirX);
            int beamY = playerY + (i * attackDirY);

            // Check if the beam hits an obstacle (e.g., a wall or edge of the map)
            if (beamX < 0 || beamX >= 64 || beamY < 0 || beamY >= 48 || map[beamY * 64 + beamX] != 0) {
                break;  // Stop the beam if out of bounds or hits an obstacle
            }

            // Draw a dot at (beamX, playerY) to represent the attack

            if (attackStrength != powerAttack){
                draw_rect(beamX * 16, beamY * 16, 3, 3, false, 0, 0, 255);  // Blue dot
            }   else {
                draw_rect(beamX * 16, beamY * 16, 3, 3, false, 255, 0, 0);  // Red dot
            }

            // Check if the attack hit any monsters
            for (auto& monster : monsters) {
                if (beamX == monster.getX() && beamY == monster.getY()) {
                    // Monster is hit, decrease its health by attackDamage
                    monster.decreaseHealthM(attackStrength);
                    monster.destroyDeadMonsters(monsters);
                }
            }
        }
        // Reset the attacking flag
        isAttacking = false;
    }

    // If Player enters the house change Map
    if (c.getX() == house_enterX && c.getY() == house_enterY) {
        currentMAP = 2;
    }

    // If health of the Player get smaller than 0 -- GAME OVER
    if (c.getHealth() <= 0){
        gameOUT = GAME_OVER;
        break;
    } else if (isMonstersEmpty(monsters) && monstersExisted == true){ // If the vector monsters is empty you win
        gameOUT = GAME_WIN;
        break;
    }

    // Draw the Player
    c.drawPlayer();

    // Instance for SDL window
    present();
}

/// END PAGE LOOP
while(running()){
    if(gameOUT == GAME_OVER){
        drawGameOver();
        present();
        if(is_pressed(SDL_SCANCODE_RETURN)){
            return 0;
        }
    }
    if (gameOUT == GAME_WIN ){
        drawGameWin();
        present();
        if(is_pressed(SDL_SCANCODE_RETURN)){
            return 0;
        }
    }
}

}
