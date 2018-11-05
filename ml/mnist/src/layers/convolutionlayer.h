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
    enum struct padding_type {
        valid, // only positions where the kernel lies entirely within the image
        same  // output is equal in size to the input
    };

public:
    convolution_layer_t(shape3d_t const &input_shape,
                        shape3d_t const &filter_shape,
                        size_t filters_number,
                        size_t stride_length,
                        padding_type padding,
                        activator_t<T> const &activator):
        input_shape_(input_shape),
        filter_shape_(filter_shape),
        conv_shape_(FILTER_DIM(input_shape.x(), filter_shape.x(), stride_length),
                    FILTER_DIM(input_shape.y(), filter_shape.y(), stride_length),
                    filters_number),
        filter_biases_(shape_row(filters_number), T(0), T(1)),
        stride_length_(stride_length),
        padding_(padding),
        activator_(activator)
    {
        filter_weights_.reserve(filters_number);
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
    virtual array3d_t<T> feedforward(array3d_t<T> const &input) override {
        assert(input.data.shape() == input_shape_);

        a_prev_ = input.clone();
        shape3d_t const &output_shape = (padding_ == padding_type::valid ? conv_shape_ : input_shape_);
        array3d_t<T> result(output_shape, 0);

        size_t pad_x = get_horizontal_padding();
        size_t pad_y = get_vertical_padding();

        const size_t fsize = filter_weights_.size();
        // perform convolution for each filter
        for (size_t i = 0; i < fsize; i++) {
            // 2D loop over the input and calculation convolution of input and current filter
            // convolution is S(i, j) = (I ∗ K)(i, j) = Sum[ I(m, n)K(i − m, j − n) ]
            // which is commutative i.e. (I ∗ K)(i, j) = Sum[ I(i - m, j - n)K(m, n) ]
            // where I is input and K is kernel (filter weights)
            for (size_t y = 0; y < output_shape.y(); y++) {
                size_t yi = y * stride_length_ - pad_y;
                for (size_t x = 0; x < output_shape.x(); x++) {
                    size_t xi = x * stride_length_ - pad_x;
                    // in this case cross-correlation (I(m, n)K(i + m, j + n)) is used
                    // (kernel is not rot180() flipped for the convolution, not commutative)
                    result(x, y, i) =
                            filter_biases_(i) +
                            dot(
                                input.data.slice(
                                    index3d_t(xi, yi, 0),
                                    index3d_t(xi + filter_shape_.x() - 1,
                                              yi + filter_shape_.y() - 1,
                                              input_shape_.z() - 1)),
                                filter_weights_[i]);
                }
            }
        }

        z_ = std::move(result);
        array3d_t<T> a = activator_.activate(z_);
        return array3d_t<T>(a);
    }

    virtual array3d_t<T> backpropagate(layer_error_t<T> const &error) override {
        assert(error.data.shape() == conv_shape_);
        // TODO: fix this code


        // previous layer should have been max-pooling so data's shape should be same
        array3d_t<T> delta = activator_.derivative(z_).element_mul(error.data);

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
                            dot(
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
    size_t get_vertical_padding() const {
        return (output_shape.y() - conv_shape_.y()) / 2;
    }

    size_t get_horizontal_padding() const {
        return (output_shape.x() - conv_shape_.x()) / 2;
    }

private:
    shape3d_t input_shape_;
    shape3d_t filter_shape_;
    // shape which is the result of the convolution of image and filter
    shape3d_t conv_shape_;
    size_t stride_length_;
    padding_type padding_;
    std::vector<array3d_t<T>> filter_weights_;
    array3d_t<T> filter_biases_;
    activator_t<T> const &activator_;
    // calculation support
    array3d_t<T> a_prev_, z_;
    std::vector<array3d_t<T>> nabla_weights_;
    std::vector<array3d_t<T>> nabla_biases_;
};

#endif // CONVOLUTIONLAYER_H
