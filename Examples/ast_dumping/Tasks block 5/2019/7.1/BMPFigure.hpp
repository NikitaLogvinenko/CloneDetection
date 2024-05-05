#pragma once
#include <vector>
#include "libbitmap.h"

typedef struct rgb
{
	int r = 0;
	int g = 0;
	int b = 0;
};

typedef struct point
{
	int x = 0;
	int y = 0;
};

class BMPFigure {
private:
	Bitmap bmp;
	bool valid = false;
	size_t stride = 0;
public:
	BMPFigure() = delete;
	BMPFigure(std::string in);
	BMPFigure(int width, int height);
	~BMPFigure();

	bool isValid();

	size_t width();
	size_t height();
	void background_color(rgb color = { 255, 255, 255 }); //unsigned char | uint8_t struct rgb {}

	rgb	 get_pixel(point r); //struct rgb {}
	void set_pixel(point r, rgb color = { 0, 0, 0 });
	void draw_line(point r0, point r1, rgb color = { 0, 0, 0 });
	void draw_rect(point r0, point r1, rgb color = { 0, 0, 0 }, rgb fill = { 255, 255, 255 });
	void draw_triangle(point r0, point r1, point r2, rgb color = { 0, 0, 0 });
	void save(std::string out = "Untitled.bmp"); // save(const char* out)

	bool out_of_range(point r);
};