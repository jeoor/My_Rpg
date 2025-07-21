#include "animation.h"

Animation::Animation(Frame *frames, int frameCount, int offsetX, int offsetY)
	: frames(frames), frameCount(frameCount), offsetX(offsetX), offsetY(offsetY)
{
	Aw = frames[0].getW();
	Ah = frames[0].getH();
	offsetXFilpped = Aw / ZOOM_RATE - offsetX, offsetYFlipped = offsetY;
}

void Animation::play(double px, double py, bool &filp, bool &canReduceHP)
{
	if (frameCount == 0)
		return;

	timer += DELTA;

	// 优化后的帧更新逻辑
	if (timer >= DELTA_TIME)
	{
		timer = 0;
		currentFrame = (currentFrame + 1) % frameCount;
	}

	// 处理反转
	if (filp)
	{
		putXOFD = px - offsetXFilpped * ZOOM_RATE;
		putYOFD = py - offsetYFlipped * ZOOM_RATE;
		frames[currentFrame].putframe(putXOFD, putYOFD, filp, canReduceHP);
	}
	else
	{
		putX = px - offsetX * ZOOM_RATE;
		putY = py - offsetY * ZOOM_RATE;
		frames[currentFrame].putframe(putX, putY, filp, canReduceHP);
	}
}

int Animation::getW() const { return Aw; }
int Animation::getH() const { return Ah; }
int Animation::getCurrentFrame() const { return currentFrame; }
int Animation::getFrameCount() const { return frameCount; }
int Animation::getFX() const { return offsetX; }
int Animation::getFY() const { return offsetY; }
void Animation::setCurrentFrame(int index)
{
	if (index >= 0 && index < frameCount)
		currentFrame = index;
	timer = 0; // 切换动画时重置计时器
}
bool Animation::haveDone() const { return currentFrame >= frameCount - 1; }
