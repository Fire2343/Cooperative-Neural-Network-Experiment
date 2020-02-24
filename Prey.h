#include <vector>
#include <cmath>
#include <iostream>

#pragma once
class Prey
{
public:
	int size;
	std::vector<int> bodyCoords;
	Prey(std::vector<int> bodyCoords, std::vector<std::vector<std::vector<double>>> weights);
	std::vector<std::vector<std::vector<double>>> weights;
	double sigmoidFunction(double z);
	double neuronActivationFunction(std::vector<double> inputs, std::vector<double> weights);
	std::vector<double> neuralNet(std::vector<double> inputs);
};