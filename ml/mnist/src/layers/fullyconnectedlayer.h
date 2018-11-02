#ifndef FULLY_CONNECTED_LAYER_H
#define FULLY_CONNECTED_LAYER_H

#include <functional>

#include "strategy/train_strategy.h"
#include "common/array3d.h"
#include "common/array3d_math.h"
#include "common/log.h"
#include "network/activator.h"
#include "layers/layer_base.h"

template<typename T = double>
class fully_connected_layer_t: public layer_base_t<T> {
public:
    fully_connected_layer_t(size_t layer_in,
                            size_t layer_out,
                            activator_t<T> const &activator):
        dimension_(layer_out),
        weights_(
            shape3d_t(layer_in, layer_out, 1),
            T(0), T(1)/sqrt((T)layer_in)),
        bias_(
            shape3d_t(layer_out, 1, 1),
            T(0), T(1)),
        z_(shape3d_t(layer_out, 1, 1), 0),
        a_prev_(shape3d_t(layer_in, 1, 1), 0),
        nabla_w_(shape3d_t(layer_in, layer_out, 1), 0),
        nabla_b_(shape3d_t(layer_out, 1, 1), 0),
        activator_(activator)
    { }

public:
    virtual layer_input_t<T> feedforward(layer_input_t<T> const &input) override {
        a_prev_ = input.data;
        // z = w*a + b
        z_ = dot_2d_1d(weights_, a_prev_).add(bias_);
        auto a = activator_.activate(z_);
        return layer_input_t<T>(a);
    }

    virtual layer_error_t<T> backpropagate(layer_error_t<T> const &error) override {
        // delta(l) = (w(l+1) * delta(l+1)) [X] activation_derivative(z(l))
        // (w(l+1) * delta(l+1)) comes as the gradient (error) from the "previous" layer
        array3d_t<T> delta = activator_.activation_derivative(z_).element_mul(error.data);
        // dC/db = delta(l)
        nabla_b_.add(delta);
        // dC/dw = a(l-1) * delta(l)
        auto delta_nabla_w = dot_transpose(delta, a_prev_);
        nabla_w_.add(delta_nabla_w);
        // gradient for the next layer w(l+1) * delta(l+1)
        delta = transpose_dot(weights_, delta);
        return layer_error_t<T>(delta);
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
    array3d_t<T> weights_;
    array3d_t<T> bias_;
    activator_t<T> const &activator_;
    // calculation support
    array3d_t<T> z_, a_prev_;
    array3d_t<T> nabla_w_;
    array3d_t<T> nabla_b_;
};

#endif // FULLY_CONNECTED_LAYER_H
