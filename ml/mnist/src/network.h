#ifndef NETWORK_H
#define NETWORK_H

#include <initializer_list>
#include <vector>
#include <tuple>
#include "calculus.h"

class network_t {
public:
    using v_d = vector_t<double>;
    using m_d = matrix_t<double>;
    using io_pair = std::tuple<std::vector<v_d>, std::vector<m_d>>;
    using training_data = std::vector<std::tuple<v_d, double>>;
    
public:
    network_t(std::initializer_list<int> layers);

public:
    // train network using stochastic gradient descent
    void train_sdg(const training_data &data,
                   size_t epochs,
                   size_t mini_batch_size,
                   double eta);

private:
    // feeds input a to the network and returns output
    v_d feedforward(v_d a);
    // updates network weights and biases using one
    // iteration of gradient descent using mini_batch of inputs
    // each mini-batch is input and output
    void update_mini_batch(const training_data &data,
                           const std::vector<size_t> &indices,
                           double eta);

    void backpropagation(const v_d &input, double result,
                         std::vector<v_d> &nabla_b,
                         std::vector<m_d> &nabla_w);

private:
    std::vector<int> layers_;
    std::vector<v_d> biases_;
    std::vector<m_d> weights_;
};

#endif // NETWORK_H
