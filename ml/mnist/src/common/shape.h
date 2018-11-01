#ifndef SHAPE_H
#define SHAPE_H

#include <array>
#include <cassert>

#define DIM(start, end) ((end) - (start) + 1)

struct index3d_t {
public:
    enum dim {
        X = 0,
        Y = 1,
        Z = 2
    };

public:
    index3d_t(size_t x, size_t y, size_t z):
        v_({x, y, z})
    { }

    index3d_t(std::array<size_t, 3> const &v):
        v_(v)
    { }

    index3d_t(index3d_t const &other):
        v_(other.v_)
    { }

public:
    size_t &x() { return v_[X]; }
    size_t &y() { return v_[Y]; }
    size_t &z() { return v_[Z]; }
    size_t const &x() const { return v_[X]; }
    size_t const &y() const { return v_[Y]; }
    size_t const &z() const { return v_[Z]; }
    std::array<size_t, 3> &data() { return v_; }
    size_t v(dim d) { return v_[(int)d]; }

public:
    inline index3d_t inc(dim d, size_t a) {
        std::array<size_t, 3> v(v_);
        v[(size_t)d] += a;
        return index3d_t(v);
    }

    inline index3d_t set(dim d, size_t a) {
        std::array<size_t, 3> v(v_);
        v[(size_t)d] = a;
        return index3d_t(v);
    }

public:
    inline bool operator==(const index3d_t &other) const {
        bool any_failure = false;
        for (size_t i = 0; i < 3; i++) {
            if (v_[i] != other.v_[i]) {
                any_failure = true;
                break;
            }
        }
        return !any_failure;
    }

    inline bool operator!=(const index3d_t &other) const {
        return !this->operator==(other);
    }

protected:
    std::array<size_t, 3> v_;
};

struct shape3d_t: index3d_t {
public:
    shape3d_t(size_t x, size_t y, size_t z):
        index3d_t(x, y, z)
    { }

    shape3d_t(shape3d_t const &other):
        index3d_t(other)
    { }

public:
    inline size_t size() const {
        return (v_[X] > 0 ? 1 : 0) +
                (v_[Y] > 0 ? 1 : 0) +
                (v_[Z] > 0 ? 1 : 0);
    }

    inline size_t capacity() const { return v_[X]*v_[Y]*v_[Z]; }

    inline size_t index(size_t x, size_t y, size_t z) const {
        return x + y*v_[X] + z*v_[X]*v_[Y];
    }

    inline size_t index(index3d_t const &i) const {
        return index(i.x(), i.y(), i.z());
    }
};

class index3d_iterator {
public:
    index3d_iterator(index3d_t const &start,
                     index3d_t const &end):
        start_(start),
        end_(end),
        current_(start),
        moves_(0),
        moves_max_(DIM(start.x(), end.x()) *
                   DIM(start.y(), end.y()) *
                   DIM(start.z(), end.z()))
    {}

public:
    bool is_valid() const { return moves_ < moves_max_; }
    index3d_iterator& operator++() { move_next(); return *this; }
    index3d_t &operator*() { return current_; }
    index3d_t const &operator*() const { return current_; }

private:
    void move_next() {
        assert(is_valid());
        auto &idx = current_.data();
        auto &end = end_.data();
        for (size_t j = 0; j < 3; j++) {
            idx[j]++;
            if (idx[j] <= end[j]) { break; }
            idx[j] = 0;
        }
        moves_++;
    }

private:
    index3d_t start_;
    index3d_t end_;
    index3d_t current_;
    size_t moves_;
    size_t moves_max_;
};

#endif // SHAPE_H
