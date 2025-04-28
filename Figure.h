#pragma once
#include "mcigraph.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;


void drawStartpage(){
        draw_rect(0,0,64*16,48*16,false,0,0,0);
        draw_image("START_PAGE.bmp",40,30);
}


void drawGameOver(){
    draw_rect(0,0, 64*16,48*16,false,0,0,0);
    draw_image("GAME_OVER.bmp",40,30);
}

void drawGameWin(){
    draw_rect(0,0, 64*16,48*16,false,0,0,0);
    draw_image("GAME_WIN.bmp",40,30);
}



/// Write something into the MAP or ACCESSIBLE array
class Mapwrite{
    public:
    void Field(int sizex, int sizey,  int *Map){
        for (int i = 0; i < sizex*sizey; i++){
            Map[i] = 11;
        }
    }




    // Write the Flowers
        /* Can place Flowers on the whole field therefore must
        be before the Lake, to not overwrite the Lake! */
    void Flowers(int sizex, int sizey,  int *Map){
        for (int x = 0; x < sizex; x++){
            for(int y = 0; y < sizey; y++){
                if(rand()%50 == 0)
                    Map[y * sizex + x] = 1; //Flowers are "1" in map array
            }
        }
    }

    // Write the Lake into the MAP and the ACCESSIBLE array
    void Lake(int x1, int y1, int x2, int y2, int *Map, int *access){
        for (int y = y1; y < y2; y++) {
    		for (int x = x1; x < x2; x++) {
      			Map[y * 64 + x] = 2;        // Water is "2" in map array
                access[y * 64 + x] = 1;
    		}
        }
        // Write the edges and corners within the specified boundaries.
        for (int y = y1; y < y2; y++) {
            for (int x = x1; x < x2; x++) {
                if (x == x1 && y == y1) {
                    Map[y * 64 + x] = 10;   // Upper left corner = "10"
                } else if (x == x1 && y == y2 - 1) {
                    Map[y * 64 + x] = 9;    // Lower left corner = "9"
                } else if (x == x2 - 1 && y == y2 - 1) {
                    Map[y * 64 + x] = 8;    // Lower right corner = "8"
                } else if (x == x2 - 1 && y == y1) {
                    Map[y * 64 + x] = 7;    // Upper right corner = "7"
                } else if (x == x1) {
                    Map[y * 64 + x] = 6;    // Left edge = "6"
                } else if (y == y2 - 1) {
                    Map[y * 64 + x] = 5;    // Lower edge = "5"
                } else if (x == x2 - 1) {
                    Map[y * 64 + x] = 4;    // Right edge = "4"
                } else if (y == y1) {
                    Map[y * 64 + x] = 3;    // Upper edge = "3"
                }
            }
        }
    }


    // Create the house
    void house(int xMid, int yMid, int *Map, int *access) {
        // Calculate the top-left corner coordinates based on the middle x coordinate
        int width = 7;   // Width of the house
        int height = 4;  // Height of the house
        int x1 = xMid - width / 2;
        int y1 = yMid - height+1;  // +1 ... shift by one faulty block

        // Calculate the coordinates of the bottom-right corner of the house
        int x2 = x1 + width;
        int y2 = y1 + height;

        for (int y = y1; y < y2; y++) {
            for (int x = x1; x < x2; x++) {
                Map[y * 64 + x] = 11;
                access[y * 64 + x] = 1;
            }
        }
        // Draw a door that is accessible
        Map[yMid * 64 + xMid] = 12;
        access[yMid * 64 + xMid] = 0;
        // Draw the windows
        Map[(yMid-1)*64 + xMid-2] = 13;
        Map[(yMid-1)*64 + xMid+2] = 13;
        // Draw the roof
        for (int x = xMid - 4; x < xMid + 5; x++){
            for (int y = yMid - 4; y < yMid - 3; y++){
            Map[y * 64 + x] = 14;
            access[y * 64 + x] = 1;
            }
        }
        for (int x = xMid - 3; x < xMid + 4; x++){
            for (int y = yMid - 5; y < yMid - 4; y++){
            Map[y * 64 + x] = 14;
            access[y * 64 + x] = 1;
            }
        }
    }



