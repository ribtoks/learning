#ifndef NDARRAY_H
#define NDARRAY_H

#include <algorithm>
#include <array>
#include <cassert>
#include <initializer_list>
#include <functional>
#include <numeric>
#include <random>
#include <vector>

#include "common/shape.h"

template<typename T>
class array3d_t {
public:
    array3d_t():
        shape_(0, 0, 0)
    {}

    array3d_t(shape3d_t const &shape, T a):
        shape_(shape),
        v_(shape.capacity(), a)
    {}

    array3d_t(shape3d_t const &shape, T mean, T stddev):
        shape_(shape)
    {
        const size_t size = shape.capacity();
        std::default_random_engine generator;
        std::normal_distribution<T> distribution(mean, stddev);

        v_.reserve(size);
        for (size_t i = 0; i < size; i++) {
            T number = distribution(generator);
            v_.push_back(number);
        }
    }

    array3d_t(array3d_t<T> const &other):
        shape_(other.shape_),
        v_(other.v_)
    {
    }

    array3d_t(array3d_t<T> &&other):
        shape_(other.shape_),
        v_(std::move(other.v_))
    {}

    array3d_t(shape3d_t const &shape, std::vector<T> const &v):
        shape_(shape),
        v_(v)
    {}

    template<typename Q>
    array3d_t(const std::vector<Q> &other):
        shape_(other.size(), 1, 1)
    {
        const size_t size = other.size();
        v_.resize(size);
        for (size_t i = 0; i < size; i++) {
            v_[i] = (T)other[i];
        }
    }

public:
    // slicing supports cases of negative indices
    array3d_t<T> slice(index3d_t const &start,
                       index3d_t const &end,
                       index3d_t const &step=index3d_t(1, 1, 1)) const {
        shape3d_t shape(DIM(start.x(), end.x(), step.x()),
                        DIM(start.y(), end.y(), step.y()),
                        DIM(start.z(), end.z(), step.z()));
        std::vector<T> v(shape.capacity(), T(0));
        index3d_iterator it(start, end, step);
        for (size_t i = 0; it.is_valid(); ++it, ++i) {
            if (in_bounds(*it)) {
                v[i] = v_[shape_.index(*it)];
            }
        }
        return array3d_t<T>(shape, v);
    }

    array3d_t<T> slice(dim_type d, size_t start, size_t end) const {
        index3d_t istart(0, 0, 0);
        index3d_t iend(shape_.x() - 1, shape_.y() - 1, shape_.z() - 1);
        return slice(istart.inc(d, start),
                     iend.set(d, end));
    }

    array3d_t<T> clone() const {
        return array3d_t(*this);
    }

private:
    inline bool in_bounds(index3d_t const &i) {
        return ((0 <= i.x()) && (i.x() < shape_.x())) &&
                ((0 <= i.y()) && (i.y() < shape_.y())) &&
                ((0 <= i.z()) && (i.z() < shape_.z()));
    }

public:
    inline std::vector<T> const &data() const { return v_; }
    inline shape3d_t const &shape() const { return shape_; }
    inline size_t size() const { return v_.size(); }
    inline T &at(size_t x, size_t y, size_t z) { return v_.at(shape_.index(x, y, z)); }
    inline T &operator()(size_t x, size_t y, size_t z) { return at(x, y, z); }
    inline T &operator()(size_t x, size_t y) { return at(x, y, 0); }
    inline T &operator()(size_t x) { return at(x, 0, 0); }
    inline T const &at(size_t x, size_t y, size_t z) const { return v_.at(shape_.index(x, y, z)); }
    inline T const &operator()(size_t x, size_t y, size_t z) const { return at(x, y, z); }
    inline T const &operator()(size_t x, size_t y) const { return at(x, y, 0); }
    inline T const &operator()(size_t x) const { return at(x, 0, 0); }

public:
    array3d_t<T> &operator=(array3d_t<T> &&other) {
        shape_ = other.shape_;
        v_ = std::move(other.v_);
        return *this;
    }

    array3d_t<T> &operator=(array3d_t<T> const &other) = delete;

    array3d_t<T> &mul(const T &a) {
        for (auto &v: v_) { v *= a; }
        return *this;
    }

    array3d_t<T> &element_mul(array3d_t<T> const &other) {
        assert(other.shape() == shape_);
        assert(v_.size() == other.v_.size());

        const size_t size = v_.size();
        for (size_t i = 0; i < size; i++) {
            v_[i] *= other.v_[i];
        }

        return *this;
    }

    array3d_t<T> &add(array3d_t<T> const &other) {
        assert(other.shape() == shape_);
        assert(v_.size() == other.v_.size());

        const size_t size = v_.size();
        for (size_t i = 0; i < size; i++) {
            v_[i] += other.v_[i];
        }

        return *this;
    }

    array3d_t<T> &subtract(array3d_t<T> const &other) {
        assert(other.shape() == shape_);
        assert(v_.size() == other.v_.size());

        const size_t size = v_.size();
        for (size_t i = 0; i < size; i++) {
            v_[i] -= other.v_[i];
        }

        return *this;
    }

    array3d_t<T> &apply(const std::function<T(const T&)> &f) {
        std::transform(v_.begin(), v_.end(), v_.begin(), f);
        return *this;
    }

    void reset(const T &a) {
        std::fill(v_.begin(), v_.end(), a);
    }

    void reshape(shape3d_t const &shape) {
        assert(shape_.capacity() == shape.capacity());
        shape_ = shape;
    }

    T max() const {
        assert(!v_.empty());
        T vmax = v_[0];
        const size_t size = v_.size();
        for (size_t i = 1; i < size; i++) {
            if (v_[i] > vmax) { vmax = v_[i]; }
        }
        return vmax;
    }

    array3d_t<T> flip_xyz() const {
        array3d_t<T> copy(shape_, T(0));

        const size_t x_size = shape_.x();
        const size_t y_size = shape_.y();
        const size_t z_size = shape_.z();

        for (size_t z = 0; z < z_size; z++) {
            for (size_t y = 0; y < y_size; y++) {
                for (size_t x = 0; x < x_size; x++) {
                    copy.at(x, y, z) = this->at(x_size - 1 - x,
                                                y_size - 1 - y,
                                                z_size - 1 - z);
                }
            }
        }

        return copy;
    }

private:
    shape3d_t shape_;
    std::vector<T> v_;
};

#endif // NDARRAY_H
