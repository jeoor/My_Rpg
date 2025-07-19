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

	// �߽������
	Cx = (int)(Cx + WINDOWS_W) % WINDOWS_W;
	Cy = (int)(Cy + WINDOWS_H) % WINDOWS_H;
	animations[currentAnimation].play(Cx, Cy, flip);
	Cw = animations[currentAnimation].getW();
	Ch = animations[currentAnimation].getH();
}
void Character::updateSpeed()
{
    // Ĭ�ϼ����߼�
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

    updateSpeed(); // ͳһ����/�����߼�

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

    // ������С�����жϣ���ֹ���ڽӽ�
    const double MIN_DISTANCE = ZOOM_RATE * 6.0; // ��С���־���
    const double SLOW_DOWN_DISTANCE = ZOOM_RATE * 20.0; // ��ʼ���ٵľ���

    if (delta_len > MIN_DISTANCE)
    {
        // ֻ��x���������Բ��ʱ���ж�����
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

        // �����ƶ���־
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

        // ���ݾ��붯̬�����ٶ�
        double speedFactor = 1.0;
        if (delta_len < SLOW_DOWN_DISTANCE)
        {
            // �ڼ��پ����ڣ��ٶ��𽥽���
            speedFactor = (delta_len - MIN_DISTANCE) / (SLOW_DOWN_DISTANCE - MIN_DISTANCE);
        }

        // ��̬���٣������Ǿ�������
        if (CurrentSpeed < MaxSpeed)
        {
            CurrentSpeed += Acceleration;
            if (CurrentSpeed > MaxSpeed)
                CurrentSpeed = MaxSpeed;
        }
        
        // Ӧ�þ������ص�ʵ���ٶ�
        double actualSpeed = CurrentSpeed * speedFactor;

        if (delta_len != 0 && actualSpeed > 0.0)
        {
            Cx += delta_x * actualSpeed * ZOOM_RATE / delta_len;
            Cy += delta_y * actualSpeed * ZOOM_RATE / delta_len;
        }
    }
    else
    {
        // ����̫����ֹͣ�ƶ�������һ������
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
int Character::getCollision() const { return collision; } // ��ȡ��ײ��ⷶΧ
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