    // Function to draw the Map
        /* Must be in the Game loop to always be refreshed.*/
    void draw_map(int *Map) {
        for (int y = 0; y < 48; y++) {
            for (int x = 0; x < 64; x++) {
                if (Map[y * 64 + x] == 0)
                    draw_image("tiles/3_16.bmp", x * 16, y * 16);   // grass
                else if (Map[y * 64 + x] == 1)
                    draw_image("tiles/3_7.bmp", x * 16, y * 16);    // flowers
                else if (Map[y * 64 + x] == 2)
                    draw_image("tiles/0_0.bmp", x * 16, y * 16);    // water

                // This draws the edges around the lake
                else if (Map[y * 64 + x] == 3)
                    draw_image("tiles/3_0.bmp", x * 16, y * 16);   // upper edge
                else if (Map[y * 64 + x] == 4)
                    draw_image("tiles/4_1.bmp", x * 16, y * 16);   // Right edge
                else if (Map[y * 64 + x] == 5)
                    draw_image("tiles/3_2.bmp", x * 16, y * 16);   // Lower edge
                else if (Map[y * 64 + x] == 6)
                    draw_image("tiles/2_1.bmp", x * 16, y * 16);   // Left edge

                else if (Map[y * 64 + x] == 7)
                    draw_image("tiles/4_0.bmp", x * 16, y * 16);   // Upper right corner
                else if (Map[y * 64 + x] == 8)
                    draw_image("tiles/4_2.bmp", x * 16, y * 16);   // Lower right corner
                else if (Map[y * 64 + x] == 9)
                    draw_image("tiles/2_2.bmp", x * 16, y * 16);   // Lower left corner
                else if (Map[y * 64 + x] == 10)
                    draw_image("tiles/2_0.bmp", x * 16, y * 16);   // Upper left corner

                else if (Map[y * 64 + x] == 11)
                    draw_image("tiles/7_5.bmp", x * 16, y * 16);   // Wall house
                else if (Map[y * 64 + x] == 12)
                    draw_image("tiles/35_0.bmp", x * 16, y * 16);   // door house
                else if (Map[y * 64 + x] == 13)
                    draw_image("tiles/43_5.bmp", x * 16, y * 16);   // window house
                else if (Map[y * 64 + x] == 14)
                    draw_image("tiles/47_13.bmp", x * 16, y * 16);   // roof house
            }
        }
    }
};


/// Definition of the BASE CLASS
class Figure {
protected:
    int x, y;
    string img;
public:
    // Default constructor:
        Figure(){
        x = 0;
        y = 0;
        }

    // Concrete constructor:
        Figure(int xVal, int yVal, const string& imgVal){        // string& imgVal is a reference to a string
        x = xVal;
        y = yVal;
        img = imgVal;
        }

        // Getter functions
        /* The const is a indicator that shows that it does't modify the function.
        It is a good practice in C++. */
        int getX() const { return x; }
        int getY() const { return y; }
        string getImg() const { return img; }



    // Setter functions
        void setX(int xVal) { x = xVal; }
        void setY(int yVal) { y = yVal; }
        void setImg(const string& imgVal) { img = imgVal; }



    // Function to draw the figure
        void draw() const {draw_image(img, x * 16, y * 16); }

    // Functions to move the figure
        /*Here the condition, if a field is accessible, is implemented. */
        void moveLeft(int* access)   {x--; if (x < 0 || access[y * 64 + x] == 1) x++;}
        void moveRight(int* access)  {x++; if (x > 63 || access[y * 64 + x] == 1) x--;}
        void moveUp(int* access)     {y--; if (y < 0 || access[y * 64 + x] == 1) y++;}
        void moveDown(int* access)   {y++; if (y > 47 || access[y * 64 + x] == 1) y--;}

    // Member function to check collision with another figure
        bool checkCollision(const Figure &other) const {
            return (x == other.x && y == other.y);
        }
};



/// Function of the TYPE FIGURE
Figure randomFigure(const string& img, int *access) {
    Figure f;
    int x, y;

    // Loop to create the monsters but not on the not Accessible fields
    while (true) {
        x = rand() % 64;
        y = rand() % 48;

        if (access[y * 64 + x] == 0) {
            f.setX(x);  // These call the SET functions defined in Figure
            f.setY(y);
            f.setImg(img);
            break; // Zugängliches Feld gefunden, beende die Schleife
        }
    }
    return f;
}



