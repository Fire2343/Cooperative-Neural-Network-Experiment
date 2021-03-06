#include <SFML/Graphics.hpp>
#include <thread>
#include <fstream>
#include <string>
#include <sstream> 
#include <iostream>
#include <random>
#include <chrono>
#include "Predator.h"
#include "Prey.h"
#include <Windows.h>


const int DWW = 1000; // display window width, in pixels.
const int DWH = 1000; //display window height, in pixels.
const int WSL = 10; //size of both world dimensions, in units-area.
const int WORLDS = 4; //each thread runs a world


using namespace sf;
using namespace std;

vector<int> convertToXY(int coord) {
    vector<int> xycoords;
    int x;
    int y;
    if (coord / 10 > 0) {
        y = int(coord / 10);
        x = 0;
        for (int c = y * 10; c < coord; c++) {
            x++;
        }
    }
    else {
        x = coord;
        y = 0;
    }
    xycoords.push_back(x);
    xycoords.push_back(y);
    return xycoords;
}

bool insideBounds(vector<int> coords, int move) {
    if (abs(move) == 1) {
        if (convertToXY(coords[1] * 10 + coords[0])[1] != convertToXY(coords[1] * 10 + coords[0] - move)[1]) {            
            return false;
        }
    }
    else {
        if (convertToXY(coords[1] * 10 + coords[0])[1] == convertToXY(coords[1] * 10 + coords[0] - move)[1]) {
            return false;
        }
        if (abs(convertToXY(coords[1] * 10 + coords[0])[0] - convertToXY(coords[1] * 10 + coords[0] - move)[0]) > 1) {
            return false;
        }
    }
    if (coords[0] < 0 || coords[0] > 9) {
        return false;
    }
    if (coords[1] < 0 || coords[1] > 9) {
        return false;
    }
    return true;
}

void displayBestOfGen(int ua) {
    RenderWindow window(VideoMode(DWW, DWH), "display");

    int m = 0;
    ifstream movementData;
    movementData.open("movementData.txt");
    string data;
    while (window.isOpen()){
        //cout << data[m] << endl;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (!movementData.eof()) {
            getline(movementData, data);
        }
        if (m >= data.size()) {
            break;
        }
        if (data[m] == "|"[0]) {
           m++;
           window.display();
           window.clear(Color(0, 255, 255, 255));
           Sleep(2000);
        }
        else {
           if (data[m] == "+"[0]) {
               m++;
           }
           else {
               int numberSize = 1;
               if (data[m + 1] != "+"[0]) {
                  numberSize++;
               }
               stringstream converter(data.substr(m, numberSize));
               int converted;
               converter >> converted;
               vector<int> coordsXY = convertToXY(converted);
               RectangleShape bodyPart(Vector2f(ua, ua));              
               bodyPart.setPosition(coordsXY[0] * ua, coordsXY[1] * ua);
               bodyPart.setFillColor(Color(0, 255, 0, 255));
               window.draw(bodyPart);
               m += numberSize;
           }
       }
    }
} 


