#include "frame.h"
#include <iostream>

Frame::Frame(LPCTSTR pImgFile, int w, int h, double ZoomRate)
{
	set(pImgFile, w, h, ZoomRate);
}
void Frame::set(LPCTSTR pImgFile, int w, int h, double ZoomRate)
{
	IMAGE temp;
	loadimage(&temp, pImgFile, w, h, true);
	ZoomImage(&img, &temp, ZoomRate);
	flipframe();		// 获得翻转后的帧
	sketchframe();		// 获得闪白正帧
	sketchframeOFD();	// 获得闪白反帧
}
void Frame::putframe(double px, double py, bool &flip, bool &canReduceHP)
{
	short flag = 1;
	if (!canReduceHP)
	{
		timer += DELTA;
		if(timer >= 0.5 * DELTA_TIME)
		{
			timer = 0;
			flag++;
		}
	}
	else
		flag = 1;

	flag %= 2;
	if (flip)
		putalpha((int)px, (int)py, (flag ? & flippedImg : &sketchedimgOFD));
	else
		putalpha((int)px, (int)py, (flag ? & img : &sketchedimg));
}
void Frame::flipframe()
{
	FlipImage(&flippedImg, &img);
}
void Frame::sketchframe()
{
	SketchImage(&sketchedimg, &img);
}
void Frame::sketchframeOFD()
{
	SketchImage(&sketchedimgOFD, &flippedImg);
}

int Frame::getW() const { return img.getwidth(); }
int Frame::getH() const { return img.getheight(); }