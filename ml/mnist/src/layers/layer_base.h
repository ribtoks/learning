#ifndef ILAYER_H
#define ILAYER_H

#include "layers/input.h"
#include "layers/error.h"

template<typename T>
class train_strategy_t;

template<typename T>
class layer_base_t {
public:
    virtual ~layer_base_t() {}
    virtual layer_input_t<T> feedforward(layer_input_t<T> const &input) = 0;
    virtual layer_error_t<T> backpropagate(layer_error_t<T> const &error) = 0;
    virtual void update_weights(train_strategy_t<T> const &) = 0;
};


#endif // ILAYER_H
