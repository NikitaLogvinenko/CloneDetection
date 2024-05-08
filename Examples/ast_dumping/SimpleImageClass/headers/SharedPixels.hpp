#pragma once

class SharedPixels final
{
    unsigned char* data_;
    size_t* refs_counter_;
    size_t data_size_;
    bool clear_data_if_no_refs_;

public:
    SharedPixels();
    SharedPixels(unsigned char* data, size_t data_size, bool clear_if_no_refs);

    SharedPixels(const SharedPixels& copied_data);
    SharedPixels& operator=(const SharedPixels& assigned_data);

    ~SharedPixels();

    [[nodiscard]] unsigned char* data();
    [[nodiscard]] const unsigned char* data() const;

    [[nodiscard]] size_t data_size() const;
    [[nodiscard]] bool clears_data_itself() const;

    [[nodiscard]] size_t count_ref() const;

    unsigned char& operator[](int i);
    [[nodiscard]] const unsigned char& operator[](int i) const;
    unsigned char& at(int i);
    [[nodiscard]] const unsigned char& at(int i) const;

    unsigned char* operator+(int i);
    const unsigned char* operator+(int i) const;

private:
    void clear() const;
};
