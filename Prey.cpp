#include "Prey.h"

using namespace std;

Prey::Prey(vector<int> bodyCoords, vector<vector<vector<double>>> weights) {
	this->size = bodyCoords.size();
	this->bodyCoords = bodyCoords;
	this->weights = weights;
}

double Prey::sigmoidFunction(double z) {
	return 1 / (1 + exp(-z));
}

double Prey::neuronActivationFunction(vector<double> inputs, vector<double> weights) {
	double dotProduct = 0;
	for (int i = 0; i < inputs.size(); i++) {
		dotProduct += inputs[i] * weights[i];
	}
	return sigmoidFunction(dotProduct);
}

vector<double> Prey::neuralNet(vector<double> inputs) {
	vector<double> nextLayerInputs;
	vector<double> previousLayerInputs = inputs;
	for (int l = 0; l < weights.size(); l++) {
		nextLayerInputs.clear();
		for (int n = 0; n < weights[l].size(); n++) {
			nextLayerInputs.push_back(neuronActivationFunction(previousLayerInputs, weights[l][n]));
		}
		previousLayerInputs = nextLayerInputs;
	}
	return nextLayerInputs;
}