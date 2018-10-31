#ifndef CALCULUS_TYPES_H
#define CALCULUS_TYPES_H

#include <vector>
#include <random>
#include <cassert>
#include <functional>

struct shape_t {
    size_t x;
    size_t y;
    size_t z;
    size_t d;
};

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

    vector_t(const vector_t<T> &other) :
        v_(other.v_)
    {
    }

    template<typename Q>
    vector_t(const std::vector<Q> &other) {
        const size_t size = other.size();
        v_.resize(size);
        for (size_t i = 0; i < size; i++) {
            v_[i] = (T)other[i];
        }
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
        v_ = other.v_;
    }

public:
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
        rows_.reserve(height);
        for (size_t i = 0; i < height; i++) {
            rows_.emplace_back(width, mean, stddev);
        }
    }

    matrix_t(size_t height, size_t width, T v):
        height_(height),
        width_(width)
    {
        rows_.reserve(height);
        for (size_t i = 0; i < height; i++) {
            rows_.emplace_back(width, v);
        }
    }

public:
    vector_t<T> &operator[](size_t index) { return rows_[index]; }
    vector_t<T> const &operator[](size_t index) const { return rows_[index]; }

    size_t height() const { return height_; }
    size_t width() const { return width_; }

public:
    void operator=(matrix_t<T> const &other) {
        if (rows_.size() != other.rows_.size()) {
            rows_.resize(other.rows_.size());
        }

        const size_t size = rows_.size();
        for (size_t i = 0; i < size; i++) {
            rows_[i] = other.rows_[i];
        }
    }
    
public:
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


#endif // CALCULUS_TYPES_H
