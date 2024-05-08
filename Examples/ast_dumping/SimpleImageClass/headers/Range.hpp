#pragma once

class Range final
{
    int start_{}, end_{};
public:
    Range() = default;
    Range(int start, int end);
    [[nodiscard]] int size() const;
    [[nodiscard]] bool empty() const;

    [[nodiscard]] int start() const;
    [[nodiscard]] int end() const;

    static Range all();
};