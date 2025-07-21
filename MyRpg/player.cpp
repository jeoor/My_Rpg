#include "player.h"

Player::Player(int x, int y)
{
    // 待机动画
    for (int i = 0; i < 4; ++i)
    {
        std::wstring path = L"source/characters/basic/basic_animesword_idle_0" + my_utils::to_wstring(i + 1) + L".png";
        idle[i].set((LPCTSTR)path.c_str(), 32, 16, ZOOM_RATE);
    }
    Animation idleAnimation(idle, 4, 5, 15);

    // 跑步动画
    for (int i = 0; i < 8; ++i)
    {
        std::wstring path = L"source/characters/basic/basic_animesword_run_0" + my_utils::to_wstring(i + 1) + L".png";
        run[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
    }
    Animation runAnimation(run, 8, 8, 14);

    // 攻击动画
    for (int i = 0; i < 7; ++i)
    {
        std::wstring path = L"source/characters/basic/basic_animesword_attack_0" + my_utils::to_wstring(i + 1) + L".png";
        attack[i].set((LPCTSTR)path.c_str(), 48, 24, ZOOM_RATE);
    }
    Animation attackAnimation(attack, 7, 8, 21, false);

    // 设置动画
    animations[0] = idleAnimation;
    animations[1] = runAnimation;
    animations[2] = attackAnimation;
    set(x, y, animations, 3);
    setMaxSpeed(PLAYER_MAX_SPEED);        // 设置移动速度
    setAcceleration(PLAYER_ACCELERATION); // 设置加速度
	setHeight(9); // 设置角色高度
    setAttackOffset(10);
    setAttackRange(6);
}

void Player::updateState()
{
    // 攻击状态优先
    if (isAttacking())
    {
        // 只在刚进入攻击状态时切换动画并重置帧
        if (getCurrentAnimation() != 2)
            Attack();
        // 播放攻击动画
        if (animations[getCurrentAnimation()].haveDone())
            setAttacking(false);
        return;
    }

    // 移动状态
    if (isMoving())
    {
        if (getCurrentAnimation() != 1)
            Run();
        Cmove();
    }
    else
    {
        if (getCurrentAnimation() != 0)
            Idle();
    }

    if (haveT())
        move2(getTx(), getTy());
}
void Player::getMessage(ExMessage *msg)
{
    // 一帧内处理所有消息
    while (peekmessage(msg, EX_MOUSE | EX_KEY))
    {
        if (msg->message == WM_KEYDOWN)
        {
            switch (msg->vkcode)
            {
            case VK_W:
            case VK_UP:
                setUp(true);
                break;
            case VK_A:
            case VK_LEFT:
                setLeft(true);
                break;
            case VK_S:
            case VK_DOWN:
                setDown(true);
                break;
            case VK_D:
            case VK_RIGHT:
                setRight(true);
                break;
            case VK_J:
                setAttacking(true);
                break;
            }
        }
        if (msg->message == WM_KEYUP)
        {
            switch (msg->vkcode)
            {
            case VK_W:
            case VK_UP:
                setUp(false);
                break;
            case VK_A:
            case VK_LEFT:
                setLeft(false);
                break;
            case VK_S:
            case VK_DOWN:
                setDown(false);
                break;
            case VK_D:
            case VK_RIGHT:
                setRight(false);
                break;
            }
        }
        if (msg->message == WM_LBUTTONUP)
        {
            setTarget(msg->x, msg->y);
        }
        if (msg->message == WM_RBUTTONDOWN)
        {
            int x = msg->x;
            // 如果鼠标点击位置在角色左侧，设置方向为左
            if (x <= getX())
            {
                if (getDir() == dir::right)
                {
                    changeFlip();
                    setDir(dir::left);
                }
            }
            // 如果鼠标点击位置在角色右侧，设置方向为右
            else if (x > getX())
            {
                if (getDir() == dir::left)
                {
                    changeFlip();
                    setDir(dir::right);
                }
            }
            setAttacking(true);
        }
    }
}

void Player::Idle() { setCurrentAnimation(0); }
void Player::Run() { setCurrentAnimation(1); }
void Player::Attack() { setCurrentAnimation(2); }
void Player::setAttacking(bool isATK)
{
    Attacking = isATK;
    if (isATK)
    {
        setCurrentAnimation(2);
        animations[2].setCurrentFrame(0);
    }
}
bool Player::isAttacking() const { return Attacking; }