#ifndef FULLY_CONNECTED_LAYER_H
#define FULLY_CONNECTED_LAYER_H

#include <functional>

#include "strategy/train_strategy.h"
#include "common/calculus_types.h"
#include "common/log.h"
#include "network/activator.h"
#include "network/layers/layer_base.h"

template<typename T = double>
class fully_connected_layer_t: public layer_base_t<T> {
public:
    fully_connected_layer_t(size_t layer_in, size_t layer_out,
                            activator_t<T> const &activator):
        dimension_(layer_out),
        weights_(layer_out, layer_in, T(0), T(1)/sqrt((T)layer_in)),
        bias_(layer_out, T(0), T(1)),
        z_(layer_out, 0),
		a_prev_(layer_in, 0),
        nabla_w_(layer_out, layer_in, 0),
        nabla_b_(layer_out, 0),
        activator_(activator)
    { }

public:
    virtual vector_t<T> feedforward(vector_t<T> const &input) override {
        a_prev_ = input;
        // z = w*a + b
        z_ = dot(weights_, input).add(bias_);
        return activator_.activate(z_);
    }

    virtual vector_t<T> backpropagate(vector_t<T> const &error) override {
        // delta(l) = (w(l+1) * delta(l+1)) [X] activation_derivative(z(l))
        // (w(l+1) * delta(l+1)) comes as the gradient (error) from the "previous" layer
        vector_t<T> delta = activator_.activation_derivative(z_).element_mul(error);
        // dC/db = delta(l)
        nabla_b_.add(delta);
        // dC/dw = a(l-1) * delta(l)
        auto delta_nabla_w = dot_transpose(delta, a_prev_);
        nabla_w_.add(delta_nabla_w);
        // gradient for the next layer w(l+1) * delta(l+1)
        delta = transpose_dot(weights_, delta);
        return delta;
    }

    virtual void update_weights(train_strategy_t<T> const &strategy) override {
        strategy.update_bias(bias_, nabla_b_);
        strategy.update_weights(weights_, nabla_w_);
		nabla_b_.reset(0);
		nabla_w_.reset(0);
    }

private:
    // own data
    size_t dimension_;
    matrix_t<T> weights_;
    vector_t<T> bias_;
    activator_t<T> const &activator_;
    // calculation support
    vector_t<T> z_, a_prev_;
    matrix_t<T> nabla_w_;
    vector_t<T> nabla_b_;
};

#endif // FULLY_CONNECTED_LAYER_H