/// Definition of the DERIVED CLASS PLAYER
class Player : public Figure {
private:
    int Health;

public:
    // Concrete constructor
        /* In this constructor first the Base Class is initialized and
        second the Member Variable is initialized! */
    Player(int xVa, int yVa, const string& imgVa)
        : Figure(xVa, yVa, imgVa), Health(100) {}

    // Getter function
    int getHealth() const { return Health; }

    // Function to move the player
    void checkMovement(int left, int right, int up, int down, int* accessible) {
        if (left)
            moveLeft(accessible);
        if (right)
            moveRight(accessible);
        if (up)
            moveUp(accessible);
        if (down)
            moveDown(accessible);
    }

    // Function that draws the Player and the Healthbar
    void drawPlayer() {
        Figure::draw();     // First the Figure is drawn
        int barLength = (int)(16 * (Health / 100.0));  // Calculate the bar length as a percentage of full length
        int NbarLength = (int)(16 );
        draw_rect(x*16, y*16 - 4, barLength, 2, false, 0 ,255,0);   // Here a rectangle with the width of 2 and the length of Healthbar is drawn
        // Draw a extra Healthbar in the top right corner
        draw_rect(58*16, 2*16, NbarLength *5, 4,false, 255, 0, 0);  // Red bar for lost health. must be under/before the greed bar function!
        draw_rect(58*16, 2*16, barLength  *5, 4,false, 0, 255, 0);
    }


    // Function to decrease the Health of a Player
    void decreaseHealth (int armour){
        Health = Health-armour;
    }
};



/// Definition of the DERIVED CLASS MONSTER
class Monster : public Figure{
private:
    int HealthM;
    bool dead;

public:
    // Concrete constructor
        /* In this constructor first the Base Class is initialized and
        second the Member Variable is initialized! */
    Monster(int xVa, int yVa, const string& imgVa)
        : Figure(xVa, yVa, imgVa),HealthM(100), dead(false) {}

    // Getter functions
        bool isDead() const { return dead; }
        int getHealthM() const { return HealthM; }

    // Draw the Monster
    void drawMonster() {
        Figure::draw();     // First the Figure is drawn
        int barLength = (int)(16 * (HealthM / 100.0));  // Calculate the bar length as a percentage of full length
        draw_rect(x*16, y*16 - 4, barLength, 2, false, 255,0,0);   // Here a rectangle with the width of 2 and the length of Healthbar is drawn
    }

    // Function to decrease the Health of a Player
    void decreaseHealthM (int damage){
        HealthM = HealthM - damage;
    }

    // Function that move the monsters randomly
    void moveRandom(int* accessible){
        if (rand()%4 == 0){     // If statement is used to update the movement of monster with a probability of 25%
            int direction = rand() % 4;     // This creates random value from 0 to 3. Used for the direction.
            // Move into direction given by direction
            switch(direction){
                case 0:
                    moveLeft(accessible);
                    break;
                case 1:
                    moveRight(accessible);
                    break;
                case 2:
                    moveUp(accessible);
                    break;
                case 3:
                    moveDown(accessible);
                    break;
            }
        }
    }

    // Function that destroys the monsters that have a Health of 0
    void destroyDeadMonsters(vector<Monster>& monsters) {
        // Create an iterator to traverse the vector
        auto it = monsters.begin();

        while (it != monsters.end()) {
            if (it->getHealthM() <= 0) {    // is the health of the current monster pointed to by the iterator less or equal to 0
                // Remove the monster from the vector
                it = monsters.erase(it); // takes iterator and removes element at that position
            } else {
                ++it;
            }
        }
    }
};


/// Definition of the DERIVED CLASS OBJECT
class Object : public Figure {
protected:
    bool _collectible;

public:
    // Constructor to initialize Object
    Object(int xVal, int yVal, const string& imgVal, bool isCollectible)
        : Figure(xVal, yVal, imgVal), _collectible(isCollectible) {}

    // Getter function
    bool isCollectible() { return _collectible; }

    void drawPU() {
        Figure::draw();     // First the Figure is drawn
    }

};

