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
void Character::updateSpeed()
{
    // 默认加速逻辑
    if (isMoving() || haveT()) {
        if (CurrentSpeed < MaxSpeed) {
            CurrentSpeed += Acceleration;
            if (CurrentSpeed > MaxSpeed)
                CurrentSpeed = MaxSpeed;
        }
    }
    else {
        if (CurrentSpeed > 0.0) {
            CurrentSpeed -= Acceleration;
            if (CurrentSpeed < 0.0)
                CurrentSpeed = 0.0;
        }
    }
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

    updateSpeed(); // 统一加速/减速逻辑

    if (delta_len != 0 && CurrentSpeed > 0.0)
    {
        Cx += delta_x * CurrentSpeed * ZOOM_RATE / delta_len;
        Cy += delta_y * CurrentSpeed * ZOOM_RATE / delta_len;
    }
}
void Character::move2(int x, int y)
{
    double delta_x = static_cast<double>(x) - Cx;
    double delta_y = static_cast<double>(y) - Cy;
    double delta_len = sqrt(delta_x * delta_x + delta_y * delta_y);

    // 增加最小距离判断，防止过于接近
    const double MIN_DISTANCE = ZOOM_RATE * 6.0; // 最小保持距离
    const double SLOW_DOWN_DISTANCE = ZOOM_RATE * 20.0; // 开始减速的距离

    if (delta_len > MIN_DISTANCE)
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

        // 根据距离动态调整速度
        double speedFactor = 1.0;
        if (delta_len < SLOW_DOWN_DISTANCE)
        {
            // 在减速距离内，速度逐渐降低
            speedFactor = (delta_len - MIN_DISTANCE) / (SLOW_DOWN_DISTANCE - MIN_DISTANCE);
        }

        // 动态加速，但考虑距离因素
        if (CurrentSpeed < MaxSpeed)
        {
            CurrentSpeed += Acceleration;
            if (CurrentSpeed > MaxSpeed)
                CurrentSpeed = MaxSpeed;
        }
        
        // 应用距离因素到实际速度
        double actualSpeed = CurrentSpeed * speedFactor;

        if (delta_len != 0 && actualSpeed > 0.0)
        {
            Cx += delta_x * actualSpeed * ZOOM_RATE / delta_len;
            Cy += delta_y * actualSpeed * ZOOM_RATE / delta_len;
        }
    }
    else
    {
        // 距离太近，停止移动并保持一定距离
        CurrentSpeed = 0.0;
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
int Character::getCollision() const { return collision; } // 获取碰撞检测范围
int Character::getHeight() const { return height; }
Character::dir& Character::getDir() { return d; }
void Character::setUp(bool isMUp) { isMoveUp = isMUp; }
void Character::setDown(bool isMDown) { isMoveDown = isMDown; }
void Character::setLeft(bool isMLeft) { isMoveLeft = isMLeft; }
void Character::setRight(bool isMRight) { isMoveRight = isMRight; }
void Character::setDir(dir di) { d = di; }
void Character::setHeight(int h) { height = h; }
bool Character::isMoving() const { return  isMoveUp || isMoveDown || isMoveLeft || isMoveRight; }
bool Character::haveT() const { return haveTarget; }
void Character::setTarget(int x, int y) { Tx = x; Ty = y; haveTarget = true; }
void Character::setMaxSpeed(double maxspeed) { MaxSpeed = maxspeed; }
void Character::setAcceleration(double acceleration) { Acceleration = acceleration; }
void Character::sethaveTarget(bool haveT) { haveTarget = haveT; }