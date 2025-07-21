#include "edit.h"

void ZoomImage(IMAGE *TarImg, IMAGE *OriImg, double ZoomRate)
{
	TarImg->Resize((int)(OriImg->getwidth() * ZoomRate), (int)(OriImg->getheight() * ZoomRate));
	DWORD *M = GetImageBuffer(TarImg);
	DWORD *N = GetImageBuffer(OriImg);

	for (int i = 0; i < TarImg->getheight(); i++)
		for (int j = 0; j < TarImg->getwidth(); j++)
			M[j + i * TarImg->getwidth()] = N[(int)(j / ZoomRate) + (int)(i / ZoomRate) * OriImg->getwidth()];
}
void FlipImage(IMAGE *TarImg, IMAGE *OriImg)
{
	TarImg->Resize((int)(OriImg->getwidth()), (int)(OriImg->getheight()));
	DWORD *M = GetImageBuffer(TarImg);
	DWORD *N = GetImageBuffer(OriImg);

	for (int i = 0; i < TarImg->getwidth(); i++)
		for (int j = 0; j < TarImg->getheight(); j++)
			M[j * TarImg->getwidth() + i] = N[j * TarImg->getwidth() + TarImg->getwidth() - i - 1];
}
void SketchImage(IMAGE* TarImg, IMAGE* OriImg)
{
	TarImg->Resize((int)(OriImg->getwidth()), (int)(OriImg->getheight()));
	DWORD* M = GetImageBuffer(TarImg);
	DWORD* N = GetImageBuffer(OriImg);

	for (int i = 0; i < TarImg->getheight(); i++)
		for (int j = 0; j < TarImg->getwidth(); j++)
			if (N[i * TarImg->getwidth() + j] & 0xFF000000 >> 24)
				M[i * TarImg->getwidth() + j] = BGR(RGB(255, 255, 255)) | (((DWORD)(BYTE)(255)) << 24);
}