void mutateNeuralNet(vector<vector<vector<double>>> *weights) {
    unsigned WorldSeed = chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 generator(WorldSeed);
    uniform_int_distribution<int> distribution(0, 100);
    uniform_real_distribution<double> vdistribution(-0.10, 0.10);
    uniform_real_distribution<double> nlvdistribution(-0.20, 0.20);
    for (int l = 0; l < (*weights).size(); l++) { //percorrer camadas
        for (int n = 0; n < (*weights)[l].size(); n++) { //percorrer neuronios da camada
            if (distribution(generator) == 1 && l < (*weights).size() - 1 && (*weights)[l].size() <= 300) { //oportunidade de muta��o para cria��o de novos neur�nios
                vector<double> newNeuron = (*weights)[l][n];
                (*weights)[l].push_back(newNeuron);
                for (int nln = 0; nln < (*weights)[l + 1].size(); nln++) {
                    (*weights)[l + 1][nln].push_back(nlvdistribution(generator));
                }
            }
            for (int w = 0; w < (*weights)[l][n].size(); w++) { // percorrer pesos do neuronio
                if (distribution(generator) == 1) { // oportunidade de muta��o para alterar os pesos nos neur�nio 
                    (*weights)[l][n][w] += vdistribution(generator);
                }
            }
        }
        if (distribution(generator) == 1 && l == (*weights).size() - 1 && (*weights).size() <= 4) { //oportunidade de muta�ao de novas camadas
            vector<vector<double>> newLayer;
            vector<double> newLayerWeights;
            uniform_int_distribution<int> nNdistribution(-(int((*weights)[l].size() / 2)), 0);
            for (int n = 0; n < (*weights)[l].size() + nNdistribution(generator); n++) {
                for (int w = 0; w < (*weights)[l].size(); w++) {
                    newLayerWeights.push_back(nlvdistribution(generator));
                }
                newLayer.push_back(newLayerWeights);
                newLayerWeights.clear();
            }
            (*weights).push_back(newLayer);
        }
    }
}
void generateInitialGeneticMemory(vector<double> *world) {
    ofstream geneticMemory("neuralNetValues.txt");
    geneticMemory << "|";
    for (int n = 0; n < 4; n++) {
        for (int i = 0; i < (*world).size() + 2; i++) {
            geneticMemory << "0.00+";
        }
        geneticMemory << "|";
    }
    geneticMemory << "|";
    geneticMemory.close();
}

bool checkAdjacency(vector<int> *targetBodyParts, int seekerBodyPart) {
    for (int i = 0; i < (*targetBodyParts).size(); i++) {
        int d = abs((*targetBodyParts)[i] - seekerBodyPart);
        switch (d) 
        {
            case 11:
                if (convertToXY((*targetBodyParts)[i])[1] != convertToXY(seekerBodyPart)[1]) {
                    return true;
                }
                else {
                    break;
                }
            case 10:
                if (convertToXY((*targetBodyParts)[i])[1] != convertToXY(seekerBodyPart)[1]) {
                    return true;
                }
                else {
                    break;
                }
            case 9:
                if (convertToXY((*targetBodyParts)[i])[1] != convertToXY(seekerBodyPart)[1]) {
                    return true;
                }
                else {
                    break;
                }
            case 1:
                return true;
            default:
                break;
        }
    }
    return false;
}

bool belongsToSelf(int c, vector<int>* bodyParts) {
    for (int i = 0; i < (*bodyParts).size(); i++) {
        if ((*bodyParts)[i] == c) {
            return true;
        }
    }
    return false;
}

