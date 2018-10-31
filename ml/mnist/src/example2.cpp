#include <memory>
#include <string>
#include <initializer_list>

#include "algorithm/sdg_algorithm.h"
#include "common/calculus.h"
#include "common/calculus_types.h"
#include "network/activator.h"
#include "network/layers/crossentropyoutputlayer.h"
#include "network/layers/fullyconnectedlayer.h"
#include "network/network2.h"
#include "parsing/mnist_dataset.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Data root not specified through the command line");
    }
    
    std::string data_root(argv[1]);
    mnist_dataset_t mnist_dataset(data_root);

    size_t mini_batch_size = 10;
    double learning_rate = 0.1;
    double decay_rate = 20.0;

    auto training_data = mnist_dataset.training_data();
    activator_t<double> sigmoid_activator(sigmoid_v, sigmoid_derivative_v);
    activator_t<double> last_layer_activator(sigmoid_v,
                                             [](vector_t<double> const &x){return vector_t<double>(x.size(), 1.0);});
    sdg_algorithm_t<double> sdg_algorithm(mini_batch_size,
                                          training_data.size(),
                                          decay_rate,
                                          learning_rate);
    network2_t network(
                std::initializer_list<network2_t::layer_type>(
    {
                        std::make_shared<fully_connected_layer_t<double>>(28*28, 30, sigmoid_activator),
                        std::make_shared<fully_connected_layer_t<double>>(30, 10, last_layer_activator),
                        std::make_shared<crossentropy_output_layer_t<double>>(sigmoid_activator)}));

    size_t epochs = 30;

    network.train(training_data,
                  sdg_algorithm,
                  epochs,
                  mini_batch_size);
    
    return 0;
}
