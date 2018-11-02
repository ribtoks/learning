#ifndef CONVOLUTIONLAYER_H
#define CONVOLUTIONLAYER_H

#include <vector>
#include <cassert>

#include "common/array3d.h"
#include "common/array3d_math.h"
#include "common/shape.h"
#include "layers/layer_base.h"
#include "network/activator.h"
#include "strategy/train_strategy.h"

#define FILTER_DIM(input, filter, stride) (((input) - (filter))/(stride) + 1)

template<typename T>
class convolution_layer_t: public layer_base_t<T> {
public:
    convolution_layer_t(shape3d_t const &input_shape,
                        shape3d_t const &filter_shape,
                        size_t filters_number,
                        size_t stride_length,
                        activator_t<T> const &activator):
        input_shape_(input_shape),
        filter_shape_(filter_shape),
        conv_shape_(FILTER_DIM(input_shape.x(), filter_shape.x(), stride_length),
                    FILTER_DIM(input_shape.y(), filter_shape.y(), stride_length),
                    filters_number),
        stride_length_(stride_length),
        activator_(activator)
    {
        filter_weights_.reserve(filters_number);
        filter_biases_.reserve(filters_number);
        nabla_weights_.reserve(filters_number);
        nabla_biases_.reserve(filters_number);
        // all neurons in each filter share same weights and bias
        for (size_t i = 0; i < filters_number; i++) {
            filter_weights_.emplace_back(
                        filter_shape,
                        T(0), T(1)/sqrt((T)filter_shape.capacity()));
            nabla_weights_.emplace_back(filter_shape, 0);
            nabla_biases_.emplace_back(conv_shape_, 0);
        }
    }

public:
    virtual layer_input_t<T> feedforward(layer_input_t<T> const &input) override {
        assert(input.data.shape() == input_shape_);

        a_prev_ = input.data;
        array3d_t<T> result(conv_shape_, 0);

        const size_t fsize = filter_weights_.size();
        // perform convolution for each filter
        for (size_t i = 0; i < fsize; i++) {
            auto &bias = filter_biases_[i];
            // 2D loop over the input and calculation of standard a(l) = activation(a(l-1)*w(l) + b(l))
            // but we have convolution instead of matrix multiplication
            for (size_t y = 0; y <= input_shape_.y() - filter_shape_.y(); y += stride_length_) {
                for (size_t x = 0; x <= input_shape_.x() - filter_shape_.x(); x += stride_length_) {
                    // convolution is [w (x) a], where w is matrix of filter weights and a is input
                    result(x/stride_length_, y/stride_length_, i) =
                            bias(x/stride_length_, y/stride_length_) +
                            dot_1d(
                                input.data.slice(
                                    index3d_t(x, y, 0),
                                    index3d_t(x + filter_shape_.x() - 1,
                                              y + filter_shape_.y() - 1,
                                              input_shape_.z() - 1)).flatten(),
                                filter_weights_[i].flatten());
                }
            }
        }

        z_ = std::move(result);
        array3d_t<T> a = activator_.activate(z_);
        return layer_input_t<T>(a);
    }

    virtual layer_error_t<T> backpropagate(layer_error_t<T> const &error) override {
        assert(error.data.shape() == conv_shape_);
        // previous layer should have been max-pooling so data's shape should be same
        array3d_t<T> delta = activator_.activation_derivative(z_).element_mul(error.data);

        const size_t fsize = filter_weights_.size();
        auto &err_shape = error.data.shape();
        auto flat_errors = delta.flatten();
        // calculate nabla_w for each filter
        for (size_t i = 0; i < fsize; i++) {
            auto &nabla_w = nabla_weights_[i];
            auto &nabla_b = nabla_biases_[i];
            // dC/db = delta(l)
            nabla_b.add(delta);
            // dC/dw = a(l-1) (x) delta(l)
            // where (x) is a convolution operator instead of multiplication
            for (size_t y = 0; y < conv_shape_.y(); y += stride_length_) {
                for (size_t x = 0; x < conv_shape_.x(); x += stride_length_) {
                    nabla_w(x/stride_length_, y/stride_length_, i) +=
                            dot_1d(
                                a_prev_.data.slice(
                                    index3d_t(x, y, 0),
                                    index3d_t(x + conv_shape_.x() - 1,
                                              y + conv_shape_.y() - 1,
                                              conv_shape_.z() - 1)).flatten(),
                                flat_errors);
                }
            }
        }
    }

    virtual void update_weights(train_strategy_t<T> const &strategy) override {
        const size_t size = filter_weights_.size();
        for (size_t i = 0; i < size; i++) {
            strategy.update_weights(filter_weights_[i], nabla_weights_[i]);
            strategy.update_bias(filter_biases_[i], nabla_biases_[i]);
            nabla_weights_[i].reset(0);
            nabla_biases_[i].reset(0);
        }
    }

private:
    shape3d_t input_shape_;
    shape3d_t filter_shape_;
    shape3d_t conv_shape_;
    size_t stride_length_;
    std::vector<array3d_t<T>> filter_weights_;
    std::vector<array3d_t<T>> filter_biases_;
    activator_t<T> const &activator_;
    // calculation support
    array3d_t<T> a_prev_, z_;
    std::vector<array3d_t<T>> nabla_weights_;
    std::vector<array3d_t<T>> nabla_biases_;
};

#endif // CONVOLUTIONLAYER_H
