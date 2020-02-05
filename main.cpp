#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>
#include <random>
#include "Predator.h"
#include "Prey.h"

const int DWW = 1920; // display window width, in pixels.
const int DWH = 1080; //display window height, in pixels.
const int WSL = 10; //size of both world dimensions, in units-area.
const int WORLDS = 10; //each thread runs a world, with 5 worlds per row in 2 rows.


using namespace sf;
using namespace std;

void runWorld(int* world) {

}

int main() {
    int ua = 38; //size of each unit-area side, in pixels (square this number to get unit-area in pixels).
    vector<vector<int>> worlds;
    for(int w = 0; w < WORLDS; w++) {
        vector<int> world(WSL * WSL, 0);
        worlds.push_back(world);
    }
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
	
	return 0;
}