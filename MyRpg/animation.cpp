#include "animation.h"

Animation::Animation(Frame* frames, int frameCount, int offsetX, int offsetY, bool loop)
	: frames(frames), frameCount(frameCount), offsetX(offsetX), offsetY(offsetY), loop(loop)
{
	Aw = frames[0].getW();
	Ah = frames[0].getH();
	offsetXFilpped = Aw / ZOOM_RATE - offsetX - 2, offsetYFlipped = offsetY;
}
void Animation::play(double px, double py, bool& filp)
{
	if (frameCount == 0)
		return;

	timer += DELTA;

	// ֻ�ж���δ���ʱ�Ÿ���֡
	if ((loop || currentFrame < frameCount - 1) && timer >= DELTA_TIME)
	{
		timer = 0;
		currentFrame++;
		if (loop && currentFrame >= frameCount)
			currentFrame = 0;
		else if (!loop && currentFrame >= frameCount)
			currentFrame = frameCount - 1; // ��ѭ������ͣ�����һ֡
	}

	// ����ת
	if (filp)
	{
		putXOFD = px - offsetXFilpped * ZOOM_RATE, putYOFD = py - offsetYFlipped * ZOOM_RATE;
		frames[currentFrame].putframe(putXOFD, putYOFD, filp);
	}
	else
	{
		putX = px - offsetX * ZOOM_RATE, putY = py - offsetY * ZOOM_RATE;
		frames[currentFrame].putframe(putX, putY, filp);
	}
}

int Animation::getW() const { return Aw; }
int Animation::getH() const { return Ah; }
int Animation::getCurrentFrame() const { return currentFrame; }
int Animation::getFrameCount() const { return frameCount; }
int Animation::getFX() const { return offsetX; }
int Animation::getFY() const { return offsetY; }
bool Animation::Loop() const { return loop; }
void Animation::setCurrentFrame(int index)
{
	if (index >= 0 && index < frameCount)
		currentFrame = index;
	timer = 0; // �л�����ʱ���ü�ʱ��
}
bool Animation::haveDone() const { return currentFrame >= frameCount - 1; }
