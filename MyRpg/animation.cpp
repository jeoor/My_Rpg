#include "animation.h"

Animation::Animation(Frame* frames, int frameCount, int offsetX, int offsetY, bool loop)
	: frames(frames), frameCount(frameCount), offsetX(offsetX), offsetY(offsetY), loop(loop)
{
	Aw = frames[0].getW();
	Ah = frames[0].getH();
	offsetXFilpped = Aw / ZOOM_RATE - offsetX - 2, offsetYFlipped = offsetY;
}
void Animation::play(int px, int py, bool& filp)
{
	if (frameCount == 0)
		return;

	timer += DELTA;

	// ����֡
	if (timer >= DELTA_TIME)
	{
		timer = 0; // ���ü�ʱ��

		if (loop || currentFrame < frameCount - 1) // �����ѭ���������һ֡
			currentFrame++;
		else
			currentFrame = 0; // �������ѭ�����ص���һ֡
	}
	// ��ֹԽ��
	currentFrame = (currentFrame + frameCount) % frameCount;
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
}
