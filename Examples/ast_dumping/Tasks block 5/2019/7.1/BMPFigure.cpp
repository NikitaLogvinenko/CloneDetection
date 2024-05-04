#include <iostream>
#include "BMPFigure.hpp"

using namespace std;

void swap(int* a, int* b)
{
	*a = *a - *b;
	*b = *a + *b;
	*a = *b - *a;
}

bool BMPFigure::out_of_range(point r)
{
	if (r.x > bmp.m_width || r.y > bmp.m_height || r.x < 0 || r.y < 0)
		return true;
	return false;
}

size_t alignTo32(int w)
{
	return (w + 3) & ~3;
}

bool BMPFigure::isValid()
{
	return valid;
}

size_t BMPFigure::width()
{
	return bmp.m_width;
}

size_t BMPFigure::height()
{
	return bmp.m_height;
}

BMPFigure::BMPFigure(string in)
{
	valid = bmp.load(in);
	stride = alignTo32(bmp.m_width * 3);
}

BMPFigure::BMPFigure(int width, int height)
{
	bmp.m_width = width;
	bmp.m_height = height;
	stride = alignTo32(width * 3);

	bmp.m_buffer = new uint8_t[height*stride];
	valid = true;
}

BMPFigure::~BMPFigure()
{
	valid = false;
}

rgb	BMPFigure::get_pixel(point r)
{
	if (!valid)
		throw "picture is not valid.";
	if (out_of_range(r))
		return { -1, -1, -1 };
	int cell = r.y * stride + r.x * 3;
	rgb pixel = { bmp.m_buffer[cell],
					bmp.m_buffer[cell + 1],
					bmp.m_buffer[cell + 2] };
	return pixel;
}

void BMPFigure::set_pixel(point r, rgb color)
{
	if (out_of_range(r))
		return;

	int cell = r.y * stride + r.x * 3;
	bmp.m_buffer[cell] = color.r;
	bmp.m_buffer[cell + 1] = color.g;
	bmp.m_buffer[cell + 2] = color.b;
}

void BMPFigure::draw_line(point r0, point r1, rgb color)
{
	bool flag = abs(r1.y - r0.y) > abs(r1.x - r0.x) ? 1 : 0;
	if (flag)
	{
		swap(&r0.x, &r0.y);
		swap(&r1.x, &r1.y);
	}
	if (r1.x - r0.x < 0)
	{
		swap(&r0.x, &r1.x);
		swap(&r0.y, &r1.y);
	}

	int deltax = abs(r1.x - r0.x);
	int deltay = abs(r1.y - r0.y);

	int error = 0;
	int deltaerr = deltay;
	int y = r0.y;
	int diry = r1.y - r0.y;

	if (diry > 0)
		diry = 1;
	if (diry < 0)
		diry = -1;

	for (int x = r0.x; x <= r1.x; x++)
	{
		int cell = flag ? (x * stride + y * 3) : (y * stride + x * 3);

		error += deltaerr;
		if (2 * error >= deltax)
		{
			y += diry;
			error -= deltax;
		}

		if ((!flag && ((x >= bmp.m_width) || (x < 0) || (y >= bmp.m_height) || (y < 0))
			|| (y >= bmp.m_width) || (y < 0) || (x >= bmp.m_height) || (x < 0)))
			continue;
		bmp.m_buffer[cell + 0] = color.r;
		bmp.m_buffer[cell + 1] = color.g;
		bmp.m_buffer[cell + 2] = color.b;
	}
}

void BMPFigure::draw_rect(point r0, point r1, rgb color, rgb fill)
{
	draw_line(r0, point{ r1.x, r0.y }, color);
	draw_line(point{ r1.x, r0.y }, r1, color);
	draw_line(r1, point{ r0.x, r1.y }, color);
	draw_line(point{ r0.x, r1.y }, r0, color);

	int minX = r0.x > r1.x ? r1.x : r0.x;
	int maxX = r0.x > r1.x ? r0.x : r1.x;
	int minY = r0.y > r1.y ? r1.y : r0.y;
	int maxY = r0.y > r1.y ? r0.y : r1.y;

	for (int i = minX+1; i < maxX; i++)
	{
		for (int j = minY+1; j < maxY; j++)
		{
			set_pixel({ i, j }, fill);
		}
	}
}

void BMPFigure::draw_triangle(point r0, point r1, point r2, rgb color)
{
	draw_line(r0, r1, color);
	draw_line(r1, r2, color);
	draw_line(r2, r0, color);
}

void BMPFigure::background_color(rgb color)
{
	for (int i = 0; i < bmp.m_width; i++)
		for (int j = 0; j < bmp.m_height; j++)
			set_pixel(point{ i, j }, color);
}

void BMPFigure::save(std::string out)
{
	if (!valid)
		return;
	bmp.save(out);
}