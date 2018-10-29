#ifndef FULLY_CONNECTED_LAYER_H
#define FULLY_CONNECTED_LAYER_H

#include <functional>
#include "common/calculus_types.h"
#include "network/activator.h"

template<typename T = double>
class fully_connected_layer_t {
public:
    fully_connected_layer_t(size_t layer_dim, size_t prev_layer_dim,
                            T mean, T stddev,
                            activator_t<T> const &activator):
        dimension_(layer_dim),
        weights_(layer_dim, prev_layer_dim, mean, stddev),
        bias_(layer_dim, mean, stddev),
        activator_(activator)
    { }

public:
    matrix_t<T> const &nabla_w() const { return nabla_w_; }
    vector_t<T> const &nabla_b() const { return nabla_b_; }

public:
    vector_t<T> const &feedforward(vector_t<T> const &input) {
        z_ = dot(weights_, input).add(bias_);
        a_ = z_;
        a_.apply(activator_.activator());
        return a_;
    }

    vector_t<T> backpropagate(vector_t<T> const &error) {
        // delta(l) = (w(l+1) * delta(l+1)) [X] sigma_deriv(z(l))
        // dC/db = delta(l)
        // dC/dw = a(l-1) * delta(l)
        vector_t<T> delta = z_.apply(activator_.derivative()).element_mul(error);
        nabla_b_ = delta;
        nabla_w_ = dot_transpose(delta, a_);
        delta = transpose_dot(weights_, delta);
        return delta;
    }

private:
    // own data
    size_t dimension_;
    matrix_t<T> weights_;
    vector_t<T> bias_;
    activator_t<T> const &activator_;
    // calculation support
    vector_t<T> z_, a_;
    matrix_t<T> nabla_w_;
    vector_t<T> nabla_b_;
};

#endif // FULLY_CONNECTED_LAYER_H
