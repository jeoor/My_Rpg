#ifndef __FRAME_H__
#define __FRAME_H__

#include <easyx.h>
#include "edit.h"
#include "alpha.h"
class Frame
{
public:
	Frame() {}
	Frame(LPCTSTR pImgFile, int w, int h, double ZoomRate);
	void set(LPCTSTR pImgFile, int w, int h, double ZoomRate);
	void putframe(double px, double py, bool &flip);
	void flipframe();
	int getW() const;
	int getH() const;

private:
	IMAGE img;
	IMAGE flippedImg; // ·­×ªºóµÄÍ¼Ïñ
};

#endif