#include "character.h"

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
void Character::updateAnimation()
{
	if (AnimationCount == 0) return;

	// 边界检查计算
	Cx = (Cx + WINDOWS_W) % WINDOWS_W;
	Cy = (Cy + WINDOWS_H) % WINDOWS_H;
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
			changeFlip();
			d = dir::left;
		}
	}
	if (isMoveRight)
	{
		if (d == dir::left)
		{
			changeFlip();
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
void Character::move2(int x, int y)
{
    double delta_x = static_cast<double>(x) - Cx;
    double delta_y = static_cast<double>(y) - Cy;

    // 判断并设置左右移动
    if (delta_x <= 0)
    {
        setLeft(true);
        setRight(false);
        if (d == dir::right)
        {
            changeFlip();
            d = dir::left;
        }
    }
    else
    {
        setRight(true);
        setLeft(false);
        if (d == dir::left)
        {
            changeFlip();
            d = dir::right;
        }
    }

    // 判断并设置上下移动
    if (delta_y <= 0) setUp(true);
    else setDown(true);

    double delta_len = sqrt(delta_x * delta_x + delta_y * delta_y);
    if (delta_len > 10.0)
    {
		std::cout << Cx << ", " << Cy << " -> " << x << ", " << y << " : " << delta_len << std::endl;
        double unit_x = delta_x / delta_len;
        double unit_y = delta_y / delta_len;
        Cx += static_cast<int>(unit_x * Step * ZOOM_RATE);
        Cy += static_cast<int>(unit_y * Step * ZOOM_RATE);
    }
    else
    {
        haveTarget = false;
        std::cout << "DONE! " << Cx << ", " << Cy << " -> " << x << ", " << y << " : " << delta_len << std::endl;
        setUp(false); setDown(false); setLeft(false); setRight(false); // 只在停止时清除
    }
}
void Character::setCurrentAnimation(int index)
{
	// 设置当前动画
	if (index >= 0 && index < AnimationCount)
		currentAnimation = index;
}
int Character::getCurrentAnimation() const { return  currentAnimation; }
void Character::changeFlip() { flip = !flip; }
int Character::getX() const { return Cx; }
int Character::getY() const { return Cy; }
int Character::getTx() const { return Tx; }
int Character::getTy() const { return Ty; }
int Character::getW() const { return Cw; }
int Character::getH() const { return Ch; }
Character::dir& Character::getDir() { return d; }
void Character::setUp(bool isMUp) { isMoveUp = isMUp; }
void Character::setDown(bool isMDown) { isMoveDown = isMDown; }
void Character::setLeft(bool isMLeft) { isMoveLeft = isMLeft; }
void Character::setRight(bool isMRight) { isMoveRight = isMRight; }
void Character::setDir(dir di) { d = di; }
bool Character::isMoving() const { return  isMoveUp || isMoveDown || isMoveLeft || isMoveRight; }
bool Character::haveT() const { return haveTarget; }
void Character::setTarget(int x, int y) { Tx = x; Ty = y; haveTarget = true; }