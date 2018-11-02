#ifndef ARRAY3D_MATH_H
#define ARRAY3D_MATH_H

#include "common/array3d.h"
#include <exception>

double sigmoid(double x);
double sigmoid_derivative(double x);
array3d_t<double> sigmoid_v(array3d_t<double> const &x);
array3d_t<double> sigmoid_derivative_v(array3d_t<double> const &x);
array3d_t<double> stable_softmax_v(array3d_t<double> const &x);

template<typename T>
size_t argmax1d(array3d_t<T> const &v) {
    assert(v.size() > 0);
    assert(v.shape().size() == 1);
    const size_t size = v.size();
    T max_v = v(0);
    size_t max_i = 0;
    for (size_t i = 1; i < size; i++) {
        T vi = v(i);
        if (vi > max_v) {
            max_v = vi;
            max_i = i;
        }
    }
    return max_i;
}

template<typename T>
T dot_1d(array3d_t<T> const &a, array3d_t<T> const &b) {
    assert(a.shape().size() == b.shape().size());
    assert(a.shape().size() == 1);
    assert(a.size() == b.size());
    T sum = 0;
    const size_t size = b.size();
    for (size_t i = 0; i < size; i++) {
        sum += a(i) * b(i);
    }
    return sum;
}

template<typename T>
array3d_t<T> dot_2d_1d(array3d_t<T> const &m, array3d_t<T> const &v) {
    assert(m.shape().size() == 2);
    assert(v.shape().size() == 1);
    assert(m.shape().x() == v.shape().x());
    const size_t height = m.shape().y();
    array3d_t<T> result(shape3d_t(height, 1, 1), 0);
    for (size_t i = 0; i < height; i++) {
        auto row = m.slice(index3d_t::Y, i, i);
        result(i) = dot_1d(row, v);
    }
    return result;
}

template<typename T>
array3d_t<T> dot_transpose_1d(array3d_t<T> const &a, array3d_t<T> const &b) {
    assert(a.shape().size() == b.shape().size());
    assert(a.shape().size() == 1);

    const size_t height = a.shape().x();
    const size_t width = b.shape().x();

    array3d_t<T> c(shape3d_t(width, height, 1), 0);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            c(j, i) = a(i) * b(j);
        }
    }

    return c;
}

template<typename T>
array3d_t<T> transpose_dot_2d_1d(array3d_t<T> const &m, array3d_t<T> const &v) {
    assert(m.shape().size() == 2);
    assert(v.shape().size() == 1);
    assert(m.shape().y() == v.shape().x());

    const size_t width = m.shape().x();
    const size_t height = m.shape().y();
    array3d_t<T> output(shape3d_t(width, 1, 1), 0);

    for (size_t i = 0; i < width; i++) {
        T sum = 0;
        for (size_t j = 0; j < height; j++) {
            sum += m(i, j) * v(j);
        }
        output(i) = sum;
    }

    return output;
}

template<typename T>
array3d_t<T> dot_transpose(array3d_t<T> const &a, array3d_t<T> const &b) {
    auto &sa = a.shape();
    auto &sb = b.shape();
    if (sa.size() == 1 && sb.size() == 1) {
        return dot_transpose_1d(a, b);
    } else {
        throw std::runtime_error("dot product not supported for such dimensions");
    }
}

template<typename T>
array3d_t<T> transpose_dot(array3d_t<T> const &m, array3d_t<T> const &v) {
    auto &sa = m.shape();
    auto &sb = v.shape();
    if (sa.size() == 2 && sb.size() == 1) {
        return transpose_dot_2d_1d(m, v);
    } else {
        throw std::runtime_error("dot product not supported for such dimensions");
    }
}

#endif // ARRAY3D_MATH_H
