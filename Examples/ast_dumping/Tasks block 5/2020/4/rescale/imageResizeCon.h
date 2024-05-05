#pragma once
#include <string>
#include <fstream>

#include "image.h"


class ImageResizeCon final
{
Image out;

static bool fileExists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}
	
void saveImage() const;
public:
	std::string outFileName;
	std::string inFileName;
	std::string method;
	
	float horizontalScale = 1;
	float verticalScale = 1;

	ImageResizeCon(const std::string& inName, const std::string& outName, float h, float v, const std::string& curve);

	bool resize();
	void write() const;
	
};
