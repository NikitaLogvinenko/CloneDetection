
#include <cmath>
#include "resizer.h"
#include "interpoltype.h"
#include <algorithm>
#include <cstdint>

uint8_t Image::Kernel(double f_x, double f_y, int radius, Image& output, int channeloffset, const InterpolationType& type) const {
	double sum = 0;
	double normalizer = 0;
	for (int i = -radius + 1; i <= 2; ++i)
		for (int j = -radius + 1; j <= 2; ++j) {
			const int ix = std::clamp(static_cast<int>(std::round(f_x)) + j,0,static_cast<int>(m_width-1));
			const int iy = std::clamp(static_cast<int>(std::round(f_y)) + i,0,static_cast<int>(m_height-1));
			const double f_xdiff = f_x - ix;
			const double f_ydiff = f_y - iy;
			const double weight = type.getweight(f_xdiff) * type.getweight(f_ydiff);
			sum += m_buffer[get_pixel_id(ix, iy, channeloffset)]*weight;
			normalizer += weight;
		}
	return static_cast<uint8_t>(std::clamp(static_cast<int>(sum/normalizer), 0, 255));
}
void Image::resize(Image& output,int radius,const InterpolationType& type) const
{
	const double ratiox = m_width / static_cast<double>(output.m_width);
	const double ratioy = m_height / static_cast<double>(output.m_height);
	for (size_t y = 0; y < output.m_height; ++y)
	{
		for (size_t x = 0; x < output.m_width; ++x)
		{
			const double oldx = x * ratiox;
			const double oldy = y * ratioy;
			for(size_t channeloffset = 0;channeloffset<3;++channeloffset)
			{
				output.m_buffer[output.get_pixel_id(x, y, channeloffset)] = Kernel(oldx, oldy, radius, output, channeloffset, type);
			}
		}
	}
}