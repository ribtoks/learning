#ifndef CROSSENTROPYOUPTUTLAYER_H
#define CROSSENTROPYOUPTUTLAYER_H

#include "common/calculus_types.h"
#include "network/activator.h"
#include "network/layers/layer_base.h"
#include "strategy/train_strategy.h"

template <typename T>
class crossentropy_output_layer_t : public layer_base_t<T> {
public:
    crossentropy_output_layer_t()
    { }

public:
    virtual vector_t<T> feedforward(vector_t<T> const &input) override {
        last_activation_ = input;
        return input;
    }

    virtual vector_t<T> backpropagate(vector_t<T> const &result) override {
        // delta(L) = cost_deriv [X] activation_deriv(z(L))
        // cross-entropy derivative is [a(x) - y]
        last_activation_.subtract(result);
        return last_activation_;
    }

    virtual void update_weights(train_strategy_t<T> const &) override {}

private:
    vector_t<T> last_activation_;
};

#endif // CROSSENTROPYOUPTUTLAYER_H
