#ifndef MNIST_DATASET_H
#define MNIST_DATASET_H

#include <string>
#include <vector>
#include "common/calculus_types.h"

class mnist_dataset_t {
public:
    mnist_dataset_t(const std::string &data_root);

public:
    // returns mnist dataset as vector of tuples with inputs and vectorized output
    // input is normalized pixel data and output is vector of zeros with the only
    // "one" on the index of corresponding digit (0-9) which is encoded by the image
    std::vector<std::tuple<vector_t<double>, vector_t<double>>> training_data(int limit=-1);
    void save_as_images(int limit = -1);

private:
    std::string data_root_;
};

#endif // MNIST_DATASET_H
