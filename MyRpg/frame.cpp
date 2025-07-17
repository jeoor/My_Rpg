#include "frame.h"

Frame::Frame(LPCTSTR pImgFile, int w, int h, double ZoomRate)
{
	set(pImgFile, w, h, ZoomRate);
}
void Frame::set(LPCTSTR pImgFile, int w, int h, double ZoomRate)
{
	IMAGE temp;
	loadimage(&temp, pImgFile, w, h, true);
	ZoomImage(&img, &temp, ZoomRate);
	FlipImage(&flippedImg, &img);
}
void Frame::putframe(int px, int py, bool& flip)
{
	if (flip)
		putalpha(nullptr, px, py, &flippedImg);
	else
		putalpha(nullptr, px, py, &img);
}
void Frame::flipframe()
{
	FlipImage(&flippedImg, &img);
}

int Frame::getW() const { return img.getwidth(); }
int Frame::getH() const { return img.getheight(); }