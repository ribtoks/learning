#ifndef INPUT_H
#define INPUT_H

#include "common/array3d.h"

template<typename T>
struct layer_input_t {
    layer_input_t(array3d_t<T> &a):
        data(std::move(a))
    {}
    layer_input_t(array3d_t<T> const &a):
        data(a)
    {}

    array3d_t<T> data;
};

#endif // INPUT_H
