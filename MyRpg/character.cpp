#include "character.h"

void Character::set(double x, double y, Animation *animations, int AnimationCount)
{
    Cx = x, Cy = y;
    this->animations = animations;
    this->AnimationCount = AnimationCount;
}
void Character::set(double x, double y) { Cx = x, Cy = y; }
void Character::set(Animation *animations, int AnimationCount)
{
    this->animations = animations;
    this->AnimationCount = AnimationCount;
}
void Character::updateAnimation()
{
    // 更新闪烁和受击状态
    if (!canReduceHP)
    {
        timer += DELTA;
        if (timer >= 1.5 * DELTA_TIME)
        {
            timer = 0;
            canReduceHP = true;
        }
    }

    if (AnimationCount == 0)
        return;

    // 边界检查计算
    Cx = (int)(Cx + WINDOWS_W) % WINDOWS_W;
    Cy = (int)(Cy + WINDOWS_H) % WINDOWS_H;
    animations[currentAnimation].play(Cx, Cy, flip, canReduceHP);

    // 绘制血条
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 2);
    double rate = HP * 1.0 / fullHP;
    double length = 30.0;
    double width = 5.0;
    double XLU = getX() - length / 2;
    double YLU = getY() - getHeight() * 1.0 - width;
    double XRB = getX() - length / 2 + length * rate;
    double YRB = getY() - getHeight() * 1.0;
    setfillcolor(BLACK);
    fillrectangle(static_cast<int>(XLU), static_cast<int>(YLU), static_cast<int>(XLU + length), static_cast<int>(YRB));
    setfillcolor(GREEN);
    fillrectangle(static_cast<int>(XLU), static_cast<int>(YLU), static_cast<int>(XRB), static_cast<int>(YRB));
}
void Character::updateSpeed()
{
    // 默认加速逻辑
    if (isMoving() || haveT())
    {
        if (CurrentSpeed < MaxSpeed)
        {
            CurrentSpeed += Acceleration;
            if (CurrentSpeed > MaxSpeed)
                CurrentSpeed = MaxSpeed;
        }
    }
    else
    {
        if (CurrentSpeed > 0.0)
        {
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
    const double MIN_DISTANCE = ZOOM_RATE * 5.0;        // 最小保持距离
    const double SLOW_DOWN_DISTANCE = ZOOM_RATE * 10.0; // 开始减速的距离

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
        setUp(false);
        setDown(false);
        setLeft(false);
        setRight(false);
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
int Character::getCurrentAnimation() const { return currentAnimation; }
void Character::changeFlip() { flip = !flip; }
double Character::getX() const { return Cx; }
double Character::getY() const { return Cy; }
int Character::getTx() const { return Tx; }
int Character::getTy() const { return Ty; }
int Character::getCenterX() const { return static_cast<int>(getX()); }
int Character::getCenterY() const { return static_cast<int>(getY() - static_cast<double>(getHeight() / 2)); }
int Character::getCollision() const { return collision * ZOOM_RATE; } // 获取碰撞检测范围
int Character::getHeight() const { return height * ZOOM_RATE; }
int Character::getAttackOffset() const { return attackOffset * ZOOM_RATE; }
int Character::getAttackRange() const { return attackRange * ZOOM_RATE; }
int Character::getAttackX() const { return getCenterX() + ((getDir() == dir::right) - (getDir() == dir::left)) * getAttackOffset(); }
int Character::getAttackY() const { return getCenterY(); }
Character::dir Character::getDir() const { return d; }
void Character::setUp(bool isMUp) { isMoveUp = isMUp; }
void Character::setDown(bool isMDown) { isMoveDown = isMDown; }
void Character::setLeft(bool isMLeft) { isMoveLeft = isMLeft; }
void Character::setRight(bool isMRight) { isMoveRight = isMRight; }
void Character::setDir(dir di) { d = di; }
void Character::setHeight(int h) { height = h; }
bool Character::isMoving() const { return isMoveUp || isMoveDown || isMoveLeft || isMoveRight; }
bool Character::haveT() const { return haveTarget; }
void Character::setTarget(int x, int y)
{
    Tx = x;
    Ty = y;
    haveTarget = true;
}
void Character::setMaxSpeed(double maxspeed) { MaxSpeed = maxspeed; }
void Character::setAcceleration(double acceleration) { Acceleration = acceleration; }
void Character::sethaveTarget(bool haveT) { haveTarget = haveT; }
void Character::setAttackOffset(int offset) { attackOffset = offset; }
void Character::setAttackRange(int range) { attackRange = range; }
void Character::setAlive(bool Alive) { alive = Alive; }
bool Character::isAlive() { return alive = (HP > 0); }
void Character::Hurt()
{
    // 防止连续伤害
    if (canReduceHP)
    {
        canReduceHP = false;
        HP--;
    }
}
void Character::setHP(int hp) { fullHP = HP = hp; }
bool Character::canHurt() const { return canReduceHP; }