#ifndef ACTIVATOR_H
#define ACTIVATOR_H

#include <functional>
#include "common/calculus_types.h"

template<typename T>
class activator_t {
    using activator_func_t = std::function<vector_t<T>(const vector_t<T>&)>;
public:
    activator_t(activator_func_t const &activation_func,
                activator_func_t const &activation_derivative):
        activation_func_(activation_func),
        derivative_(activation_derivative)
    { }

    activator_t(activator_t const &other):
        activation_func_(other.activation_func_),
        derivative_(other.derivative_)
    { }

public:
    vector_t<T> activate(vector_t<T> const &v) const { return activation_func_(v); }
    vector_t<T> activation_derivative(vector_t<T> const &v) const { return derivative_(v); }

private:
    activator_func_t activation_func_;
    activator_func_t derivative_;
};

#endif // ACTIVATOR_H
