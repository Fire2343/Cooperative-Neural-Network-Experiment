#include "Predator.h"

using namespace std;

Predator::Predator(vector<int> bodyCoords) {
	this->size = bodyCoords.size();
	this->bodyCoords = bodyCoords;
}

int Predator::chooseMove(vector<Prey> preys) {
	vector<int> rawCoords;
	rawCoords.push_back(this->bodyCoords[0]);
	rawCoords.push_back(preys[0].bodyCoords[0]);
	rawCoords.push_back(preys[1].bodyCoords[0]);
	vector<int> ycoords(3,0);
	vector<int> xcoords(3,0);
	int directionX = 0;
	int directionY = 0;
	int distanceX = 0;
	int distanceY = 0;
	ycoords[0] = int(rawCoords[0] / 10);
	ycoords[1] = int(rawCoords[1] / 10);
	ycoords[2] = int(rawCoords[2] / 10);
	for (int i = 0; i < 3; i++) {
		for (int c = ycoords[i] * 10; c < rawCoords[i]; c++) {
			xcoords[i]++;
		}
	}
	double prey1Distance = hypot(xcoords[1] - xcoords[0], ycoords[1] - ycoords[0]);
	double prey2Distance = hypot(xcoords[2] - xcoords[0], ycoords[2] - ycoords[0]);
	if (prey1Distance <= prey2Distance) {
		distanceX = xcoords[1] - xcoords[0];
		distanceY = ycoords[1] - ycoords[0];
	}
	else {
		distanceX = xcoords[2] - xcoords[0];
		distanceY = ycoords[2] - ycoords[0];
	}
	if (distanceX != 0) {
		directionX = distanceX / abs(distanceX);
	}
	if (distanceY != 0) {
		directionY = (distanceY / abs(distanceY)) * 10;
	}
	return directionX + directionY;
}
