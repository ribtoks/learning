#ifndef CONVOLUTIONLAYER_H
#define CONVOLUTIONLAYER_H

#include "common/calculus_types.h"
#include "layers/layer_base.h"

template<typename T>
class convolution_layer_t: public layer_base_t<T> {
public:
    convolution_layer_t(shape_t input_shape,
                        shape_t filter_shape,
                        size_t stride_length)
    {}

public:
    virtual vector_t<T> feedforward(vector_t<T> const &input) override {

    }
    virtual vector_t<T> backpropagate(vector_t<T> const &error) override {

    }
    virtual void update_weights(train_strategy_t<T> const &) override {

    }
};

#endif // CONVOLUTIONLAYER_H
