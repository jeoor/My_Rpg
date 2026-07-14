#include "character.h"
#include <algorithm>

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
    // 锟斤拷锟斤拷锟斤拷烁锟斤拷锟杰伙拷状态
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

    // 锟竭斤拷锟斤拷锟斤拷锟�
    Cx = std::clamp(Cx, 0.0, static_cast<double>(WORLD_W));
    Cy = std::clamp(Cy, 0.0, static_cast<double>(WORLD_H));
    animations[currentAnimation].play(Cx, Cy, flip, canReduceHP);

    // 锟斤拷锟斤拷血锟斤拷
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 2);
    double rate = getHPrate();
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
    // 默锟较硷拷锟斤拷锟竭硷拷
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

    updateSpeed(); // 统一锟斤拷锟斤拷/锟斤拷锟斤拷锟竭硷拷

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

    // 锟斤拷锟斤拷锟斤拷小锟斤拷锟斤拷锟叫断ｏ拷锟斤拷止锟斤拷锟节接斤拷
    const double MIN_DISTANCE = ZOOM_RATE * 5.0;        // 锟斤拷小锟斤拷锟街撅拷锟斤拷
    const double SLOW_DOWN_DISTANCE = ZOOM_RATE * 10.0; // 锟斤拷始锟斤拷锟劫的撅拷锟斤拷

    if (delta_len > MIN_DISTANCE)
    {
        // 只锟斤拷x锟斤拷锟斤拷锟斤拷锟斤拷锟皆诧拷锟绞憋拷锟斤拷卸锟斤拷锟斤拷锟�
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

        // 锟斤拷锟斤拷锟狡讹拷锟斤拷志
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

        // 锟斤拷锟捷撅拷锟诫动态锟斤拷锟斤拷锟劫讹拷
        double speedFactor = 1.0;
        if (delta_len < SLOW_DOWN_DISTANCE)
        {
            // 锟节硷拷锟劫撅拷锟斤拷锟节ｏ拷锟劫讹拷锟金渐斤拷锟斤拷
            speedFactor = (delta_len - MIN_DISTANCE) / (SLOW_DOWN_DISTANCE - MIN_DISTANCE);
        }

        // 锟斤拷态锟斤拷锟劫ｏ拷锟斤拷锟斤拷锟角撅拷锟斤拷锟斤拷锟斤拷
        if (CurrentSpeed < MaxSpeed)
        {
            CurrentSpeed += Acceleration;
            if (CurrentSpeed > MaxSpeed)
                CurrentSpeed = MaxSpeed;
        }

        // 应锟矫撅拷锟斤拷锟斤拷锟截碉拷实锟斤拷锟劫讹拷
        double actualSpeed = CurrentSpeed * speedFactor;

        if (delta_len != 0 && actualSpeed > 0.0)
        {
            Cx += delta_x * actualSpeed * ZOOM_RATE / delta_len;
            Cy += delta_y * actualSpeed * ZOOM_RATE / delta_len;
        }
    }
    else
    {
        // 锟斤拷锟斤拷太锟斤拷锟斤拷停止锟狡讹拷锟斤拷锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷
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
int Character::getCenterY() const { return static_cast<int>(getY() - getHeight() * 0.5); }
int Character::getCollision() const { return collision * ZOOM_RATE; } // 锟斤拷取锟斤拷撞锟斤拷夥段�
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
bool Character::isAlive() { return HP > 0; }
void Character::Hurt()
{
    // 锟斤拷止锟斤拷锟斤拷锟剿猴拷
    if (canReduceHP)
    {
        canReduceHP = false;
        HP--;
    }
}
void Character::setHP(int hp) { fullHP = HP = hp; }
void Character::setCanAddHP(bool canAdd) { canAddHP = canAdd; }
void Character::addHP(int hp)
{
    if (HP < fullHP && canAddHP)
    {
        canAddHP = false;
        HP += hp;
    }
    if (HP >= fullHP)
        HP = fullHP;
}
bool Character::canHurt() const { return canReduceHP; }
double Character::getHPrate() const { return HP * 1.0 / fullHP; }