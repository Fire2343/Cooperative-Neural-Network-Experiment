#pragma once
#include <vector>
#include <cmath>

class Prey
{
public:
	int size;
	Prey(int size);
	double sigmoidFunction(double z);
	double neuronActivationFunction(std::vector<double> inputs, std::vector<double> weights);
	double neuralNet(std::vector<double> inputs, std::vector<std::vector<double>> weights);
};