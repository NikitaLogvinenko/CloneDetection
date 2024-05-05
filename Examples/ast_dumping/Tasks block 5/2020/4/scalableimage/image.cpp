#include "image.h"
#include <fstream>

#include "imageresampler.h"


Image::Image(size_t w, size_t h): Bitmap()
{
	if (w > 0 && h > 0)
	{
		m_width = w;
		m_height = h;
		m_buffer = new uint8_t[w * h * 3 + padding() * h]();
	}
}

bool Image::load(const std::string& path)
{
	std::ifstream file(path);
	const bool result = Bitmap::load(file);
	file.close();
	return result;
}

bool Image::save(const std::string& path) const
{
	std::ofstream file(path);
	const bool result = Bitmap::save(file);
	file.close();
	return result;
}


void Image::resize(Image& output, size_t radius, const InterpolationCurve& weightCurve) const
{
	ImageResampler::resampleImage(*this, output, radius, weightCurve);
}