void runWorld(vector<double> *world, vector<int> *fitnessValues, int worldNumber, vector<vector<vector<vector<double>>>> *worldNeuralWeights, vector<vector<int>> *worldsMovementData) {
    vector<int> predatorBodyCoords;
    predatorBodyCoords.push_back(31);
    predatorBodyCoords.push_back(32);
    predatorBodyCoords.push_back(41);
    predatorBodyCoords.push_back(42);
    (*world)[31] = 1.0;
    (*world)[32] = 1.0;
    (*worldsMovementData)[worldNumber].push_back(31);
    (*worldsMovementData)[worldNumber].push_back(32);
    (*world)[41] = 1.0;
    (*world)[42] = 1.0;
    (*worldsMovementData)[worldNumber].push_back(41);
    (*worldsMovementData)[worldNumber].push_back(42);
    Predator predator(predatorBodyCoords);
    vector<int> preyBodyCoords;
    preyBodyCoords.push_back(18);
    preyBodyCoords.push_back(19);
    vector<vector<vector<double>>> weights;
    ifstream geneticMemory;
    string data;
    geneticMemory.open("neuralNetValues.txt");
    while (!geneticMemory.eof()) {
        vector<double> neuronWeights;
        vector<vector<double>> layerWeights;
        getline(geneticMemory, data);
        char c;
        for (int i = 0; i < data.size(); i++) {
            c = data[i];
            if (c != ("|")[0] && c != ("+")[0]) {
                int sizeNumber = 0;
                for (int si = i; si < 100000000; si++) {
                    //cout << si << endl;
                    if (data[si] == "+"[0]) { 
                        break;
                    }
                    sizeNumber++;
                }
                stringstream converter(data.substr(i, sizeNumber));
                double converted;
                converter >> converted;
                //cout << sizeNumber << "sizeNumber" << worldNumber << endl;
                //cout << converted << "converted" << worldNumber << endl;
                i += sizeNumber;
                neuronWeights.push_back(converted);
            }
            else {
		         if(c == ("|")[0]) {
                    if (neuronWeights.size() > 0) {
                        layerWeights.push_back(neuronWeights);
                        neuronWeights.clear();
                    }
                    if (data[i + 1] == c) {
                        weights.push_back(layerWeights);
                        layerWeights.clear();
                    }
                 }
            }
        }
    }
    geneticMemory.close();
    mutateNeuralNet(&weights);
    int size = weights.size();
    Prey prey1(preyBodyCoords, weights);
    (*world)[18] = 1.0;
    (*world)[19] = 1.0;
    (*worldsMovementData)[worldNumber].push_back(18);
    (*worldsMovementData)[worldNumber].push_back(19);
    preyBodyCoords[0] += 50;
    preyBodyCoords[1] += 50;
    Prey prey2(preyBodyCoords, weights);
    (*world)[68] = 1.0;
    (*world)[69] = 1.0;
    (*worldsMovementData)[worldNumber].push_back(68);
    (*worldsMovementData)[worldNumber].push_back(69);
    bool flag1 = false;
    bool flag2 = false;
    bool touchingPrey = false;
    bool endGame = false;
    vector<int> occupiedCoords;
    vector <int> unnocupiedCoords;
    int moves = 0;
    for (int t = 0; t < 20; t++) {        
        vector<Prey> preys;
        preys.push_back(prey1);
        preys.push_back(prey2);
        flag1 = false;
        flag2 = false;
        (*worldsMovementData)[worldNumber].push_back(-1);
        unnocupiedCoords.clear();
        occupiedCoords.clear();
        //vector<int> occupiedCoords;
        //vector <int> unnocupiedCoords;
        vector<double> input1 = (*world);
        input1.insert(input1.end(), prey1.bodyCoords.begin(), prey1.bodyCoords.end());
        vector<double> input2 = (*world);
        input2.insert(input2.end(), prey2.bodyCoords.begin(), prey2.bodyCoords.end());
        vector<vector<double>> outputs;
        outputs.push_back(prey1.neuralNet(input1));
        outputs.push_back(prey2.neuralNet(input2));
        for (int osi = 0; osi < outputs.size(); osi++) {
            int toMove = 0;
            for (int oi = 0; oi < outputs[osi].size(); oi++) {
                if (outputs[osi][oi] > 0.50) {
                    switch (oi)
                    {
                        case 0:
                            toMove += -1;
                            break;
                        case 1:
                            toMove += 1;
                            break;
                        case 2:
                            toMove += -10;
                            break;
                        case 3:
                            toMove += 10;
                            break;
                        default:
                            break;
                    }
                }
            }
            bool dontMove = false;
            if (osi == 0) {
                vector<int> buffer;
                for (int bi = 0; bi < prey1.bodyCoords.size(); bi++) {  //Check to see if both preys are touching predator, so their roles may change.                    
                    buffer.push_back(prey1.bodyCoords[bi]);
                    if (insideBounds(convertToXY(prey1.bodyCoords[bi] + toMove), toMove) == false) {
                        dontMove = true;
                    }
                    else {
                        if ((*world)[prey1.bodyCoords[bi] + toMove] != 0.0) {
                            if (belongsToSelf(prey1.bodyCoords[bi] + toMove, &prey1.bodyCoords) == false) {
                                dontMove = true;
                            }
                        }
                    }
                }
                if (!dontMove) {
                    if (toMove != 0) {
                        moves++;
                    }
                    for (int bi = 0; bi < prey1.bodyCoords.size(); bi++) {
                        unnocupiedCoords.push_back(prey1.bodyCoords[bi]);
                        prey1.bodyCoords[bi] += toMove;
                        occupiedCoords.push_back(prey1.bodyCoords[bi]);
                        if (!flag1) {
                            flag1 = checkAdjacency(&predator.bodyCoords, prey1.bodyCoords[bi]);
                        }
                    }
                }
                else {
                    occupiedCoords.insert(occupiedCoords.end(), buffer.begin(), buffer.end());
                }
            }
            else {
                vector<int> buffer;
                for (int bi = 0; bi < prey2.bodyCoords.size(); bi++) {
                    buffer.push_back(prey2.bodyCoords[bi]);
                    if (insideBounds(convertToXY(prey2.bodyCoords[bi] + toMove), toMove) == false) {                        
                        dontMove = true;
                    }
                    else {
                        if ((*world)[prey2.bodyCoords[bi] + toMove] != 0.0) {
                            if (belongsToSelf(prey2.bodyCoords[bi] + toMove, &prey2.bodyCoords) == false) {
                                dontMove = true;
                            }
                        }
                    }
                }
                if (!dontMove) {
                    if (toMove != 0) {
                        moves++;
                    }
                    for (int bi = 0; bi < prey2.bodyCoords.size(); bi++) {
                        unnocupiedCoords.push_back(prey2.bodyCoords[bi]);
                        prey2.bodyCoords[bi] += toMove;
                        occupiedCoords.push_back(prey2.bodyCoords[bi]);
                        if (!flag2) {
                            flag2 = checkAdjacency(&predator.bodyCoords, prey2.bodyCoords[bi]);
                        }
                    }
                }
                else {
                    occupiedCoords.insert(occupiedCoords.end(), buffer.begin(), buffer.end());
                }
            }
        }
        if (flag1 && flag2) {
            (*fitnessValues)[worldNumber] += 10000 / ((t + 1) + moves);
            occupiedCoords.insert(occupiedCoords.end(), predator.bodyCoords.begin(), predator.bodyCoords.end());
            endGame = true;
        }
        if (!endGame) {
            bool dontMove = false;
            int predatorMove = predator.chooseMove(preys);
            vector<int> buffer;
            for (int bi = 0; bi < predator.bodyCoords.size(); bi++) {
                buffer.push_back(predator.bodyCoords[bi]);
                if (insideBounds(convertToXY(predator.bodyCoords[bi] + predatorMove), predatorMove) == false) {
                    dontMove = true;
                }
                else {
                    if ((*world)[predator.bodyCoords[bi] + predatorMove] != 0.0) {
                        if (belongsToSelf(predator.bodyCoords[bi] + predatorMove, &predator.bodyCoords) == false) {
                            dontMove = true;
                        }
                    }                
                }
            }
            if (!dontMove) {
                for (int bi = 0; bi < predator.bodyCoords.size(); bi++) {
                    unnocupiedCoords.push_back(predator.bodyCoords[bi]);
                    predator.bodyCoords[bi] += predatorMove;
                    occupiedCoords.push_back(predator.bodyCoords[bi]);
                    if (!touchingPrey) {
                        touchingPrey = checkAdjacency(&prey1.bodyCoords, predator.bodyCoords[bi]);
                        if (!touchingPrey) {
                           touchingPrey = checkAdjacency(&prey2.bodyCoords, predator.bodyCoords[bi]);
                        }
                    }
                }
            }
            else {
                occupiedCoords.insert(occupiedCoords.end(), buffer.begin(), buffer.end());
            }
            if (touchingPrey) {
                (*fitnessValues)[worldNumber] += -100 / (t + 1) + moves;
                endGame = true;
            }
        }
        for (int i = 0; i < unnocupiedCoords.size(); i++) {
            (*world)[unnocupiedCoords[i]] = 0.0;
        }
        for (int i = 0; i < occupiedCoords.size(); i++) {
            (*world)[occupiedCoords[i]] = 1.0;
            (*worldsMovementData)[worldNumber].push_back(occupiedCoords[i]);
        }
        if (endGame) {
            break;
        }
    }
    (*worldNeuralWeights)[worldNumber] = weights;
}

