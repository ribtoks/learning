#include "mnist_dataset.h"
#include "parsing/parsed_images.h"
#include "parsing/parsed_labels.h"
#include "common/log.h"
#include "common/cpphelpers.h"
#include "parsing/bmp_image.h"

mnist_dataset_t::mnist_dataset_t(const std::string &data_root):
    data_root_(data_root)
{    
}

std::vector<std::tuple<vector_t<double>, vector_t<double>>> mnist_dataset_t::training_data(int limit) {
    parsed_images_t parsed_images(data_root_ + "train-images-idx3-ubyte");
    auto itImg = parsed_images.begin();
    auto itImgEnd = parsed_images.end();
    
    parsed_labels_t parsed_labels(data_root_ + "train-labels-idx1-ubyte");
    auto itLbl = parsed_labels.begin();
    auto itLblEnd = parsed_labels.end();

    std::vector<std::tuple<vector_t<double>, vector_t<double>>> data;
    const size_t count_limit = limit == -1 ? parsed_images.size() : limit;
    data.reserve(count_limit);

    for (;
         itImg != itImgEnd && itLbl != itLblEnd;
         ++itImg, ++itLbl) {
        vector_t<double> input(*itImg); input.mul(1.0 / 255.0);
        vector_t<double> result(10, 0.0); result[*itLbl] = 1.0;

        data.emplace_back(input, result);

        if (data.size() >= count_limit) { break; }
    }

    log("Training data loaded: %d images", data.size());

    return data;
}

void mnist_dataset_t::save_as_images(int limit) {
    parsed_images_t parsed_images(data_root_ + "train-images-idx3-ubyte");
    auto itImg = parsed_images.begin();
    auto itImgEnd = parsed_images.end();

    parsed_labels_t parsed_labels(data_root_ + "train-labels-idx1-ubyte");
    auto itLbl = parsed_labels.begin();
    auto itLblEnd = parsed_labels.end();

    const size_t count_limit = limit == -1 ? parsed_images.size() : limit;
    size_t i = 0;

    for (;
         itImg != itImgEnd && itLbl != itLblEnd;
         ++itImg, ++itLbl) {

        bmp_image_t(*itImg, 28)
            .save(
                string_format("test_%d_digit_%d.bmp", i++, *itLbl));

        if (i > count_limit) { break; }
    }
}
