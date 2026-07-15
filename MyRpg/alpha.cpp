#include "alpha.h"
void putAlpha(int x, int y, IMAGE *img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(nullptr), x, y, w, h, GetImageHDC(img), 0, 0, w, h,
			   {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}
void putAlpha(int x, int y, IMAGE *img, int w, int h)
{
	AlphaBlend(GetImageHDC(nullptr), x, y, w, h, GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(),
			   {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}
