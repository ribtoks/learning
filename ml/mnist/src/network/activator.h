#ifndef ACTIVATOR_H
#define ACTIVATOR_H

#include <functional>

template<typename T>
class activator_t {
    using activator_func_t = std::function<T(const T&)>;
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
    activator_func_t const &activator() const { return activation_func_; }
    activator_func_t const &derivative() const { return derivative_; }

private:
    activator_func_t activation_func_;
    activator_func_t derivative_;
};

#endif // ACTIVATOR_H
