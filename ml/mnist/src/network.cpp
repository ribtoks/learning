#include "network.h"
#include <algorithm>

network_t::network_t(std::initializer_list<int> layers):
    layers_(layers)
{
    const size_t layers_count = layers_.size();

    biases.reserve(layers_count - 1);
    // first layer of neurons is an input
    // and doesn't have any biases
    for (size_t i = 1; i < layers_count; i++) {
        int dim = layers_[i];
        biases_.emplace_back(dim, 0.0, 1.0);
    }

    weights_reserve(layers_count - 1);
    // weights_[i] means weights between
    // layer (i) and layer (i+1)
    // let w be this matrix weights_[i]
    // then w[j][k] means weight of connection
    // between k-th neuron in layer (i) and
    // j-th neuron in layer (i+1)
    for (size_t i = 0; i < layers_count - 1; i++) {
        size_t height = layers_[i+1];
        size_t width = layers_[i];
        weights_.emplace_back(height, width, 0.0, 1.0);
    }

    assert(weights_.size() == biases_.size());
}

std::vector<std::vector<size_t>> batch_indices(size_t size, size_t batch_size) {
    std::vector<size_t> indices(size, 0);
    for (size_t i = 0; i < size; i++) { idices[i] = i; }
    std::random_shuffle(indices.begin(), indices.end());

    std::vector<std::vector<size_t>> batches;
    std::vector<size_t> batch;
    batch.reserve(batch_size);

    for (auto &i: indices) {
        if (batch.size() < batch_size) {
            batch.push_back(i);
        } else {
            batches.emplace_back(std::move(batch));
            batch.reserve(batch_size);
        }
    }

    if (batch.size() > 0) {
        batches.emplace_back(std::move(batch));
    }    
    
    return batches;
}

void network_t::train_sdg(const training_data &data,
                          size_t epochs,
                          size_t minibatch_size,
                          double eta) {
    for (size_t j = 0; j < epochs; j++) {
        auto indices_batches = batch_indices(data.size(), minibatch_size);
        for (auto &indices: indices_batches) {
            update_mini_batch(data, indices, eta);
        }
    }
}

network_t::v_d network_t::feedforward(network_t::v_d a) {
    assert(weights_.size() == biases_.size());
    const size_t size = weights_.size();
    for (size_t i = 0; i < size; i++) {
        // a = sigma(w*a + b)
        a = weights_[i].dot(a).add(biases_[i]).apply(sigmoid);
    }
    return a;
}

void network_t::update_mini_batch(const network_t::training_data &data,
                                  const std::vector<size_t> &indices,
                                  double eta) {
    // initialize
    
    std::vector<network_t::v_d> nabla_b, delta_nabla_b;
    nabla_b.reserve(biases_.size());
    delta_nabla_b.reserve(biases_.size());
    for (auto &b: biases_) {
        nabla_b.emplace_back(b.size(), 0);
        delta_nabla_b.emplace_back(b.size(), 0);
    }

    std::vector<network_t::m_d> nabla_w, delta_nabla_w;
    nabla_w.reserve(weights_.size());
    delta_nabla_w.reserve(weights_.size());
    for (auto &w: weights_) {
        nabla_w.emplace_back(w.height(), w.width(), 0);
        delta_nabla_w.emplace_back(w.height(), w.width(), 0);
    }

    // end initialize

    for (auto i: indices) {
        auto &input = std::get<0>(data[i]);
        auto &result = std::get<1>(data[i]);

        for (auto &dnb: delta_nabla_b) { dnb.reset(0.0); }
        for (auto &dnw: delta_nabla_w) { dnw.reset(0.0); }

        backpropagation(input, result, delta_nabla_b, delta_nabla_w);

        for (size_t i = 0; i < layers_size; i++) {
            nabla_b[i].add(delta_nabla_b[i]);
            nabla_w[i].add(delta_nabla_w[i]);
        }
    }

    double minibatch_size = indices.size() + 0.0;
    double scale = eta/minibatch_size;

    // w = w - eta/minibatch_size * gradient_w
    for (size_t i = 0; i < layers_size; i++) {
        biases_[i].add(nabla_b[i].mul(-scale));
        weights_[i].add(nabla_w[i].mul(-scale));
    }
}

void network_t::backpropagation(const network_t::v_d &input, double result,
                                std::vector<network_t::v_d> &nabla_b,
                                std::d::vector<network_t::m_d> &nabla_w) {
    std::vector<network_t::v_d> zs;
    std::vector<network_t::v_d> activations { input };
    const size_t layers_count = layers_.size();
    for (size_t i = 0; i < layers_count; i++) {
        auto &last_activation = activations.back();
        auto z = weights_[i].dot(last_activation).add(biases_[i]);
        zs.push_back(z);
        activations.push_back(z.apply(sigmoid));
    }

    
}
