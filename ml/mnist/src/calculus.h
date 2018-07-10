#ifndef CALCULUS_H
#define CALCULUS_H

#include <vector>
#include <cmath>
#include <random>
#include <cassert>
#include <functional>

double sigmoid(double x) {
    return 1.0/(1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    double sigmoid_x = sigmoid(x);
    return sigmoid_x * (1.0 - sigmoid_x);
}

template<typename T>
class matrix_t;

template<typename T = double>
class vector_t {
public:
    vector_t(size_t size, T mean, T stddev) {
        std::default_random_engine generator;
        std::normal_distribution<T> distribution(mean, stddev);

        v_.reserve(size);
        for (size_t i = 0; i < size; i++) {
            T number = distribution(generator);
            v_.push_back(number);
        }
    }

    vector_t(const vector_t<T> &other):
        v_(other.v_)
    {
    }

    vector_t(size_t size, T v) {
        v_.resize(size, v);
    }

    vector_t() { }

public:
    T &operator[](size_t index) { return v_[index]; }
    T const &operator[](size_t index) const { return v_[index]; }

    size_t size() const { return v_.size(); }

public:
    void operator=(vector_t<T> const &other) {
        if (size() != other.size()) {
            v_.resize(other.size());
        }
        const size_t size = other.size();
        for (size_t i = 0; i < size; i++) {
            v_[i] = other.v_[i];
        }
    }

public:
    T dot(const vector_t<T> &other) const {
        assert(other.size() == size());
        T sum = 0;
        const size_t size = other.size();
        for (size_t i = 0; i < size; i++) {
            sum += v_[i] * other.v_[i];
        }
        return sum;
    }

    matrix_t<T> dot_transpose(const vector_t<T> &other) const {
        const size_t height = this->size();
        const size_t width = other.size();

        matrix_t<T> result(height, width, 0);

        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                result[i][j] = v_[i] * other.v_[j];
            }
        }

        return result;
    }

    vector_t<T> &mul(const T &a) {
       for (auto &v: v_) {
           v *= a;
        }
        return *this;
    }

    vector_t<T> &element_mul(const vector_t<T> &other) {
        assert(other.size() == size());
        const size_t size = other.size();
        for (size_t i = 0; i < size; i++) {
            v_[i] *= other.v_[i];
        }
        return *this;
    }

    vector_t<T> &add(const vector_t<T> &other) {
        assert(other.size() == size());
        const size_t size = other.size();
        for (size_t i = 0; i < size; i++) {
            v_[i] += other.v_[i];
        }
        return *this;
    }

    vector_t<T> &subtract(const vector_t<T> &other) {
        assert(other.size() == size());
        const size_t size = other.size();
        for (size_t i = 0; i < size; i++) {
            v_[i] -= other.v_[i];
        }
        return *this;
    }

    vector_t<T> &apply(const std::function<T(const T&)> &f) {
        for (auto &v: v_) {
            v = f(v);
        }
        return *this;
    }

    void reset(const T &a) {
        for (auto &v: v_) {
            v = a;
        }
    }
    
private:
    std::vector<T> v_;
};

template<typename T = double>
class matrix_t {
public:
    matrix_t(size_t height, size_t width, T mean, T stddev):
        height_(height),
        width_(width)
    {
        for (size_t i = 0; i < height; i++) {
            rows_.emplace_back(vector_t<T>(width, mean, stddev));
        }
    }

    matrix_t(size_t height, size_t width, T v):
        height_(height),
        width_(width)
    {
        for (size_t i = 0; i < height; i++) {
            rows_.emplace_back(vector_t<T>(width, v));
        }
    }

public:
    vector_t<T> &operator[](size_t index) { return rows_[index]; }
    vector_t<T> const &operator[](size_t index) const { return rows_[index]; }

    size_t height() const { return height_; }
    size_t width() const { return width_; }
    
public:
    vector_t<T> dot(const vector_t<T> &v) const {
        assert(width_ == v.size());
        vector_t<T> output(height_, 0);
        for (size_t i = 0; i < height_; i++) {
            output[i] = rows_[i].dot(v);
        }
        return output;
    }

    vector_t<T> transpose_dot(const vector_t<T> &v) const {
        assert(height_ == v.size());
        vector_t<T> output(width_, 0);

        for (size_t i = 0; i < width_; i++) {
            T sum = 0;
            for (size_t j = 0; j < height_; j++) {
                sum += rows_[j][i] * v[j];
            }
            output[i] = sum;
        }
        
        return output;
    }

    matrix_t<T> &mul(const T &v) {
        for (auto &r: rows_) {
            r.mul(v);
        }
        return *this;
    }

    matrix_t<T> &add(const matrix_t<T> &other) {
        assert(height_ == other.height_);

        for (size_t i = 0; i < height_; i++) {
            rows_[i].add(other.rows_[i]);
        }
        return *this;
    }

    void reset(const T &v) {
        for (auto &r: rows_) {
            r.reset(v);
        }
    }

private:
    std::vector<vector_t<T>> rows_;
    size_t height_;
    size_t width_;
};


#endif // CALCULUS_H
