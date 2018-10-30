#ifndef SDG_ALGO_H
#define SDG_ALGO_H

#include "algorithm/optimization_algorithm.h"

template <typename T>
class sdg_algorithm_t: public optimization_algorithm_t<T> {
public:
    sdg_algorithm_t(size_t minibatch_size,
                    size_t input_size,
                    T decay_rate,
                    T learning_rate):
        minibatch_size_(minibatch_size),
        input_size_(input_size),
        weight_decay_(decay_rate),
        learning_rate_(learning_rate)
    {}

public:
    virtual void update_bias(vector_t<T> &v, vector_t<T> &nabla_v) const override {
        T scale = learning_rate_ / (T)minibatch_size_;
        v.add(nabla_v.mul(-scale));
    }

    virtual void update_weights(matrix_t<T> &w, matrix_t<T> &nabla_w) const override {
        T scale = learning_rate_;
        T decay = T(1) - learning_rate_*weight_decay_ / (T)input_size_;
        w.mul(weight_decay_).add(nabla_w.mul(-scale));
    }

private:
    size_t minibatch_size_;
    size_t input_size_;
    T weight_decay_;
    T learning_rate_;
};

#endif // SDG_ALGO_H
