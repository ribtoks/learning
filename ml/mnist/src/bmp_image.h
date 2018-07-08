#ifndef BMP_HELPER_H
#define BMP_HELPER_H

#include <cstdint>
#include <vector>
#include <string>

class bmp_image_t {
public:
    bmp_image_t(const std::vector<std::vector<uint8_t>> &data):
        data_(data)
    { }

public:
    void save(const std::string &filepath);

private:
    const std::vector<std::vector<uint8_t>> &data_;
};


#endif // BMP_HELPER_H
