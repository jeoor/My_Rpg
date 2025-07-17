#ifndef FRAME_H
#define FRAME_H

#include <easyx.h>
#include "edit.h"
#include "alpha.h"
class Frame
{
public:
	Frame() = default;
	Frame(LPCTSTR pImgFile, int w, int h, double ZoomRate);
	void set(LPCTSTR pImgFile, int w, int h, double ZoomRate);
	void putframe(int px, int py, bool& flip);
	void flipframe();
	int getW() const;
	int getH() const;

private:
	IMAGE img;
	IMAGE flippedImg;			// ·­×ªºóµÄÍ¼Ïñ
};

#endif