#include "network.h"
#include <algorithm>

std::vector<network_t::v_d> copy_shapes(const std::vector<network_t::v_d> &from) {
    std::vector<network_t::v_d> to;
    to.reserve(from.size());

    for (auto &v: from) {
        to.emplace_back(v.size(), 0);
    }

    return to;
}

std::vector<network_t::m_d> copy_shapes(const std::vector<network_t::m_d> &from) {
    std::vector<network_t::m_d> to;
    to.reserve(from.size());

    for (auto &m: from) {
        to.emplace_back(m.height(), m.width(), 0);
    }

    return to;
}

network_t::network_t(std::initializer_list<int> layers):
    layers_(layers)
{
    const size_t layers_count = layers_.size();

    biases_.reserve(layers_count - 1);
    // first layer of neurons is an input
    // and doesn't have any biases
    for (size_t i = 1; i < layers_count; i++) {
        int dim = layers_[i];
        biases_.emplace_back(dim, 0.0, 1.0);
    }

    weights_.reserve(layers_count - 1);
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
    for (size_t i = 0; i < size; i++) { indices[i] = i; }
    std::random_shuffle(indices.begin(), indices.end());

    std::vector<std::vector<size_t>> batches;
    for(size_t i = 0; i < size; i += batch_size) {
        auto last = std::min(size, i + batch_size);
        batches.emplace_back(indices.begin() + i, indices.begin() + last);
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
    auto nabla_b = copy_shapes(biases_);
    auto nabla_w = copy_shapes(weights_);

    auto delta_nabla_b = copy_shapes(biases_);
    auto delta_nabla_w = copy_shapes(weights_);

    const size_t layers_size = layers_.size();
    for (auto i: indices) {
        auto &input = std::get<0>(data[i]);
        auto &result = std::get<1>(data[i]);

        for (auto &dnb: delta_nabla_b) { dnb.reset(0.0); }
        for (auto &dnw: delta_nabla_w) { dnw.reset(0.0); }

        backpropagate(input, result, delta_nabla_b, delta_nabla_w);

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

void network_t::backpropagate(const network_t::v_d &input, double result,
                              std::vector<network_t::v_d> &nabla_b,
                              std::vector<network_t::m_d> &nabla_w) {
    std::vector<network_t::v_d> zs;
    std::vector<network_t::v_d> activations { input };
    const size_t layers_count = layers_.size();

    for (size_t i = 0; i < layers_count; i++) {
        auto &last_activation = activations.back();
        auto z = weights_[i].dot(last_activation).add(biases_[i]);
        zs.push_back(z);
        activations.push_back(z.apply(sigmoid));
    }

    auto delta = cost_derivative(activations.back(), zs.back())
        .element_mul(zs.back().apply(sigmoid_derivative));
    
    nabla_b[layers_count - 1] = delta;
    nabla_w[layers_count - 1] = delta.dot_transpose(activations[layers_count - 2]);

    for (size_t i = 2; i < layers_count; i++) {
        size_t j = layers_count - 1 - i;
        delta = weights_[j + 1].transpose_dot(delta)
            .element_mul(
                zs[j].apply(sigmoid_derivative));
        nabla_b[j] = delta;
        nabla_w[j] = delta.dot_transpose(activations[j - 1]);
    }
}

network_t::v_d network_t::cost_derivative(const v_d &actual, const v_d &expected) {
    network_t::v_d result(actual);
    result.subtract(expected);
    return result;
}
