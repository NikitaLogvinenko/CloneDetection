#pragma once
#include "image.h"

class ImageResampler final
{
public:
	ImageResampler() = delete;

	static void resampleImage(const Image& src, Image& output, size_t radius, const InterpolationCurve& curve);
};