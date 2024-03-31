#pragma once
#include <bitset>

namespace utility
{
    [[nodiscard]] constexpr size_t hash_combine(const size_t first, const size_t second) noexcept
    {
        return first ^ (second + 0x9e3779b9 + (first << 6) + (first >> 2));
    }
}
