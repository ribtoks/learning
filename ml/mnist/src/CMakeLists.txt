cmake_minimum_required(VERSION 3.6)

project(mnist_training CXX)

set (CMAKE_CXX_STANDARD 11)

set(SOURCES
    parsing/bmp_image.h
    parsing/bmp_image.cpp
    common/cpphelpers.h
    common/cpphelpers.cpp
    parsing/parsed_images.h
    parsing/parsed_images.cpp
    parsing/parsed_labels.h
    parsing/parsed_labels.cpp
    common/calculus.h
    algorithm/sdg_algorithm.h
    algorithm/optimization_algorithm.h
    network/network2.h
    network/network2.cpp
    network/network1.h
    network/network1.cpp
    network/layers/fullyconnectedlayer.h
    network/layers/crossentropyoutputlayer.h
    network/layers/layer_base.h
    network/activator.h
    parsing/mnist_dataset.h
    parsing/mnist_dataset.cpp
    common/calculus.cpp
    common/calculus_types.h
    common/log.h
    common/log.cpp)

#add_executable(example1 ${SOURCES} example1.cpp)
add_executable(example2 ${SOURCES} example2.cpp)

#target_include_directories(example1 PRIVATE .)
target_include_directories(example2 PRIVATE .)
