#include "imageResizeCon.h"
#include <iostream>

#include "biqubicCurve.h"
#include "lanczosCurve.h"
#include "linearCurve.h"

void ImageResizeCon::saveImage() const
{
	if (!out.save(outFileName.c_str()))
	{
		std::cout << "For some reason image was not saved\n";
	}
	else
	{
		std::cout << "File <" << outFileName << "> was saved\n";
	}
}

ImageResizeCon::ImageResizeCon(const std::string& inName, const std::string& outName, float h, float v,
                               const std::string& curve) :
	outFileName(outName), inFileName(inName), method(curve), horizontalScale(h), verticalScale(v)
{
}

bool ImageResizeCon::resize()
{
	Image image;
	if (!image.load(inFileName.c_str()))
	{
		std::cout << "Could not load image\n";
		return false;
	}

	if (horizontalScale <= 0 || verticalScale <= 0)
	{
		std::cout << "Invalid scale values\n";
		return false;
	}
	out.m_width = image.m_width * static_cast<size_t>(horizontalScale);
	out.m_height = image.m_height * static_cast<size_t>(verticalScale);
	out.m_buffer = new uint8_t[out.m_width * out.m_height * 3 + out.padding() * out.m_height]();

	if (method == "lin")
		image.resize(out, 2, LinearCurve());
	else if (method == "biq")
		image.resize(out, 2, BiqubicCurve());
	else if (method == "lan")
		image.resize(out, 2, LanczosCurve());
	else
	{
		std::cout << "Resize method not specified\n";
		return false;
	}
	return true;
}

void ImageResizeCon::write() const
{
	if (fileExists(outFileName))
	{
		std::cout << "File already <" << outFileName << "> exists. Do you want to overwrite it?\nAnswer (y\\n): ";
		char answer;
		std::cin >> answer;
		if (answer == 'y')
		{
			saveImage();
		}
	}
	else
		saveImage();
}
