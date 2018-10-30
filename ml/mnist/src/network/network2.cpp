#include "network2.h"

#include "network1.h"
#include <algorithm>
#include <numeric>
#include "common/calculus.h"
#include "common/cpphelpers.h"
#include "common/log.h"

network2_t::network2_t(std::initializer_list<network2_t::layer_type> layers):
    layers_(layers)
{ }

void network2_t::train(const network2_t::training_data &data,
                       const optimization_algorithm_t<network2_t::data_type> &algorithm,
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
            update_mini_batch(data, indices, algorithm);
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

network2_t::v_d network2_t::feedforward(network2_t::v_d a) {
    for (auto &layer: layers_) {
        a = layer->feedforward(a);
    }
    return a;
}

size_t network2_t::evaluate(const network2_t::training_data &data, const std::vector<size_t> &indices) {
    size_t count = 0;
    for (auto i: indices) {
        network2_t::v_d result = feedforward(std::get<0>(data[i]));
        assert(result.size() == std::get<1>(data[i]).size());
        if (argmax(result) == argmax(std::get<1>(data[i]))) { count++; }
    }
    return count;
}

void network2_t::update_mini_batch(const network2_t::training_data &data,
                                   const std::vector<size_t> &indices,
                                   const optimization_algorithm_t<network2_t::data_type> &algorithm) {
    for (auto i: indices) {
        auto &input = std::get<0>(data[i]);
        auto &result = std::get<1>(data[i]);

        backpropagate(input, result);
    }

	// update weights
	for (auto &layer : layers_) {
		layer->update_weights(algorithm);
	}
}

void network2_t::backpropagate(network2_t::v_d input, const network2_t::v_d &result) {
    const size_t layers_size = layers_.size();

    // feedforward input
    for (size_t i = 0; i < layers_size; i++) {
        input = layers_[i]->feedforward(input);
    }

    // backpropagate error
    network2_t::v_d error = result;
    for (size_t i = layers_size; i-- > 0;) {
        error = layers_[i]->backpropagate(error);
    }
}

