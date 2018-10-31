#ifndef OPTIMIZATION_ALGORITHM_H
#define OPTIMIZATION_ALGORITHM_H

#include "common/calculus_types.h"

template <typename T>
class train_strategy_t {
public:
    virtual ~train_strategy_t() {}
    virtual void update_bias(vector_t<T> &b, vector_t<T> &nabla_b) const = 0;
    virtual void update_weights(matrix_t<T> &w, matrix_t<T> &nabla_w) const = 0;
};

#endif // OPTIMIZATION_ALGORITHM_H
