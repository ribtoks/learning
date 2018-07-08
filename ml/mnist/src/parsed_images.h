#ifndef IMAGES_PARSER_H
#define IMAGES_PARSER_H

#include <string>
#include <fstream>
#include <vector>

class parsed_images_t {
public:
    using data1d = std::vector<uint8_t>;
    using data2d = std::vector<data1d>;
    
public:
    class iterator {
    public:
        iterator(std::ifstream &stream, size_t columns, size_t rows, size_t index=0);

    private:
        void read_next();
        
    public:
        const data2d& operator*() { read_next(); return data_; }
        iterator& operator++() { return *this; }
        bool operator==(const iterator &other);
        bool operator!=(const iterator &other);
        
    private:
        std::ifstream &stream_;
        size_t columns_;
        size_t rows_;
        size_t index_;
        data2d data_;
    };
    
public:
    parsed_images_t(const std::string &filepath);

public:
    size_t size() const { return images_count_; }

public:
    iterator begin();
    iterator end();

private:
    void read_header();

private:
    std::ifstream stream_;
    size_t images_count_ = 0;
    size_t rows_ = 0;
    size_t columns_ = 0;
};

#endif // IMAGES_PARSER_H










