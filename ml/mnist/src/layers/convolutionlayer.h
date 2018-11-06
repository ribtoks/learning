#ifndef CONVOLUTIONLAYER_H
#define CONVOLUTIONLAYER_H

#include <vector>
#include <cassert>
#include <cmath>

#include "common/array3d.h"
#include "common/array3d_math.h"
#include "common/shape.h"
#include "common/utils.h"
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
        // shape of the result of convolution of input and kernel/filter
        conv_shape_(FILTER_DIM(input_shape.x(), filter_shape.x(), stride_length),
                    FILTER_DIM(input_shape.y(), filter_shape.y(), stride_length),
                    filters_number),
        stride_length_(stride_length),
        padding_(padding),
        activator_(activator)
    {
        assert(filter_shape.z() == input_shape.z());
        filter_weights_.reserve(filters_number);
        filter_biases_.reserve(filters_number);
        nabla_weights_.reserve(filters_number);
        nabla_biases_.reserve(filters_number);
        // all neurons in each filter share same weights and bias
        for (size_t i = 0; i < filters_number; i++) {
            filter_weights_.emplace_back(
                        filter_shape,
                        T(0), T(1)/sqrt((T)filter_shape.capacity()));
            filter_biases_.emplace_back(shape_row(1), 0);
            nabla_weights_.emplace_back(filter_shape, 0);
            nabla_biases_.emplace_back(conv_shape_, 0);
        }
    }

public:
    virtual array3d_t<T> feedforward(array3d_t<T> const &input) override {
        // TODO: add matrix multiplication implementation variation
        return feedforward_loop(input);
    }

    virtual array3d_t<T> backpropagate(array3d_t<T> const &error) override {
        // TODO: add matrix multiplication implementation variation
        return backpropagate_loop(error);
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
    array3d_t<T> feedforward_loop(array3d_t<T> const &input) override {
        assert(input.shape() == input_shape_);

        input_ = input.clone();
        const shape3d_t output_shape = get_output_shape();
        array3d_t<T> result(output_shape, 0);

        const int pad_x = get_left_padding();
        const int pad_y = get_top_padding();

        const size_t fsize = filter_weights_.size();
        // perform convolution for each filter
        for (size_t i = 0; i < fsize; i++) {
            // 2D loop over the input and calculation convolution of input and current filter
            // convolution is S(i, j) = (I ∗ K)(i, j) = Sum[ I(m, n)K(i − m, j − n) ]
            // which is commutative i.e. (I ∗ K)(i, j) = Sum[ I(i - m, j - n)K(m, n) ]
            // where I is input and K is kernel (filter weights)
            for (size_t y = 0; y < output_shape.y(); y++) {
                int yi = y * stride_length_ - pad_y;

                for (size_t x = 0; x < output_shape.x(); x++) {
                    int xi = x * stride_length_ - pad_x;
                    // in this case cross-correlation (I(m, n)K(i + m, j + n)) is used
                    // (kernel is not rot180() flipped for the convolution, not commutative)
                    // previous formula (w*x + b) is used with convolution instead of product
                    result(x, y, i) =
                            filter_biases_(i) +
                            dot(
                                input.slice(
                                    index3d_t(xi, yi, 0),
                                    index3d_t(xi + filter_shape_.x() - 1,
                                              yi + filter_shape_.y() - 1,
                                              input_shape_.z() - 1)),
                                filter_weights_[i]);
                }
            }
        }

        output_ = std::move(result);
        return activator_.activate(output_);
    }

    array3d_t<T> backpropagate_loop(array3d_t<T> const &error) override {
        // error shape was already transformed in the prev layer as delta(l+1)(*)rot180(w(l+1))
        assert(error.shape() == output_.shape());
        array3d_t<T> delta;
        delta = activator_.derivative(output_); delta.element_mul(error);

        const int pad_x = get_left_padding();
        const int pad_y = get_top_padding();

        const size_t fsize = filter_weights_.size();
        auto &error_shape = error.shape();
        // calculate nabla_w for each filter
        for (size_t i = 0; i < fsize; i++) {
            auto &nabla_w = nabla_weights_[i];
            auto &nabla_b = nabla_biases_[i];
            for (int y = 0; y < filter_shape_.y(); y++) {
                int yi = y - pad_y;

                for (int x = 0; x < filter_shape_.x(); x++) {
                    int xi = x - pad_x;

                    // dC/db = delta(l)
                    nabla_b(i) += delta(x, y, i);
                    // dC/dw = a(l-1) (x) delta(l)
                    // weight (x, y) has affected the whole "error_shape" of values
                    // therefore it's gradient should include their deltas
                    nabla_w(x, y, i) +=
                            dot(
                                input_.slice(
                                    index3d_t(xi, yi, 0),
                                    index3d_t(xi + error_shape.x() - 1,
                                              yi + error_shape.y() - 1,
                                              error_shape.z() - 1)),
                                delta);
                }
            }
        }

        const int weight_pad_x = utils::get_left_padding(error_shape, filter_shape_, stride_length_);
        const int weight_pad_y = utils::get_top_padding(error_shape, filter_shape_, stride_length_);
        // gradient for the next layer = delta(l) (*) rot180(w(l)
        // with 'full' convolution (http://www.johnloomis.org/ece563/notes/filter/conv/convolution.html)
        array3d_t<T> delta_next(input_shape_, T(0));
        // for each filter
        for (size_t i = 0; i < fsize; i++) {
            auto &weights = filter_weights_[i];

            for (int z = 0; z < input_shape_.z(); z++) {
                for (int y = 0; y < input_shape_.y(); y++) {
                    int yi = y - weight_pad_y;

                    for (int x = 0; x < input_shape_.x(); x++) {
                        int xi = x - weight_pad_x;

                        delta_next(x, y, z) +=
                                dot(
                                    weights.slice(
                                        index3d_t(xi, yi, 0),
                                        index3d_t(xi + error_shape.x() - 1,
                                                  yi + error_shape.y() - 1,
                                                  error_shape.z() - 1)),
                                    delta);
                    }
                }
            }
        }

        return delta_next;
    }

    shape3d_t get_output_shape() const {
        if (padding_ == padding_type::valid) { return conv_shape_; }

        double width = ceil(double(input_shape_.x()) / double(stride_length_));
        double height = ceil(double(input_shape_.y()) / double(stride_length_));
        return shape3d_t((size_t)width, (size_t)height, filter_weights_.size());
    }

    int get_top_padding() const {
        if (padding_ == padding_type::valid) { return 0; }

        return utils::get_top_padding(input_shape_, filter_shape_, stride_length_);
    }

    int get_left_padding() const {
        if (padding_ == padding_type::valid) { return 0; }

        return utils::get_left_padding(input_shape_, filter_shape_, stride_length_);
    }

private:
    shape3d_t input_shape_;
    shape3d_t filter_shape_;
    // shape which is the result of the convolution of image and filter
    shape3d_t conv_shape_;
    size_t stride_length_;
    const padding_type padding_;
    activator_t<T> const &activator_;
    std::vector<array3d_t<T>> filter_weights_;
    std::vector<array3d_t<T>> filter_biases_;
    // calculation support
    array3d_t<T> input_, output_;
    std::vector<array3d_t<T>> nabla_weights_;
    std::vector<array3d_t<T>> nabla_biases_;
};

#endif // CONVOLUTIONLAYER_H
