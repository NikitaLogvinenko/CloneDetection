#pragma once
#include "libbitmap.h"
#include "curves.h"

class Image : public Bitmap
{
public:

    Image(void):Bitmap(){}
    Image(size_t w, size_t h);
    bool load(const std::string& path);
    bool save(const std::string& path) const;
    
    void resize(
        Image& output,   
        size_t radius,         
        const InterpolationCurve& weightCurve
    ) const;

    bool empty() const
    {
	    return !(m_width > 0 && m_height > 0 && m_buffer != nullptr);
    }
    virtual ~Image() = default;
};
