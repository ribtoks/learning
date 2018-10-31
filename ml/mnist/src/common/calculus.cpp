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
    return result;
}

vector_t<double> sigmoid_derivative_v(const vector_t<double> &x) {
    vector_t<double> result(x);
    result.apply(sigmoid_derivative);
    return result;
}

vector_t<double> stable_softmax_v(const vector_t<double> &x) {
    vector_t<double> result(x);
    const size_t size = result.size();
    double x_max = x[0];
    for (size_t i = 0; i < size; i++) {
        if (x[i] > x_max) { x_max = x[i]; }
    }

    double sum = 0.0;
    for (size_t i = 0; i < size; i++) {
        result[i] = exp(x[i] - x_max);
        sum += result[i];
    }

    for (size_t i = 0; i < size; i++) {
        result[i] /= sum;
    }

    return result;
}
