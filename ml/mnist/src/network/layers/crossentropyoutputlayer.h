#ifndef CROSSENTROPYOUPTUTLAYER_H
#define CROSSENTROPYOUPTUTLAYER_H

#include "common/calculus_types.h"
#include "network/activator.h"
#include "network/layers/layer_base.h"

template <typename T>
class crossentropy_output_layer_t : public layer_base_t<T> {
public:
    crossentropy_output_layer_t(activator_t<T> const &activator):
        activator_(activator)
    { }

public:
    virtual vector_t<T> const &feedforward(vector_t<T> const &input) override {
        last_activation_ = input;
        return input;
    }

    virtual vector_t<T> backpropagate(vector_t<T> const &result) override {
        // delta(L) = cost_deriv [X] activation_deriv(z(L))
        // cross-entropy derivative is [a(x) - y]
        last_activation_.subtract(result);
        return last_activation_;
    }

    virtual void update_weights(optimization_algorithm_t<T> const &) override {}
	virtual void reset() override {}

private:
    activator_t<T> const &activator_;
    vector_t<T> last_activation_;
};

#endif // CROSSENTROPYOUPTUTLAYER_H
