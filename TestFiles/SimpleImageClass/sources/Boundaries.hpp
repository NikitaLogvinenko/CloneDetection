#pragma once

#include "Range.hpp"

struct Boundaries final
{
    Range rows_range_{}, cols_range_{}, channels_range_{};

    Boundaries() = default;
    Boundaries(const Range& rows_range, const Range& cols_range, const Range& channels_range);

    [[nodiscard]] int rows() const;
    [[nodiscard]] int cols() const;
    [[nodiscard]] int total() const;
    [[nodiscard]] int channels() const;
    [[nodiscard]] bool empty() const;
};
