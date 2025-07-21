#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "frame.h"
#include "config.h"
class Animation
{
public:
	Animation() = default;
	Animation(Frame *frames, int frameCount, int offsetX = 0, int offsetY = 0);
	void play(double px, double py, bool &filp, bool &canReduceHP);
	int getW() const;
	int getH() const;
	int getCurrentFrame() const;
	int getFrameCount() const;
	int getFX() const;
	int getFY() const;
	void setCurrentFrame(int index);
	bool haveDone() const;

private:
	Frame *frames = nullptr;
	int frameCount = 0;
	int currentFrame = 0;
	int Aw = 0, Ah = 0;							// ���ؿ��
	int offsetX = 0, offsetY = 0;				// ����ƫ����
	int offsetXFilpped = 0, offsetYFlipped = 0; // ��ת���ƫ����
	double putX = 0.0, putY = 0.0;				// ����λ��
	double putXOFD = 0.0, putYOFD = 0.0;		// ��ת��Ļ���λ��
	int timer = 0;								// ��ʱ��
};

#endif