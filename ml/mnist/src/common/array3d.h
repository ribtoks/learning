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
    class slice3d {
    public:
        slice3d(array3d_t<T> &array,
                index3d_t const &start,
                index3d_t const &end):
            array_(array),
            shape_(DIM(end.x(), start.x()),
                   DIM(end.y(), start.y()),
                   DIM(end.z(), start.z())),
            start_(start),
            end_(end)
        { }

    public:
        T &operator()(size_t x, size_t y, size_t z) { return at(x, y, z); }
        T &operator()(size_t x, size_t y) { return at(x, y, 0); }
        T &operator()(size_t x) { return at(x, 0, 0); }
        array3d_t<T> &array() { return array_; }
        shape3d_t const &shape() const { return shape_; }
        size_t size() const {
            return DIM(start_.x(), end_.x()) *
                    DIM(start_.y(), end_.y()) *
                    DIM(start_.z(), end_.z());
        }

    public:
        slice3d &mul(const T &a) {
            index3d_iterator it(start_, end_);
            for (; it.is_valid(); ++it) {
                at(*it) *= a;
            }
            return *this;
        }

        slice3d &element_mul(slice3d const &other) {
            assert(other.shape() == shape_);

            index3d_iterator it(start_, end_);
            index3d_iterator ito(other.start_, other.end_);

            for (; it.is_valid() && ito.is_valid();
                 ++it, ++ito) {
                at(*it) *= other.at(*ito);
            }

            return *this;
        }

        slice3d &add(slice3d const &other) {
            assert(other.shape() == shape_);

            index3d_iterator it(start_, end_);
            index3d_iterator ito(other.start_, other.end_);

            for (; it.is_valid() && ito.is_valid();
                 ++it, ++ito) {
                at(*it) += other.at(*ito);
            }

            return *this;
        }

        slice3d &subtract(slice3d const &other) {
            assert(other.shape() == shape_);

            index3d_iterator it(start_, end_);
            index3d_iterator ito(other.start_, other.end_);

            for (; it.is_valid() && ito.is_valid();
                 ++it, ++ito) {
                at(*it) -= other.at(*ito);
            }

            return *this;
        }

        slice3d &apply(const std::function<T(const T&)> &f) {
            index3d_iterator it(start_, end_);
            for (; it.is_valid(); ++it) {
                T &v = at(*it);
                v = f(v);
            }
            return *this;
        }

        void reset(const T &a) {
            index3d_iterator it(start_, end_);
            for (; it.is_valid(); ++it) {
                at(*it) = a;
            }
        }

        slice3d slice(index3d_t::dim d, size_t start, size_t end) {
            return slice3d(array_,
                           start_.inc(d, start),
                           end_.set(d, start_.v(d) + end));
        }

    private:
        inline size_t array_index(size_t x, size_t y, size_t z) {
            return array_.shape_.index(start_) + shape_.index(x, y, z);
        }

        inline T &at(size_t x, size_t y, size_t z) { return array_.v_.at(array_index(x, y, z)); }
        inline T &at(index3d_t const &i) { return array_.v_.at(array_.shape_.index(i)); }

    private:
        array3d_t<T> &array_;
        shape3d_t shape_;
        index3d_t start_;
        index3d_t end_;
    };

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
    {}

    array3d_t(array3d_t<T> &&other):
        shape_(other.shape_),
        v_(std::move(other.v_))
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
    shape3d_t const &shape() const { return shape_; }
    size_t size() const { return v_.size(); }
    slice3d slice() { return slice3d(*this,
                                     index3d_t(0, 0, 0),
                                     index3d_t(shape_.x(), shape_.y(), shape_.z())); }

    slice3d slice(index3d_t const &start,
                  index3d_t const &end) { return slice3d(*this, start, end); }

    slice3d slice(index3d_t::dim d, size_t start, size_t end) {
        return slice().slice(d, start, end);
    }

    inline T &at(size_t x, size_t y, size_t z) { return v_.at(shape_.index(x, y, z)); }
    T &operator()(size_t x, size_t y, size_t z) { return at(x, y, z); }
    T &operator()(size_t x, size_t y) { return at(x, y, 0); }
    T &operator()(size_t x) { return at(x, 0, 0); }
    inline T const &at(size_t x, size_t y, size_t z) const { return v_.at(shape_.index(x, y, z)); }
    T const &operator()(size_t x, size_t y, size_t z) const { return at(x, y, z); }
    T const &operator()(size_t x, size_t y) const { return at(x, y, 0); }
    T const &operator()(size_t x) const { return at(x, 0, 0); }

public:
    array3d_t<T> &operator=(array3d_t<T> &&other) {
        shape_ = other.shape_;
        v_ = std::move(other.v_);
        return *this;
    }

    array3d_t<T> &operator=(array3d_t<T> const &other) {
        shape_ = other.shape_;
        v_ = other.v_;
        return *this;
    }

    array3d_t<T> &add(array3d_t<T> const &other) { slice().add(other.slice()); return *this; }
    array3d_t<T> &subtract(array3d_t<T> const &other) { slice().subtract(other.slice()); return *this; }
    array3d_t<T> &mul(const T &a) { slice().mul(a); return *this; }
    array3d_t<T> &element_mul(array3d_t<T> const &other) { slice().element_mul(other.slice()); }
    void reset(T const a) { slice().reset(a); }

private:
    shape3d_t shape_;
    std::vector<T> v_;
};

#endif // NDARRAY_H
