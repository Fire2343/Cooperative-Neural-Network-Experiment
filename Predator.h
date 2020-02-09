#include <vector>
#include "Prey.h"

#pragma once
class Predator
{
public:
	int size;
	std::vector<int> bodyCoords;
	Predator(std::vector<int> bodyCoords);
	int chooseMove(std::vector<Prey> preys);
};

