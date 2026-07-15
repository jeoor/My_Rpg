#include "edit.h"
void zoomImage(IMAGE *targetImg, IMAGE *originImg, double zoomRate)
{
	targetImg->Resize((int)(originImg->getwidth() * zoomRate), (int)(originImg->getheight() * zoomRate));
	DWORD *M = GetImageBuffer(targetImg);
	DWORD *N = GetImageBuffer(originImg);

	for (int i = 0; i < targetImg->getheight(); i++)
		for (int j = 0; j < targetImg->getwidth(); j++)
			M[j + i * targetImg->getwidth()] =
				N[(int)(j / zoomRate) + (int)(i / zoomRate) * originImg->getwidth()];
}
void flipImage(IMAGE *targetImg, IMAGE *originImg)
{
	targetImg->Resize((int)(originImg->getwidth()), (int)(originImg->getheight()));
	DWORD *M = GetImageBuffer(targetImg);
	DWORD *N = GetImageBuffer(originImg);

	for (int i = 0; i < targetImg->getwidth(); i++)
		for (int j = 0; j < targetImg->getheight(); j++)
			M[j * targetImg->getwidth() + i] = N[j * targetImg->getwidth() + targetImg->getwidth() - i - 1];
}
void sketchImage(IMAGE *targetImg, IMAGE *originImg)
{
	targetImg->Resize((int)(originImg->getwidth()), (int)(originImg->getheight()));
	DWORD *M = GetImageBuffer(targetImg);
	DWORD *N = GetImageBuffer(originImg);

	for (int i = 0; i < targetImg->getheight(); i++)
		for (int j = 0; j < targetImg->getwidth(); j++)
			if (N[i * targetImg->getwidth() + j] & 0xFF000000)
				M[i * targetImg->getwidth() + j] = BGR(RGB(255, 255, 255)) | (((DWORD)(BYTE)(255)) << 24);
}