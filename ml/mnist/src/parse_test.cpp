#include "parsed_images.h"
#include "bmp_image.h"
#include "cpphelpers.h"
#include <exception>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Missing path to images file");
    }

    std::string filepath(argv[1]);
    parsed_images_t parsed_images(filepath);

    auto it = parsed_images.begin();
    auto itEnd = parsed_images.end();
    int index = 0;
    for (; it != itEnd; ++it, index++) {
        bmp_image_t(*it)
            .save(
                string_format("test_image_%d.bmp", index));

        // debug
        if (index > 20) { break; }
    }
    
    return 0;
}
















