﻿#pragma once
#include <bitset>

namespace utility
{
    template <class T>
    [[nodiscard]] size_t hash_combine(size_t seed, const T& v)
    {
        std::hash<T> hasher{};
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
}
