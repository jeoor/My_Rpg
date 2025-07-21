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
void Frame::putframe(double px, double py, bool &flip)
{
	if (flip)
		putalpha((int)px, (int)py, &flippedImg);
	else
		putalpha((int)px, (int)py, &img);
}
void Frame::flipframe()
{
	FlipImage(&flippedImg, &img);
}

int Frame::getW() const { return img.getwidth(); }
int Frame::getH() const { return img.getheight(); }