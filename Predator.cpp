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
	vector<int> xdistance;
	vector<int> ydistance;
	vector<double> distances;
	vector<int> movementDirectionX(2, 0);
	vector<int> movementDirectionY(2, 0);
	vector<int> ycoords(3,0);
	vector<int> xcoords(3,0);
	int yc = 0;
	while (yc < thisCoords) {
		yc += 10;
	}
	ycoords[0] = yc / 10;
	xcoords[0] = thisCoords - yc;
	for (int i = 0; i < preys.size(); i++) {
		int yc = 0;
		while(yc < preys[i].bodyCoords[0]) {
			yc += 10;
		}
		ycoords[i + 1] += yc;
		xcoords[i + 1] = preys[i].bodyCoords[0] - yc;
	}
	if (xcoords[1] - xcoords[0] != 0) {
		movementDirectionX[0] = (xcoords[1] - xcoords[0]) / abs(xcoords[1] - xcoords[0]);
	}
	if (xcoords[2] - xcoords[0] != 0) {
		movementDirectionX[1] = (xcoords[2] - xcoords[0]) / abs(xcoords[2] - xcoords[0]);
	}
	if (ycoords[1] - ycoords[0] != 0) {
		movementDirectionY[0] = (ycoords[1] - ycoords[0]) / abs(ycoords[1] - ycoords[0]);
	}
	if (ycoords[2] - ycoords[0] != 0) {
		movementDirectionY[1] = (ycoords[2] - ycoords[0]) / abs(ycoords[2] - ycoords[0]);
	}
	xdistance.push_back(abs(xcoords[1] - xcoords[0]));
	xdistance.push_back(abs(xcoords[2] - xcoords[0]));
	ydistance.push_back(abs(ycoords[1] - ycoords[0]));
	ydistance.push_back(abs(ycoords[2] - ycoords[0]));
	distances.push_back(sqrt(pow(2.0, xdistance[0]) + pow(2.0, ydistance[0])));
	distances.push_back(sqrt(pow(2.0, xdistance[1]) + pow(2.0, ydistance[1])));
	if (distances[0] <= distances[1]) {
		return movementDirectionX[0] + 10 * movementDirectionY[0];
	}
	else {
		return movementDirectionX[1] + 10 * movementDirectionY[1];
	}
}
