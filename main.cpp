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
const int WORLDS = 3; //each thread runs a world


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
           Sleep(500);
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
               //cout << converted << endl;
               vector<int> coordsXY = convertToXY(converted);
               RectangleShape bodyPart(Vector2f(ua, ua));
               //cout << coordsXY[0] << endl;
               //cout << coordsXY[1] << endl;
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
    uniform_real_distribution<double> nlvdistribution(-1.00, 1.00);
    for (int l = 0; l < (*weights).size(); l++) { //percorrer camadas
        for (int n = 0; n < (*weights)[l].size(); n++) { //percorrer neuronios da camada
            if (distribution(generator) == 1 && l < (*weights).size() - 1) { //opurtunidade de mutação para criação de novos neurónios
                vector<double> newNeuron = (*weights)[l][n];
                (*weights)[l].push_back(newNeuron);
            }
            for (int w = 0; w < (*weights)[l][n].size(); w++) { // percorrer pesos do neuronio
                if (distribution(generator) == 1) { // opurtunidade de mutação para alterar os pesos nos neurónio 
                    (*weights)[l][n][w] += vdistribution(generator);
                }
            }
        }
        if (distribution(generator) == 1 && l == (*weights).size() - 1) { //opurtunidade de mutaçao de novas camadas
            vector<vector<double>> newLayer;
            vector<double> newLayerWeights;
            for (int n = 0; n < (*weights)[l].size(); n++) {
                for (int w = 0; w < (*weights)[l][n].size(); w++) {
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
    geneticMemory << "|" << endl;
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
    predatorBodyCoords.push_back(34);
    predatorBodyCoords.push_back(35);
    predatorBodyCoords.push_back(44);
    predatorBodyCoords.push_back(45);
    (*world)[34] = 1.0;
    (*world)[35] = 1.0;
    (*worldsMovementData)[worldNumber].push_back(34);
    (*worldsMovementData)[worldNumber].push_back(35);
    (*world)[44] = 1.0;
    (*world)[45] = 1.0;
    (*worldsMovementData)[worldNumber].push_back(44);
    (*worldsMovementData)[worldNumber].push_back(45);
    Predator predator(predatorBodyCoords);
    vector<Prey> preys;
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
            if (c != ("|")[0] && c != ("+")[0] && c != (" ")[0]) {
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
    preys.push_back(prey1);
    preys.push_back(prey2);
    bool flag1 = false;
    bool flag2 = false;
    bool touchingPrey = false;
    bool endGame = false;
    for (int t = 0; t < 20; t++) {
        flag1 = false;
        flag2 = false;
        (*worldsMovementData)[worldNumber].push_back(-1);
        vector<int> occupiedCoords;
        vector <int> unnocupiedCoords;
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
                    if (insideBounds(convertToXY(prey1.bodyCoords[bi] + toMove), toMove) == false || (*world)[prey1.bodyCoords[bi] + toMove] != 0.0) {
                        if (belongsToSelf(prey1.bodyCoords[bi] + toMove, &prey1.bodyCoords) == false) {
                            occupiedCoords.insert(occupiedCoords.end(), buffer.begin(), buffer.end());
                            dontMove = true;
                            break;
                        }
                    }
                }
                if (!dontMove) {
                    for (int bi = 0; bi < prey1.bodyCoords.size(); bi++) {
                        unnocupiedCoords.push_back(prey1.bodyCoords[bi]);
                        prey1.bodyCoords[bi] += toMove;
                        occupiedCoords.push_back(prey1.bodyCoords[bi]);
                        if (!flag1) {
                            flag1 = checkAdjacency(&predator.bodyCoords, prey1.bodyCoords[bi]);
                        }
                    }
                }
            }
            else {
                vector<int> buffer;
                for (int bi = 0; bi < prey2.bodyCoords.size(); bi++) {
                    buffer.push_back(prey2.bodyCoords[bi]);
                    if (insideBounds(convertToXY(prey2.bodyCoords[bi] + toMove), toMove) == false || (*world)[prey2.bodyCoords[bi] + toMove] != 0.0) {
                        if (belongsToSelf(prey2.bodyCoords[bi] + toMove, &prey2.bodyCoords) == false) {
                            occupiedCoords.insert(occupiedCoords.end(), buffer.begin(), buffer.end());
                            dontMove = true;
                            break;
                        }
                    }
                }
                if (!dontMove) {
                    for (int bi = 0; bi < prey2.bodyCoords.size(); bi++) {
                        unnocupiedCoords.push_back(prey2.bodyCoords[bi]);
                        prey2.bodyCoords[bi] += toMove;
                        occupiedCoords.push_back(prey2.bodyCoords[bi]);
                        if (!flag2) {
                            flag2 = checkAdjacency(&predator.bodyCoords, prey2.bodyCoords[bi]);
                        }
                    }
                }
            }
        }
        if (flag1 && flag2) {
            (*fitnessValues)[worldNumber] += 1000 / (t + 1);
            endGame = true;
        }
        if (!endGame) {
            bool dontMove = false;
            int predatorMove = predator.chooseMove(preys);
            vector<int> buffer;
            for (int bi = 0; bi < predator.bodyCoords.size(); bi++) {
                buffer.push_back(predator.bodyCoords[bi]);
                if (insideBounds(convertToXY(predator.bodyCoords[bi] + predatorMove), predatorMove) == false || (*world)[predator.bodyCoords[bi] + predatorMove] != 0.0) {
                    if (belongsToSelf(predator.bodyCoords[bi] + predatorMove, &predator.bodyCoords) == false) {
                        occupiedCoords.insert(occupiedCoords.end(), buffer.begin(), buffer.end());
                        dontMove = true;
                        break;
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
            if (touchingPrey) {
                (*fitnessValues)[worldNumber] -= 100 / (t + 1);
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
    
    //TODO: MELHORAR FUNÇÃO OUT OF BOUNDS PARA N FAZER TIPO 19->20!
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
    //generateInitialGeneticMemory(&worlds[0]);
    /*for (int w = 0; w < WORLDS; w++) {
        runWorld(&worlds[w], &fitnessScores, w, &worldNeuralWeights, &worldsMovementData);
    }*/
    for (int g = 0; g < 20; g++) {
        thread first(runWorld, &worlds[0], &fitnessScores, 0, &worldNeuralWeights, &worldsMovementData);
        thread second(runWorld, &worlds[1], &fitnessScores, 1, &worldNeuralWeights, &worldsMovementData);
        thread third(runWorld, &worlds[2], &fitnessScores, 2, &worldNeuralWeights, &worldsMovementData);
        first.join();
        second.join();
        third.join();
        int maxF = -10000;
        int maxFi = 0;
        for (int i = 0; i < fitnessScores.size(); i++) {
            if (fitnessScores[i] > maxF) {
                maxF = fitnessScores[i];
                maxFi = i;
            }
        }
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
            //cout << bestNetMovementData[c] << endl;
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
    }
    displayBestOfGen(100);
    return 0;
}