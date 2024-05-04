#ifndef ___HIST_BITMAP_H___
#define ___HIST_BITMAP_H___

#include "shapes.h"
#include "crop_bitmap.h"
class Histogram;

class hist_Bitmap : public crop_Bitmap
{
public:
	hist_Bitmap& operator=(const hist_Bitmap &right);
	hist_Bitmap (size_t cx, size_t cy);
	bool draw_rect(Point paste_point, int bin_height, size_t scale);
	bool draw_v_line(Point paste_point, int line_height, size_t scale);
	bool draw_h_line(Point paste_point, int line_length, size_t scale);
	bool draw_v(int x, int y1, int y2);
	bool draw_h(int x1, int x2, int y);
	bool draw_rect(Rectangle rect);

public:
	bool if_valid_v_line(Point paste_point, int line_height, size_t scale);
	bool if_valid_h_line(Point paste_point, int line_length, size_t scale);

	bool if_valid_v(int x1, int y1, int y2);
	bool if_valid_h(int x1, int x2, int y);

	
};

const size_t HIST_HEIGHT_SCALE = 200;
const size_t HIST_WIDTH_SCALE = 50;


#endif //___HIST_BITMAP_H___


