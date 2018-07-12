#include <string>
#include <exception>
#include "mnist_dataset.h"
#include "network.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Data root not specified through the command line");
    }
    
    std::string data_root(argv[1]);
    mnist_dataset_t mnist_dataset(data_root);
    network_t network({784, 30, 10});
    size_t epochs = 30;
    size_t mini_batch_size = 10;
    double learning_rate = 3.0;

    auto training_data = mnist_dataset.training_data();
    network.train_sgd(training_data,
                      epochs,
                      mini_batch_size,
                      learning_rate);
    
    return 0;
}
