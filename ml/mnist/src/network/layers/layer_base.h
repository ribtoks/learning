#ifndef ILAYER_H
#define ILAYER_H

#include "common/calculus_types.h"

template<typename T>
class optimization_algorithm_t;

template<typename T>
class layer_base_t {
public:
    virtual ~layer_base_t() {}
    virtual vector_t<T> feedforward(vector_t<T> const &input) = 0;
    virtual vector_t<T> backpropagate(vector_t<T> const &error) = 0;
    virtual void update_weights(optimization_algorithm_t<T> const &) = 0;
};


#endif // ILAYER_H
