#ifndef NETWORK2_H
#define NETWORK2_H

#include <initializer_list>
#include <vector>
#include <tuple>
#include <memory>

#include "algorithm/optimization_algorithm.h"
#include "common/calculus_types.h"
#include "network/layers/fullyconnectedlayer.h"
#include "network/activator.h"

class network2_t {
public:
    using data_type = double;
    using v_d = vector_t<data_type>;
    using m_d = matrix_t<data_type>;
    using training_data = std::vector<std::tuple<v_d, v_d>>;
    using layer_type = std::shared_ptr<layer_base_t<data_type>>;

public:
    network2_t(std::initializer_list<layer_type> layers);

public:
    void train(const training_data &data,
               optimization_algorithm_t<data_type> const &algorithm,
               size_t epochs,
               size_t minibatch_size);

private:
    // feeds input a to the network and returns output
    v_d feedforward(v_d a);

    // evaluates number of correctly classified inputs (validation data)
    size_t evaluate(const training_data &data, const std::vector<size_t> &indices);

    // updates network weights and biases using one
    // iteration of gradient descent using mini_batch of inputs and outputs
    void update_mini_batch(const training_data &data,
                           const std::vector<size_t> &indices,
                           const optimization_algorithm_t<network2_t::data_type> &algorithm);

    // runs a loop of propagation of inputs and backpropagation of errors
    // back to the beginning with weights and biases updates as a result
    void backpropagate(v_d input, const v_d &result);

private:
    //  dimensions of layers
    std::vector<std::shared_ptr<layer_base_t<data_type>>> layers_;
};

#endif // NETWORK2_H
