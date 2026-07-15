#ifndef __EDIT_H__
#define __EDIT_H__

#include <easyx.h>
void zoomImage(IMAGE *targetImg, IMAGE *originImg, double zoomRate);
void flipImage(IMAGE *targetImg, IMAGE *originImg);
void sketchImage(IMAGE *targetImg, IMAGE *originImg);
#endif
