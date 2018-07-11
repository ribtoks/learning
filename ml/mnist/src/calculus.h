#ifndef CALCULUS_H
#define CALCULUS_H

#include "calculus_types.h"

double sigmoid(double x);
double sigmoid_derivative(double x);

template<typename T>
size_t argmax(const vector_t<T> &v) {
    assert(v.size() > 0);
    const size_t size = v.size();
    T max_v = v[0];
    size_t max_i = 0;
    for (size_t i = 1; i < size; i++) {
        if (v[i] > max_v) {
            max_v = v[i];
            max_i = i;
        }
    }
    return max_i;
}

template<typename T>
T dot(const vector_t<T> &a, const vector_t<T> &b) {
    assert(a.size() == b.size());
    T sum = 0;
    const size_t size = b.size();
    for (size_t i = 0; i < size; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

template<typename T>
matrix_t<T> dot_transpose(const vector_t<T> &a, const vector_t<T> &b) {
    const size_t height = a.size();
    const size_t width = b.size();

    matrix_t<T> result(height, width, 0);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            result[i][j] = a[i] * b[j];
        }
    }

    return result;
}

template<typename T>
vector_t<T> dot(const matrix_t<T> &m, const vector_t<T> &v) {
    assert(m.width() == v.size());
    const size_t height = m.height();
    vector_t<T> output(height, 0);
    for (size_t i = 0; i < height; i++) {
        output[i] = dot(m[i], v);
    }
    return output;
}

template<typename T>
vector_t<T> transpose_dot(const matrix_t<T> &m, const vector_t<T> &v) {
    assert(m.height() == v.size());
    vector_t<T> output(m.width(), 0);

    const size_t width = m.width();
    const size_t height = m.height();
    
    for (size_t i = 0; i < width; i++) {
        T sum = 0;
        for (size_t j = 0; j < height; j++) {
            sum += m[j][i] * v[j];
        }
        output[i] = sum;
    }
        
    return output;
}


#endif // CALCULUS_H
