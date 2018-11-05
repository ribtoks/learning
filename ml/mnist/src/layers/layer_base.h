#ifndef ILAYER_H
#define ILAYER_H

#include "common/array3d.h"

template<typename T>
class train_strategy_t;

template<typename T>
class layer_base_t {
public:
    virtual ~layer_base_t() {}
    virtual array3d_t<T> feedforward(array3d_t<T> const &input) = 0;
    virtual array3d_t<T> backpropagate(array3d_t<T> const &error) = 0;
    virtual void update_weights(train_strategy_t<T> const &) = 0;
};


#endif // ILAYER_H
