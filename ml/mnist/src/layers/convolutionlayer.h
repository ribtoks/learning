#ifndef CONVOLUTIONLAYER_H
#define CONVOLUTIONLAYER_H

#include <vector>

#include "common/array3d.h"
#include "common/array3d_math.h"
#include "common/shape.h"
#include "layers/layer_base.h"

#define FILTER_DIM(input, filter, stride) (((input) - (filter))/(stride) + 1)

template<typename T>
class convolution_layer_t: public layer_base_t<T> {
public:
    convolution_layer_t(shape3d_t filter_shape,
                        size_t filters_number,
                        size_t stride_length):
        filter_shape_(filter_shape),
        stride_length_(stride_length)
    {
        filters_.reserve(filters_number);
        // all neurons in each filter share same weights and bias
        for (size_t i = 0; i < filters_number; i++) {
            filters_.emplace_back(
                        filter_shape,
                        T(0), T(1)/sqrt((T)filter_shape.capacity()));
        }
    }

public:
    virtual layer_input_t<T> feedforward(layer_input_t<T> const &input) override {
        a_prev_ = input.data;
        auto &in_shape = input.data.shape();

        array3d_t<T> result(
                    shape3d_t(FILTER_DIM(in_shape.x(), filter_shape_.x(), stride_length_),
                              FILTER_DIM(in_shape.y(), filter_shape_.y(), stride_length_),
                              filters_.size()),
                    0);

        const size_t fsize = filters_.size();
        // perform convolution for each filter
        for (size_t i = 0; i < fsize; i++) {
            for (size_t y = 0; y <= in_shape.y() - filter_shape_.y(); y += stride_length_) {
                for (size_t x = 0; x <= in_shape.x() - filter_shape_.x(); y += stride_length_) {
                    result(x/stride_length_, y/stride_length_, i) =
                            dot_1d(
                                input.data.slice(
                                    index3d_t(x, y, 0),
                                    index3d_t(x + filter_shape_.x() - 1,
                                              y + filter_shape_.y() - 1,
                                              in_shape.z() - 1)).flatten(),
                                filters_[i].flatten());
                }
            }
        }

        return layer_input_t<T>(result);
    }

    virtual layer_error_t<T> backpropagate(layer_error_t<T> const &error) override {

    }

    virtual void update_weights(train_strategy_t<T> const &) override {

    }

private:
    shape3d_t filter_shape_;
    size_t stride_length_;
    array3d_t<T> a_prev_;
    std::vector<array3d_t<T>> filters_;
};

#endif // CONVOLUTIONLAYER_H
