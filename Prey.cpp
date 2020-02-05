#include "Prey.h"

using namespace std;

Prey::Prey(int size) {
	this->size = size;
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

double Prey::neuralNet(vector<double> inputs, vector<vector<double>> weights) {
	vector<double> nextLayerInputs;
	vector<double> previousLayerInputs = inputs;
	for (int l = 0; l < weights.size(); l++) {
		nextLayerInputs.clear();
		for (int n = 0; n < weights[l].size(); n++) {
			nextLayerInputs.push_back(neuronActivationFunction(previousLayerInputs, weights[l]));
		}
		previousLayerInputs = nextLayerInputs;
	}
	return nextLayerInputs[0];
}