#include "character.h"

void Character::set(double x, double y, Animation* animations, int AnimationCount)
{
	Cx = x, Cy = y;
	this->animations = animations;
	this->AnimationCount = AnimationCount;
}
void Character::set(double x, double y) { Cx = x, Cy = y; }
void Character::set(Animation* animations, int AnimationCount)
{
	this->animations = animations;
	this->AnimationCount = AnimationCount;
}
void Character::updateAnimation()
{
	if (AnimationCount == 0) return;

	// 边界检查计算
	Cx = (int)(Cx + WINDOWS_W) % WINDOWS_W;
	Cy = (int)(Cy + WINDOWS_H) % WINDOWS_H;
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
		Cx += delta_x * Speed * ZOOM_RATE / delta_len;
		Cy += delta_y * Speed * ZOOM_RATE / delta_len;
	}
}
void Character::move2(int x, int y)
{
    double delta_x = static_cast<double>(x) - Cx;
    double delta_y = static_cast<double>(y) - Cy;
    double delta_len = sqrt(delta_x * delta_x + delta_y * delta_y);

    if (delta_len > ZOOM_RATE * 5.0)
    {
        // 只在x方向有明显差距时才判断左右
        if (delta_x < -1e-3)
        {
            setLeft(true);
            setRight(false);
            if (d != dir::left)
            {
                changeFlip();
                d = dir::left;
            }
        }
        else if (delta_x > 1e-3)
        {
            setRight(true);
            setLeft(false);
            if (d != dir::right)
            {
                changeFlip();
                d = dir::right;
            }
        }
        else
        {
            setLeft(false);
            setRight(false);
        }

        // 上下移动标志
        if (delta_y < -1e-3)
        {
            setUp(true);
            setDown(false);
        }
        else if (delta_y > 1e-3)
        {
            setDown(true);
            setUp(false);
        }
        else
        {
            setUp(false);
            setDown(false);
        }

        double unit_x = delta_x / delta_len;
        double unit_y = delta_y / delta_len;
        Cx += unit_x * Speed * ZOOM_RATE;
        Cy += unit_y * Speed * ZOOM_RATE;
    }
    else
    {
        std::cout << "Done! " << Cx << ", " << Cy << " -> " << x << ", " << y << " : " << delta_len << std::endl;
        haveTarget = false;
        setUp(false); setDown(false); setLeft(false); setRight(false);
    }
}
void Character::setCurrentAnimation(int index)
{
    if (currentAnimation != index)
    {
        currentAnimation = index;
        animations[index].setCurrentFrame(0);
    }
}
int Character::getCurrentAnimation() const { return  currentAnimation; }
void Character::changeFlip() { flip = !flip; }
double Character::getX() const { return Cx; }
double Character::getY() const { return Cy; }
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
void Character::setSpeed(double speed) { Speed = speed; }