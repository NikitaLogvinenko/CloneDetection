#pragma once

#include "Range.hpp"
#include "Boundaries.hpp"
#include "SharedPixels.hpp"


class Image final
{    
    Boundaries current_boundaries_{};
    Boundaries initial_boundaries_{};
    SharedPixels pixels_{};

public:
    Image() = default;
    Image(int rows, int cols, int channels);
    Image(int rows, int cols, int channels, unsigned char* data);
    Image(const Image& image) = default;
    Image(const Image& image, const Range& rows_range, const Range& cols_range);
    ~Image() = default;

    Image& operator=(const Image& image);
    [[nodiscard]] Image operator()(const Range& rows_range, const Range& cols_range) const;

    [[nodiscard]] Image clone() const;
    void copyTo(Image& image) const;
    void create(int rows, int cols, int channels);
    [[nodiscard]] bool empty() const;

    void release();

    [[nodiscard]] Image col(int x) const;
    [[nodiscard]] Image colRange(const Range& range) const;

    [[nodiscard]] Image row(int y) const;
    [[nodiscard]] Image rowRange(const Range& range) const;

    [[nodiscard]] const unsigned char* data() const;
    [[nodiscard]] unsigned char* data();

    [[nodiscard]] int rows() const;
    [[nodiscard]] int cols() const;
    [[nodiscard]] int total() const;
    [[nodiscard]] int channels() const;

    unsigned char& at(int index);
    [[nodiscard]] const unsigned char& at(int index) const;

    [[nodiscard]] static Image zeros(int rows, int cols, int channels);
    [[nodiscard]] static Image values(int rows, int cols, int channels, unsigned char value);

    [[nodiscard]] size_t countRef() const;

private:
    [[nodiscard]] int rel_index_to_offset(int relative_index) const;
    void copy_to_prepared(Image& image) const;
    
	static bool invalid_boundaries(int rows, int cols, int channels);
};
