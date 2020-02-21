//#include <SFML/Graphics.hpp>
#include <thread>
#include <fstream>
#include <string>
#include <sstream> 
#include <iostream>
#include <random>
#include <chrono>
#include "Predator.h"
#include "Prey.h"


const int DWW = 1000; // display window width, in pixels.
const int DWH = 1000; //display window height, in pixels.
const int WSL = 10; //size of both world dimensions, in units-area.
const int WORLDS = 10; //each thread runs a world


//using namespace sf;
using namespace std;

bool insideBounds(vector<int> coords) {
    if (coords[0] < 0 || coords[0] > 9) {
        return false;
    }
    if (coords[1] < 0 || coords[1] > 9) {
        return false;
    }
    return true;
}

vector<int> convertToXY(int coord) {
    vector<int> xycoords;
    int x;
    int y;
    if (coord / 10 > 0) {
        x = ((double(coord) / 10 - coord / 10) * 10);
        y = int(coord / 10);
    }
    else {
        x = coord;
        y = 0;
    }
    xycoords.push_back(x);
    xycoords.push_back(y);
    return xycoords;
}

void displayBestOfGen(int generation) {
    /* RenderWindow window(VideoMode(DWW, DWH), "SFML works!");

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
    } */
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
            for (int n = 0; n < (*weights)[l].size(); n++) {
                vector<double> newLayerWeights;
                for (int w = 0; w < (*weights)[l][n].size(); w++) {
                    newLayerWeights.push_back(nlvdistribution(generator));
                }
                newLayer.push_back(newLayerWeights);
            }
            (*weights).push_back(newLayer);
        }
    }
}
void generateInitialGeneticMemory(vector<double> *world) {
    ofstream geneticMemory("neuralNetValues.txt", fstream::app);
    geneticMemory << "|";
    for (int n = 0; n < 4; n++) {
        for (int i = 0; i < (*world).size() + 2; i++) {
            geneticMemory << "0.00-";
        }
        geneticMemory << "|";
    }
    geneticMemory << "|" << endl;
    geneticMemory.close();
}

bool checkAdjacency(vector<int> *targetBodyParts, int seekerBodyPart) {
    for (int i = 0; i < (*targetBodyParts).size(); i++) {
        int d = abs((*targetBodyParts)[i] - seekerBodyPart);
        if (!insideBounds(convertToXY(d))) {
            continue;
        }
        switch (d) 
        {
            case 11:
                return true;
            case 10:
                return true;
            case 9:
                return true;
            case 1:
                return true;
            default:
                return false;
        }
    }
}

void runWorld(vector<double> *world, vector<int> *fitnessValues, int worldNumber, vector<vector<vector<vector<double>>>> *worldNeuralWeights) {
    vector<int> predatorBodyCoords;
    predatorBodyCoords.push_back(34);
    predatorBodyCoords.push_back(35);
    predatorBodyCoords.push_back(44);
    predatorBodyCoords.push_back(45);
    (*world)[34, 35] = 1.0;
    (*world)[44, 45] = 1.0;
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
            if (c != ("|")[0] && c != ("\n")[0] && c != ("-")[0]) {
                stringstream converter(data.substr(i, 4));
                double converted;
                converter >> converted;
                //cout << converted << endl;
                i += 3;
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
    cout << weights.size() << endl;
    cout << weights[0].size() << endl;
    cout << weights[0][0].size() << endl;
    Prey prey1(preyBodyCoords, weights);
    (*world)[18, 19] = 1.0;
    preyBodyCoords[0] += 50;
    preyBodyCoords[1] += 50;
    Prey prey2(preyBodyCoords, weights);
    (*world)[68, 69] = 1.0;
    preys.push_back(prey1);
    preys.push_back(prey2);
    bool flag1 = false;
    bool flag2 = false;
    bool touchingPrey = false;
    for (int t = 0; t < 20; t++) {
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
            if (osi == 0) {
                for (int bi = 0; bi < prey1.bodyCoords.size(); bi++) {  //Check to see if both preys are touching predator, so their roles may change.                    
                    if (!insideBounds(convertToXY(prey1.bodyCoords[bi] + toMove))) {
                        break;
                    }
                    (*world)[prey1.bodyCoords[bi]] = 0.0;
                    prey1.bodyCoords[bi] += toMove;
                    (*world)[prey1.bodyCoords[bi]] = 1.0;
                    if (!flag1) {
                        flag1 = checkAdjacency(&predator.bodyCoords, prey1.bodyCoords[bi]);
                    }
                }
            }
            else {
                for (int bi = 0; bi < prey2.bodyCoords.size(); bi++) {
                    if (!insideBounds(convertToXY(prey1.bodyCoords[bi] + toMove))) {
                        break;
                    }
                    (*world)[prey2.bodyCoords[bi]] = 0.0;
                    prey2.bodyCoords[bi] += toMove;
                    (*world)[prey2.bodyCoords[bi]] = 1.0;
                    if (!flag2) {
                        flag2 = checkAdjacency(&predator.bodyCoords, prey2.bodyCoords[bi]);
                    }
                }
            }
        }
        if (flag1 && flag2) {
            (*fitnessValues)[worldNumber] += 1000 / t;
            break;
        }
        int predatorMove = predator.chooseMove(preys);
        for (int bi = 0; bi < predator.bodyCoords.size(); bi++) {
            if (!insideBounds(convertToXY(predator.bodyCoords[bi] + predatorMove))) {
                break;
            }
            (*world)[predator.bodyCoords[bi]] = 0.0;
            predator.bodyCoords[bi] += predatorMove;
            (*world)[predator.bodyCoords[bi]] = 1.0;
            if (!touchingPrey) {
                touchingPrey = checkAdjacency(&prey1.bodyCoords, predator.bodyCoords[bi]);
            }
        }
        if (touchingPrey) {
            (*fitnessValues)[worldNumber] -= 100 / t;
            break;
        }
    }
    (*worldNeuralWeights)[worldNumber] = weights;
}

int main() {
    int ua = 100; //size of each unit-area side, in pixels (square this number to get unit-area in pixels).
    vector<vector<double>> worlds;
    vector<int> fitnessScores;
    vector<vector<vector<vector<double>>>> worldNeuralWeights(WORLDS);
    for(int w = 0; w < WORLDS; w++) {
        vector<double> world(WSL * WSL, 0.0); //each world is represented by a single vector despite technically being a m * n matrix. The first n elements represent all the columns of the first row, and so on and so on.
        worlds.push_back(world);
        fitnessScores.push_back(0);
    }
    //generateInitialGeneticMemory(&worlds[0]);
    //runWorld(&worlds[0]);
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
    geneticHistory.open("neuralNetHistoricalData.txt");
    for (int l = 0; l < nextGenNet.size(); l++) {
        for (int n = 0; n < nextGenNet[l].size(); n++) {
            geneticData << "|";
            geneticHistory << "|";
            for (int w = 0; w < nextGenNet[l][n].size(); w++) {
                geneticData << nextGenNet[l][n][w] << "-";
                geneticHistory << nextGenNet[l][n][w] << "-";
            }
        }
        geneticData << "|";
        geneticHistory << "|";
    }
    geneticData << endl;
    geneticHistory << endl;
    geneticData.close();
    geneticHistory.close();
	return 0;
}