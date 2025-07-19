#include "alpha.h"

void putalpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(nullptr), x, y, w, h, GetImageHDC(img), 0, 0, w, h, 
		{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}