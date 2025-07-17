#include "character.h"

Character::Character(int x, int y, Animation* animations, int AnimationCount)
{
	set(x, y, animations, AnimationCount);
}
void Character::set(int x, int y, Animation* animations, int AnimationCount)
{
	Cx = x, Cy = y;
	this->animations = animations;
	this->AnimationCount = AnimationCount;
}
void Character::set(int x, int y) { Cx = x, Cy = y; }
void Character::set(Animation* animations, int AnimationCount)
{
	this->animations = animations;
	this->AnimationCount = AnimationCount;
}
void Character::playAnimation()
{
	if (AnimationCount == 0) return;

	// 边界检查计算
	Cx = (Cx + WINDOWS_W) % WINDOWS_W;
	Cy = (Cy + WINDOWS_H) % WINDOWS_H;
	/*
	// 处理非循环动画
	if (!animations[currentAnimation].Loop())
	{
		if (animations[currentAnimation].getCurrentFrame() ==
				 animations[currentAnimation].getFrameCount() - 1)
		{
		   animations[currentAnimation].setCurrentFrame(0); // 为下一次播放准备
			currentAnimation = 0; // 回到默认动画
		}
	}
	*/
	animations[currentAnimation].play(Cx, Cy, flip);
	Cw = animations[currentAnimation].getW();
	Ch = animations[currentAnimation].getH();
}
void Character::Cmove()
{
	if (isMoveLeft)
	{
		if (d == dir::right)
		{
			changeFilp();
			d = dir::left;
		}
	}
	if (isMoveRight)
	{
		if (d == dir::left)
		{
			changeFilp();
			d = dir::right;
		}
	}

	int delta_x = isMoveRight - isMoveLeft;
	int delta_y = isMoveDown - isMoveUp;

	double delta_len = sqrt(delta_x * delta_x + delta_y * delta_y);
	if (delta_len != 0)
	{
		Cx += static_cast<int>(delta_x * Step * ZOOM_RATE / delta_len);
		Cy += static_cast<int>(delta_y * Step * ZOOM_RATE / delta_len);
	}
}
void Character::KBmove(ExMessage& msg)
{
	if (!KBcontrol) return;
	while (peekmessage(&msg))
	{
		if (msg.message == WM_KEYDOWN)
		{
			if (!KBcontrol) KBcontrol = true;
			switch (msg.vkcode)
			{
			case VK_W:
				setUp(true);
				break;
			case VK_A:
				setLeft(true);
				break;
			case VK_S:
				setDown(true);
				break;
			case VK_D:
				setRight(true);
				break;
			}
		}
		if (msg.message == WM_KEYUP)
		{
			switch (msg.vkcode)
			{
			case VK_W:
				setUp(false);
				break;
			case VK_A:
				setLeft(false);
				break;
			case VK_S:
				setDown(false);
				break;
			case VK_D:
				setRight(false);
				break;
			}
		}
	}
}
void Character::move2(int x, int y)
{
	int delta_x = x - Cx;
	int delta_y = y - Cy;

	// 判断并设置左右移动
	if (delta_x < 0)
	{
		setLeft(true);
		if (d == dir::right)
		{
			changeFilp();
			d = dir::left;
		}
	}
	else if (delta_x > 0)
	{
		setRight(true);
		if (d == dir::left)
		{
			changeFilp();
			d = dir::right;
		}
	}

	// 判断并设置上下移动
	if (delta_y < 0) setUp(true);
	else if (delta_y > 0) setDown(true);

	double delta_len = sqrt(delta_x * delta_x + delta_y * delta_y);
	if (delta_len >= 10)
	{
		std::cout << "MOVING... " << Cx << ", " << Cy << " : " << delta_len << std::endl;
		Cx += static_cast<int>(delta_x * Step * ZOOM_RATE / delta_len);
		Cy += static_cast<int>(delta_y * Step * ZOOM_RATE / delta_len);
	}
	else
	{
		std::cout << "DONE! " << Cx << ", " << Cy << " -> " << x << ", " << y << " : " << delta_len << std::endl;
		setKBcontrol(true);
		setCurrentAnimation(0);
		setUp(false); setDown(false); setLeft(false); setRight(false);
	}
}
void Character::setCurrentAnimation(int index)
{
	// 设置当前动画
	if (index >= 0 && index < AnimationCount)
		currentAnimation = index;
}

void Character::changeFilp() { flip = !flip; }
int Character::getX() const { return Cx; }
int Character::getY() const { return Cy; }
int Character::getW() const { return Cw; }
int Character::getH() const { return Ch; }
Character::dir& Character::getDir() { return d; }
void Character::setUp(bool isMUp) { isMoveUp = isMUp; }
void Character::setDown(bool isMDown) { isMoveDown = isMDown; }
void Character::setLeft(bool isMLeft) { isMoveLeft = isMLeft; }
void Character::setRight(bool isMRight) { isMoveRight = isMRight; }
void Character::setDir(dir di) { d = di; }
bool Character::isMoving() const { return  isMoveUp || isMoveDown || isMoveLeft || isMoveRight; }
void Character::setKBcontrol(bool isKB) { KBcontrol = isKB; }
bool Character::KBcon() const { return KBcontrol; }