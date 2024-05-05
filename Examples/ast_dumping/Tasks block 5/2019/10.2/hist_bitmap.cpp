#include "stdafx.h"
#include "hist_bitmap.h"
#include "Histogram.hpp"
#include <cmath>




hist_Bitmap & hist_Bitmap::operator=(const hist_Bitmap & right)
{
	if (&right == this)
		return *this;

	m_height = right.m_height;
	m_width = right.m_width;
	size_t stride = right.get_stride();
	m_buffer = new uint8_t[m_height*stride];
	memcpy(m_buffer, right.m_buffer, right.m_height * stride);
	return *this;
}

hist_Bitmap::hist_Bitmap(size_t cx, size_t cy)
{
	m_width = cx;
	m_height = cy;
	size_t stride = get_stride();
	m_buffer = new uint8_t[stride*m_height];
}

bool hist_Bitmap::draw_rect(Point paste_point, int bin_height, size_t scale)
{
	for (size_t i = 1; i <= scale; i++)
	{
		draw_v_line(paste_point, bin_height, scale);
		paste_point.x ++;
	}
	return true;
}

bool hist_Bitmap::draw_rect(Rectangle rect)
{
	if (valid_rect(rect))
	{
		size_t rect_width = rect.right_down_corner.x - rect.left_upper_corner.x;
		size_t init_coord = rect.left_upper_corner.x;
		for (size_t i = 1; i <= rect_width; ++i)
		{
			bool res = draw_v(init_coord, rect.right_down_corner.y, rect.left_upper_corner.y);
			//this->save("out.bmp"); //после второй итерации он уже не может сохранить файл
			if (res == false)
				return false;
			init_coord++;
		}
		return true;
	}
	return false;
}

bool hist_Bitmap::if_valid_v_line (Point paste_point, int line_height, size_t scale)
{
	bool res2;
	bool res1= (paste_point.x >= 0 && paste_point.y >= 0 && paste_point.x < m_width && paste_point.y < m_height);
	
	if (line_height >= 0)
	{
		res2 = ((paste_point.y - line_height * scale) >= 0);
	}
	else
	{
		res2 = ((paste_point.y + line_height * scale) <= m_height);
	}
	return res1 && res2;
}

bool hist_Bitmap::if_valid_h_line(Point paste_point, int line_length, size_t scale)
{
	bool res2;
	bool res1 = (paste_point.x >= 0 && paste_point.y >= 0 && paste_point.x < m_width && paste_point.y < m_height);

	if (line_length >= 0)
	{
		res2 = ((paste_point.x + line_length * scale) <= m_width);
	}
	else
	{
		res2 = ((paste_point.x + line_length * scale) >= 0);
	}
	return res1 && res2;
}

bool hist_Bitmap::if_valid_v(int x1, int y1, int y2)
{
	return (x1 >= 0 && y1 >= 0 && y2>=0 && x1<= m_width && y2<=m_height && y1<=m_height);
}

bool hist_Bitmap::if_valid_h(int x1, int x2, int y)
{
	return (x1>=0 && x1>=0 && y>=0 && x1<= m_width && x2<=m_width && y<m_height);
}

bool hist_Bitmap::draw_v_line(Point paste_point, int line_height, size_t scale)
{
	if (if_valid_v_line(paste_point, line_height, scale))
	{
		size_t stride = get_stride();
		uint8_t* dst = m_buffer + stride * paste_point.y + paste_point.x*PIX_SIZE;
		int sign = line_height >= 0 ?  -1 : 1;
		size_t abs_line_height = abs(line_height);
		for (size_t y = 0; y < abs_line_height * scale; ++y)
		{
			*dst = 0;
			*(dst + 1) = 0;
			*(dst + 2) = 0;
			dst += stride * sign; /*если столбец отрицательной высоты, будет спускаться
								  по страйду, если положительной - подниматься*/
		}
		return true;
	}
	return false;
}

bool hist_Bitmap::draw_h_line(Point paste_point, int line_length, size_t scale)
{
	if (if_valid_h_line(paste_point, line_length, scale))
	{
		size_t stride = get_stride();
		uint8_t* dst = m_buffer + stride * paste_point.y + paste_point.x*PIX_SIZE;
		int sign = line_length >= 0 ? -1 : 1;
		size_t abs_line_length = abs(line_length);
		for (size_t y = 0; y < abs_line_length * scale; ++y)
		{
			*dst = 0;
			*(dst + 1) = 0;
			*(dst + 2) = 0;
			dst += 3 * sign; /*если столбец отрицательной высоты, будет спускаться
								  по страйду, если положительной - подниматься*/
		}
		return true;
	}
	return false;
}

bool hist_Bitmap::draw_v(int x, int y1, int y2)
{
	if (if_valid_v(x, y1,y2))
	{
		size_t stride = get_stride();
		if (y1 > y2)
		{
			int temp = y1;
			y1 = y2;
			y2 = temp;
		}

		uint8_t* dst = m_buffer + stride * y1 + x*PIX_SIZE;
		for (size_t y = y1; y < y2; ++y)
		{
			*dst = 0;
			*(dst + 1) = 0;
			*(dst + 2) = 0;
			dst += stride; 
		}
		return true;
	}
	return false;
}


bool hist_Bitmap::draw_h(int x1, int x2, int y)
{
	if (if_valid_h(x1, x1, y))
	{
		size_t stride = get_stride();
		if (x1 > x2)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
		}
		uint8_t* dst = m_buffer + stride * y + x1*PIX_SIZE;
		for (int x = x1; x < x2; ++x)
		{
			*dst = 0;
			*(dst + 1) = 0;
			*(dst + 2) = 0;
			dst += 3; 
		}
		return true;
	}
	return false;
}


