#ifndef POOLINGLAYER_H
#define POOLINGLAYER_H

#include "layers/layer_base.h"

#define POOL_DIM(input, pool, stride) (((input) - (pool))/(stride) + 1)

template <typename T>
class pooling_layer_t: public layer_base_t<T> {
public:
    pooling_layer_t(size_t window_size,
                    size_t stride_length):
        window_size_(window_size),
        stride_length_(stride_length)
    { }

public:
    virtual layer_input_t<T> feedforward(layer_input_t<T> const &input) override {
        auto &in_shape = input.data.shape();
        array3d_t<T> result(
                    shape3d_t(POOL_DIM(in_shape.x(), window_size_, stride_length_),
                              POOL_DIM(in_shape.y(), window_size_, stride_length_),
                              in_shape.z()),
                    0);

        // pooling layer does max-pooling selecting a maximum activation
        // within the bounds of it's window
        for (size_t z = 0; z < in_shape.z(); z++) {
            for (size_t y = 0; y <= in_shape.y() - window_size_; y += window_size_) {
                for (size_t x = 0; x < in_shape.x() - window_size_; x += window_size_) {
                    result(x/window_size_, y/window_size_, z) =
                            input.data.slice(
                                index3d_t(x, y, z),
                                index3d_t(x + window_size_ - 1,
                                          y + window_size_ - 1,
                                          z)).max();
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
    size_t window_size_;
    size_t stride_length_;
};

#endif // POOLINGLAYER_H
