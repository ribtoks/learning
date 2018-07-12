#include "calculus.h"
#include <cmath>

double sigmoid(double x) {
    return 1.0/(1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    double sigmoid_x = sigmoid(x);
    return sigmoid_x * (1.0 - sigmoid_x);
}
