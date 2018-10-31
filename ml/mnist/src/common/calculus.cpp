#include "calculus.h"

#include <cmath>

double sigmoid(double x) {
    return 1.0/(1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    double sigmoid_x = sigmoid(x);
    return sigmoid_x * (1.0 - sigmoid_x);
}

vector_t<double> sigmoid_v(const vector_t<double> &x) {
    vector_t<double> result(x);
    result.apply(sigmoid);
    return x;
}

vector_t<double> sigmoid_derivative_v(const vector_t<double> &x) {
    vector_t<double> result(x);
    result.apply(sigmoid_derivative);
    return x;
}
