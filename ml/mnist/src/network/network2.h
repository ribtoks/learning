#ifndef NETWORK2_H
#define NETWORK2_H

#include <initializer_list>
#include <vector>
#include <tuple>
#include "common/calculus_types.h"
#include "network/layers/fullyconnectedlayer.h"
#include "network/activator.h"

class network2_t {
public:
    using data_type = double;
    using v_d = vector_t<data_type>;
    using m_d = matrix_t<data_type>;
    using training_data = std::vector<std::tuple<v_d, v_d>>;

public:
    network2_t(std::initializer_list<fully_connected_layer_t<data_type>> layers,
               activator_t<data_type> const &activator);

public:

private:
    // feeds input a to the network and returns output
    v_d feedforward(v_d a);

    // updates network weights and biases using one
    // iteration of gradient descent using mini_batch of inputs and outputs
    void update_mini_batch(const training_data &data,
                           const std::vector<size_t> &indices,
                           double eta,
                           double lambda);

    // runs a loop of propagation of inputs and backpropagation of errors
    // back to the beginning with weights and biases updates as a result
    void backpropagate(v_d input, const v_d &result);

private:
    //  dimensions of layers
    std::vector<fully_connected_layer_t<data_type>> layers_;
    activator_t<data_type> activator_;
};

#endif // NETWORK2_H
