#ifndef __ALPHA_H__
#define __ALPHA_H__

#include <easyx.h>
#pragma comment(lib, "MSIMG32.LIB")
void putAlpha(int x, int y, IMAGE *img);
void putAlpha(int x, int y, IMAGE *img, int w, int h);
#endif
