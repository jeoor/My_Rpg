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

	// 攻击动画
	for (int i = 0; i < 7; ++i)
	{
		std::wstring path = L"source/characters/basic/basic_animesword_attack_0" + my_utils::to_wstring(i + 1) + L".png";
		attack[i].set((LPCTSTR)path.c_str(), 48, 24, ZOOM_RATE);
	}
	Animation attackAnimation(attack, 7, 8, 21, false);

	// 跑步动画
	for (int i = 0; i < 8; ++i)
	{
		std::wstring path = L"source/characters/basic/basic_animesword_run_0" + my_utils::to_wstring(i + 1) + L".png";
		run[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
	}
	Animation runAnimation(run, 8, 8, 14);

	// 设置动画
	animations[0] = idleAnimation;
	animations[1] = attackAnimation;
	animations[2] = runAnimation;
	set(x, y, animations, 3);
}

void Player::updateState()
{
    // 攻击状态优先
    if (Attacking())
    {
        // 只在刚进入攻击状态时切换动画并重置帧
        if (getCurrentAnimation() != 1)
        {
            setCurrentAnimation(1);
            animations[1].setCurrentFrame(0);
        }
        // 播放攻击动画
        if (animations[getCurrentAnimation()].haveDone())
        {
            setAttacking(false);
            animations[0].setCurrentFrame(0);
        }
        return;
    }

    // 移动状态
    if (isMoving())
    {
        if (getCurrentAnimation() != 2)
        {
            setCurrentAnimation(2);
            animations[2].setCurrentFrame(0);
        }
        Cmove();
    }
    else
    {
        if (getCurrentAnimation() != 0)
        {
            setCurrentAnimation(0);
            animations[0].setCurrentFrame(0);
        }
    }

    if (haveT())
        move2(getTx(), getTy());
}
void Player::getMessage(ExMessage& msg)
{
    // 一帧内处理所有消息
    while (peekmessage(&msg, EX_MOUSE | EX_KEY))
    {
        if (msg.message == WM_KEYDOWN)
        {
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
            case VK_J: 
                setAttacking(true); break;
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
        if (msg.message == WM_LBUTTONDOWN)
        {
            setTarget(msg.x, msg.y);
        }
        if (msg.message == WM_RBUTTONDOWN)
        {
			int x = msg.x;
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
void Player::Attack() { setCurrentAnimation(1); }
void Player::Run() { setCurrentAnimation(2); }
void Player::setAttacking(bool isATK) { isAttacking = isATK; }
bool Player::Attacking() const { return isAttacking; }