#include <SFML/Graphics.hpp>
#include <thread>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include "Predator.h"
#include "Prey.h"


const int DWW = 1000; // display window width, in pixels.
const int DWH = 1000; //display window height, in pixels.
const int WSL = 10; //size of both world dimensions, in units-area.
const int WORLDS = 10; //each thread runs a world


using namespace sf;
using namespace std;

void displayBestOfGen(int generation) {
    RenderWindow window(VideoMode(DWW, DWH), "SFML works!");

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color(255, 50, 150, 255)); //Clear tem de ser chamado após cada update ao renderer, sn está a desenhar por cima do anterior em vez de o substituir. 
        window.display();
    }
}

void runWorld(vector<int> *world, RenderWindow *window) {
    unsigned WorldSeed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(WorldSeed);
    vector<int> predatorBodyCoords;
    predatorBodyCoords.push_back(34);
    predatorBodyCoords.push_back(35);
    predatorBodyCoords.push_back(44);
    predatorBodyCoords.push_back(45);
    Predator predator(predatorBodyCoords);
    vector<int> preyBodyCoords;
    preyBodyCoords.push_back(18);
    preyBodyCoords.push_back(19);
    Prey prey1(preyBodyCoords);
    preyBodyCoords[0] += 50;
    preyBodyCoords[1] += 50;
    Prey prey2(preyBodyCoords);
    
}

int main() {
    int ua = 100; //size of each unit-area side, in pixels (square this number to get unit-area in pixels).
    vector<vector<int>> worlds;
    for(int w = 0; w < WORLDS; w++) {
        vector<int> world(WSL * WSL, 0); //each world is represented by a single vector despite technically being a m * n matrix. The first n elements represent all the columns of the first row, and so on and so on.
        worlds.push_back(world);
    }	
	return 0;
}