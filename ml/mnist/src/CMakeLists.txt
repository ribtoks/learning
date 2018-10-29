cmake_minimum_required(VERSION 3.6)

project(mnist_training CXX)

set (CMAKE_CXX_STANDARD 11)

set(SOURCES
    parsing/bmp_image.h
    parsing/bmp_image.cpp
    common/cpphelpers.h
    parsing/parsed_images.h
    parsing/parsed_images.cpp
    parsing/parsed_labels.h
    parsing/parsed_labels.cpp
    common/calculus.h
    network/network2.h
    network/network2.cpp
    network/network1.h
    network/network1.cpp
    network/layers/fullyconnectedlayer.h
    network/layers/layer_base.h
    network/layers/inputlayer.h
    network/activator.h
    parsing/mnist_dataset.h
    parsing/mnist_dataset.cpp
    main.cpp
    common/calculus.cpp
    common/calculus_types.h
    common/log.h
    common/log.cpp)

add_executable(mnist_training ${SOURCES})

target_include_directories(mnist_training PRIVATE .)
