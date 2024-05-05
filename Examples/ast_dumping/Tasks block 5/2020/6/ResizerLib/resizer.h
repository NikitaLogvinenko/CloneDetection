#pragma once
#include "libbitmap.h"
class InterpolationType;

class Image : public Bitmap
{
public:
	Image(void):Bitmap(){};               // конструктор пустого изображения
	Image(size_t w, size_t h)
	{
		this->m_height = h;
		this->m_width = w;
		size_t size = this->m_width * this->m_height * 3 + this->padding() * this->m_height;
		this->m_buffer = new uint8_t[size];

		for (size_t i = 0; i < this->m_height; i++)
		{
			for (size_t j = 0; j < this->m_width; j++)
			{
				this->m_buffer[this->get_pixel_id(j, i, 0)] = 0;
				this->m_buffer[this->get_pixel_id(j, i, 1)] = 0;
				this->m_buffer[this->get_pixel_id(j, i, 2)] = 0;
			}
		}
	}; // конструктор изображения размером w*h (черный фон)

	void resize(Image& output, int radius, const InterpolationType& type) const;
private:
	uint8_t Kernel(double f_x, double f_y, int radius, Image& output, int channeloffset, const InterpolationType& type) const;
};