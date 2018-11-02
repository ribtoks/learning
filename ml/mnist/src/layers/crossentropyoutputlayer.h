#ifndef CROSSENTROPYOUPTUTLAYER_H
#define CROSSENTROPYOUPTUTLAYER_H

#include "network/activator.h"
#include "layers/layer_base.h"
#include "strategy/train_strategy.h"

template <typename T>
class crossentropy_output_layer_t : public layer_base_t<T> {
public:
    crossentropy_output_layer_t()
    { }

public:
    virtual layer_input_t<T> feedforward(layer_input_t<T> const &input) override {
        last_activation_ = input.data;
        return input;
    }

    virtual layer_error_t<T> backpropagate(layer_error_t<T> const &result) override {
        // delta(L) = cost_deriv [X] activation_deriv(z(L))
        // cross-entropy derivative is [a(x) - y]
        last_activation_.subtract(result.data);
        return layer_error_t<T>(last_activation_);
    }

    virtual void update_weights(train_strategy_t<T> const &) override {}

private:
    array3d_t<T> last_activation_;
};

#endif // CROSSENTROPYOUPTUTLAYER_H
