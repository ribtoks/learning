cmake_minimum_required(VERSION 3.6)

project(mnist_training CXX)

set (CMAKE_CXX_STANDARD 11)

set(SOURCES bmp_image.h bmp_image.cpp cpphelpers.h parsed_images.h parsed_images.cpp parsed_labels.h parsed_labels.cpp calculus.h network.h network.cpp mnist_dataset.h mnist_dataset.cpp main.cpp calculus.cpp calculus_types.h log.h log.cpp)

add_executable(mnist_training ${SOURCES})