int main() {    
    int ua = 100; //size of each unit-area side, in pixels (square this number to get unit-area in pixels).
    vector<vector<double>> worlds;
    vector<vector<int>> worldsMovementData(WORLDS);
    vector<int> fitnessScores;
    vector<vector<vector<vector<double>>>> worldNeuralWeights(WORLDS);
    for(int w = 0; w < WORLDS; w++) {
        vector<double> world(WSL * WSL, 0.0); //each world is represented by a single vector despite technically being a m * n matrix. The first n elements represent all the columns of the first row, and so on and so on.
        worlds.push_back(world);
        fitnessScores.push_back(0);
    }
    //generateInitialGeneticMemory(&worlds[0]); // DO NOT ACTIVATE IF YOU DONT WANT TO START TRAINING PROCESS ALL OVER AGAIN!
    for (int g = 0; g < 1; g++) {
        cout << g << endl;
        thread first(runWorld, &worlds[0], &fitnessScores, 0, &worldNeuralWeights, &worldsMovementData);
        thread second(runWorld, &worlds[1], &fitnessScores, 1, &worldNeuralWeights, &worldsMovementData);
        thread third(runWorld, &worlds[2], &fitnessScores, 2, &worldNeuralWeights, &worldsMovementData);
        thread fourth(runWorld, &worlds[3], &fitnessScores, 3, &worldNeuralWeights, &worldsMovementData);
        first.join();
        second.join();
        third.join();
        fourth.join();
        int maxF = -10000;
        int maxFi = 0;
        for (int i = 0; i < fitnessScores.size(); i++) {
            if (fitnessScores[i] > maxF) {
                maxF = fitnessScores[i];
                maxFi = i;
            }
        }
        cout << fitnessScores[maxFi] << endl;
        vector<vector<vector<double>>> nextGenNet = worldNeuralWeights[maxFi];
        ofstream geneticData;
        ofstream geneticHistory;
        geneticData.open("neuralNetValues.txt");
        geneticHistory.open("neuralNetHistoricalData.txt", ofstream::app);
        for (int l = 0; l < nextGenNet.size(); l++) {
            for (int n = 0; n < nextGenNet[l].size(); n++) {
                geneticData << "|";
                geneticHistory << "|";
                for (int w = 0; w < nextGenNet[l][n].size(); w++) {
                    geneticData << nextGenNet[l][n][w] << "+";
                    geneticHistory << nextGenNet[l][n][w] << "+";
                }
            }
            geneticData << "|";
            geneticHistory << "|";
        }
        geneticData << "|";
        geneticHistory << "|";
        geneticData << endl;
        geneticHistory << endl << endl;
        geneticData.close();
        geneticHistory.close();
        vector<int> bestNetMovementData = worldsMovementData[maxFi];
        ofstream movementData;
        ofstream movementHistory;
        movementData.open("movementData.txt");
        movementHistory.open("movementHistory.txt", fstream::app);
        movementData << "|";
        movementHistory << "|";
        for (int c = 0; c < bestNetMovementData.size(); c++) {
            if (bestNetMovementData[c] != -1) {
                movementData << bestNetMovementData[c];
                movementData << "+";
                movementHistory << bestNetMovementData[c];
                movementHistory << "+";
            }
            else {
                movementData << "|";
                movementHistory << "|";
            }
        }
        movementData << "|";
        movementHistory << "|" << endl;
        movementHistory << endl;
        movementData.close();
        movementHistory.close();
        for (int i = 0; i < worldsMovementData.size(); i++) {
            worldsMovementData[i].clear();
        }
        for (int i = 0; i < worlds.size(); i++) {
            for (int s = 0; s < worlds[i].size(); s++) {
                worlds[i][s] = 0.0;
            }
        }
        for (int i = 0; i < worldNeuralWeights.size(); i++) {
            worldNeuralWeights[i].clear();
        }
        for (int i = 0; i < fitnessScores.size(); i++) {
            fitnessScores[i] = 0;
        }
    }
    displayBestOfGen(100);
    return 0;
}