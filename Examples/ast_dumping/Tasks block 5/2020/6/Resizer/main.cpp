#include "resizer.h"
#include "interpoltype.h"
#include "bicubic.h"
#include "bilinear.h"
#include  "lanczos.h"
int main()
{
	Image input;
	if (!input.load("cat.bmp")) { return 0; }
	const size_t w = 400, h = 400;
	Image outputBC(w, h);
	Image outputBL(w, h);
	input.resize(outputBL, 2, Bilinear());
	if (!outputBL.save("BLin.bmp")) { return 0; }
	input.resize(outputBC, 2,Bicubic());
	if (!outputBC.save("BCub.bmp")) { return 0; }
	Image outputLN2(w, h);
	input.resize(outputLN2,2,Lanczos(2));
	if (!outputLN2.save("BLN2.bmp")) { return 0; }
	Image outputLN3(w, h);
	input.resize(outputLN3, 3,Lanczos(3));
	if (!outputLN3.save("BLN3.bmp")) { return 0; }
	return 1;
}