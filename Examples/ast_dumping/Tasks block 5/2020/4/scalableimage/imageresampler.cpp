#include "imageresampler.h"
#include <cmath>
#include <algorithm>

namespace
{
	int nearestCell(float id)
	{
		return int(std::round(id));
	}

	float pixelWeight(const InterpolationCurve& f, float x, float y)
	{
		return f.getWeight(x) * f.getWeight(y);
	}

	int interpolatePixel(const Image& src, size_t radius, const InterpolationCurve& f, float sampleX, float sampleY,
	                     int channel)
	{
		int sum = 0;
		for (int i = -int(radius); i <= int(radius); ++i)
		{
			for (int j = -int(radius); j <= int(radius); ++j)
			{
				const int x = nearestCell(sampleX) + i;
				const int y = nearestCell(sampleY) + j;

				const int pixel = src.getPixelChannel(x, y, channel);
				
				sum += static_cast<int>(pixel *pixelWeight(f, sampleX - static_cast<float>(x), sampleY - static_cast<float>(y)));
			}
		}

		return std::clamp(sum, 0, 255);
	}
}

void ImageResampler::resampleImage(const Image& src, Image& output, size_t radius, const InterpolationCurve& curve)
{
	if (src.empty() || output.empty())
		return;
	const float widthRatio = float(src.m_width) / output.m_width;
	const float heightRatio = float(src.m_height) / output.m_height;

	for (size_t i = 0; i < output.m_width; ++i)
	{
		for (size_t j = 0; j < output.m_height; ++j)
		{
			const float sampleX = static_cast<float>(i) * widthRatio;
			const float sampleY = static_cast<float>(j)* heightRatio;

			const int id = output.getPixelId(i, j, 0);

			output.m_buffer[id] = interpolatePixel(src, radius, curve, sampleX, sampleY, 0);
			output.m_buffer[id + 1] = interpolatePixel(src, radius, curve, sampleX, sampleY, 1);
			output.m_buffer[id + 2] = interpolatePixel(src, radius, curve, sampleX, sampleY, 2);
		}
	}
}
