#include "frame.h"
#include <iostream>
Frame::Frame(LPCTSTR pImgFile, int w, int h, double zoomRate) { set(pImgFile, w, h, zoomRate); }
void Frame::set(LPCTSTR pImgFile, int w, int h, double zoomRate)
{
	IMAGE temp;
	loadimage(&temp, pImgFile, w, h, true);
	zoomImage(&img, &temp, zoomRate);
	flipframe();	  // 获得翻转后的帧
	sketchframe();	  // 获得闪白正帧
	sketchframeOFD(); // 获得闪白反帧
}
void Frame::putframe(double px, double py, bool flip, bool canReduceHp)
{
	short flag = 1;
	if (!canReduceHp)
	{
		timer += DELTA;
		if (timer >= 0.5 * DELTA_TIME)
		{
			timer = 0;
			flag = !flag;
		}
	}
	else
		flag = 1;

	flag %= 2;
	if (flip)
		putAlpha((int)px, (int)py, (flag ? &flippedImg : &sketchedimgOFD));
	else
		putAlpha((int)px, (int)py, (flag ? &img : &sketchedimg));
}
void Frame::flipframe() { flipImage(&flippedImg, &img); }
void Frame::sketchframe() { sketchImage(&sketchedimg, &img); }
void Frame::sketchframeOFD() { sketchImage(&sketchedimgOFD, &flippedImg); }
int Frame::getW() const { return img.getwidth(); }
int Frame::getH() const { return img.getheight(); }