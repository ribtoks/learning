#ifndef ERROR_H
#define ERROR_H

#include "common/array3d.h"

template <typename T>
struct layer_error_t {
    layer_error_t(array3d_t<T> &other):
        data(std::move(other))
    {}

    layer_error_t(array3d_t<T> const &other):
        data(other)
    {}

    array3d_t<T> data;
};

#endif // ERROR_H
