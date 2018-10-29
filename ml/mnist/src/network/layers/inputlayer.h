#ifndef INPUTLAYER_H
#define INPUTLAYER_H

#include "network/layers/layer_base.h"

template <typename T>
class input_layer_t: public layer_base_t<T>
{
public:
    input_layer_t(size_t size):
        size_(size)
    { }

public:
    virtual vector_t<T> const &feedforward(vector_t<T> const &input) override {

    }

    virtual vector_t<T> backpropagate(vector_t<T> const &error) override {

    }

private:
    size_t size_;
};

#endif // INPUTLAYER_H
