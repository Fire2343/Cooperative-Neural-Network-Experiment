#include "Predator.h"

using namespace std;

Predator::Predator(vector<int> bodyCoords) {
	this->size = bodyCoords.size();
	this->bodyCoords = bodyCoords;
}

int Predator::chooseMove(vector<Prey> preys) {
	int thisCoords = this->bodyCoords[0];
	int prey1Coords = preys[0].bodyCoords[0];
	int prey2Coords = preys[1].bodyCoords[0];
	int distanceTo1 = abs(thisCoords - prey1Coords);
	int distanceTo2 = abs(thisCoords - prey2Coords);
	if (distanceTo1 <= distanceTo2) {
		if (int(prey1Coords / 10) > int(thisCoords / 10)) {
			return 10;
		}
		if (int(prey1Coords / 10) < int(thisCoords / 10)) {
			return -10;
		}
		if (prey1Coords > thisCoords) {
			return 1;
		}
		if (prey2Coords < thisCoords) {
			return -1;
		}
	}
}
