#include <vector>
#include <cmath>

#pragma once
class Prey
{
public:
	int size;
	std::vector<int> bodyCoords;
	Prey(std::vector<int> bodyCoords);
	double sigmoidFunction(double z);
	double neuronActivationFunction(std::vector<double> inputs, std::vector<double> weights);
	double neuralNet(std::vector<double> inputs, std::vector<std::vector<double>> weights);
};