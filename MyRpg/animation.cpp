#include "animation.h"

Animation::Animation(Frame *frames, int frameCount, int offsetX, int offsetY, bool loop)
	: frames(frames), frameCount(frameCount), offsetX(offsetX), offsetY(offsetY), loop(loop)
{
	Aw = frames[0].getW();
	Ah = frames[0].getH();
	offsetXFilpped = Aw / ZOOM_RATE - offsetX, offsetYFlipped = offsetY;
}

void Animation::play(double px, double py, bool &filp)
{
	if (frameCount == 0)
		return;

	timer += DELTA;

	// 优化后的帧更新逻辑
	if (timer >= DELTA_TIME)
	{
		timer = 0;
		if (loop) {
			currentFrame = (currentFrame + 1) % frameCount;
		} else if (currentFrame < frameCount - 1) {
			++currentFrame;
		}
	}

	// 处理反转
	if (filp)
	{
		putXOFD = px - offsetXFilpped * ZOOM_RATE;
		putYOFD = py - offsetYFlipped * ZOOM_RATE;
		frames[currentFrame].putframe(putXOFD, putYOFD, filp);
	}
	else
	{
		putX = px - offsetX * ZOOM_RATE;
		putY = py - offsetY * ZOOM_RATE;
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
	timer = 0; // 切换动画时重置计时器
}
bool Animation::haveDone() const { return currentFrame >= frameCount - 1; }
