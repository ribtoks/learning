#ifndef MNIST_DATASET_H
#define MNIST_DATASET_H

#include <string>
#include <vector>
#include "calculus_types.h"

class mnist_dataset_t {
public:
    mnist_dataset_t(const std::string &data_root);

public:
    std::vector<std::tuple<vector_t<double>, vector_t<double>>> training_data(int limit=-1);

private:
    std::string data_root_;
};

#endif // MNIST_DATASET_H
