#include "network2.h"

#include <algorithm>
#include <numeric>

#include "common/array3d_math.h"
#include "common/cpphelpers.h"
#include "common/log.h"
#include "layers/input.h"
#include "layers/error.h"

network2_t::network2_t(std::initializer_list<network2_t::layer_type> layers):
    layers_(layers)
{ }

void network2_t::train(network2_t::training_data &data,
                       const train_strategy_t<data_type> &strategy,
                       size_t epochs,
                       size_t minibatch_size) {
    // big chunk of data is used for training while
    // small chunk - for validation after some epochs
    const size_t training_size = 5 * data.size() / 6;
    std::vector<size_t> eval_indices(data.size() - training_size);
    std::iota(eval_indices.begin(), eval_indices.end(), training_size);

    for (size_t j = 0; j < epochs; j++) {
        auto indices_batches = batch_indices(training_size, minibatch_size);
        for (auto &indices: indices_batches) {
            update_mini_batch(data, indices, strategy);
        }

        if (j % 2 == 0) {
            auto result = evaluate(data, eval_indices);
            log("Epoch %d: %d / %d", j, result, eval_indices.size());
        } else {
            log("Epoch %d ended", j);
        }
    }

    auto result = evaluate(data, eval_indices);
    log("End result: %d / %d", result, eval_indices.size());
}

network2_t::t_d network2_t::feedforward(network2_t::t_d const &a) {
    layer_input_t<network2_t::data_type> input(a);
    for (auto &layer: layers_) {
        input = layer->feedforward(input);
    }
    return input.data;
}

size_t network2_t::evaluate(const network2_t::training_data &data, const std::vector<size_t> &indices) {
    size_t count = 0;
    for (auto i: indices) {
        network2_t::t_d result = feedforward(std::get<0>(data[i]));
        assert(result.size() == std::get<1>(data[i]).size());
        if (argmax1d(result) == argmax1d(std::get<1>(data[i]))) { count++; }
    }
    return count;
}

void network2_t::update_mini_batch(network2_t::training_data &data,
                                   const std::vector<size_t> &indices,
                                   const train_strategy_t<data_type> &strategy) {
    for (auto i: indices) {
        auto &input = std::get<0>(data[i]);
        auto &result = std::get<1>(data[i]);

        backpropagate(input, result);
    }

    for (auto &layer: layers_) {
        layer->update_weights(strategy);
    }
}

void network2_t::backpropagate(t_d &x, t_d &result) {
    const size_t layers_size = layers_.size();
    layer_input_t<network2_t::data_type> input(x);

    // feedforward input
    for (size_t i = 0; i < layers_size; i++) {
        input = layers_[i]->feedforward(input);
    }

    // backpropagate error
    layer_error_t<network2_t::data_type> error(result);
    for (size_t i = layers_size; i-- > 0;) {
        error = layers_[i]->backpropagate(error);
    }
}

