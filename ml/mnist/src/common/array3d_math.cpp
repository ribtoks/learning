#include "array3d_math.h"

#include <cmath>

double sigmoid(double x) {
    return 1.0/(1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    double sigmoid_x = sigmoid(x);
    return sigmoid_x * (1.0 - sigmoid_x);
}

array3d_t<double> sigmoid_v(const array3d_t<double> &x) {
    assert(x.shape().size() == 1);
    array3d_t<double> result(x);
    result.slice().apply(sigmoid);
    return result;
}

array3d_t<double> sigmoid_derivative_v(const array3d_t<double> &x) {
    assert(x.shape().size() == 1);
    array3d_t<double> result(x);
    result.slice().apply(sigmoid_derivative);
    return result;
}

array3d_t<double> stable_softmax_v(const array3d_t<double> &x) {
    assert(x.shape().size() == 1);
    array3d_t<double> result(x);
    const size_t size = result.size();
    double x_max = x(0);
    for (size_t i = 0; i < size; i++) {
        double xi = x(i);
        if (xi > x_max) { x_max = xi; }
    }

    double sum = 0.0;
    for (size_t i = 0; i < size; i++) {
        double fi = exp(x(i) - x_max);;
        result(i) = fi;
        sum += fi;
    }

    for (size_t i = 0; i < size; i++) {
        result(i) /= sum;
    }

    return result;
}
