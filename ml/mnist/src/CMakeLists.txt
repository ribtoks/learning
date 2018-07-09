cmake_minimum_required(VERSION 3.6)

project(parse_test CXX)

set (CMAKE_CXX_STANDARD 11)

set(SOURCES bmp_image.h bmp_image.cpp cpphelpers.h parsed_images.h parsed_images.cpp parse_test.cpp parsed_labels.h parsed_labels.cpp calculus.h network.h network.cpp)

add_executable(parse_test ${SOURCES})
