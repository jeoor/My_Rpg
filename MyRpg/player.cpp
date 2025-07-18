#include "player.h"

Player::Player(int x, int y)
{
	// ��������
	for (int i = 0; i < 4; ++i)
	{
		std::wstring path = L"source/characters/basic/basic_animesword_idle_0" + my_utils::to_wstring(i + 1) + L".png";
		idle[i].set((LPCTSTR)path.c_str(), 32, 16, ZOOM_RATE);
	}
	Animation idleAnimation(idle, 4, 5, 15);

	// ��������
	for (int i = 0; i < 7; ++i)
	{
		std::wstring path = L"source/characters/basic/basic_animesword_attack_0" + my_utils::to_wstring(i + 1) + L".png";
		attack[i].set((LPCTSTR)path.c_str(), 48, 24, ZOOM_RATE);
	}
	Animation attackAnimation(attack, 7, 8, 21, false);

	// �ܲ�����
	for (int i = 0; i < 8; ++i)
	{
		std::wstring path = L"source/characters/basic/basic_animesword_run_0" + my_utils::to_wstring(i + 1) + L".png";
		run[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
	}
	Animation runAnimation(run, 8, 8, 14);

	// ���ö���
	animations[0] = idleAnimation;
	animations[1] = attackAnimation;
	animations[2] = runAnimation;
	set(x, y, animations, 3);
}

void Player::updateState()
{
    // ����״̬����
    if (Attacking())
    {
        // ֻ�ڸս��빥��״̬ʱ�л�����������֡
        if (getCurrentAnimation() != 1)
        {
            setCurrentAnimation(1);
            animations[1].setCurrentFrame(0);
        }
        // ���Ź�������
        if (animations[getCurrentAnimation()].haveDone())
        {
            setAttacking(false);
            animations[0].setCurrentFrame(0);
        }
        return;
    }

    // �ƶ�״̬
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
    // һ֡�ڴ���������Ϣ
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
			// ��������λ���ڽ�ɫ��࣬���÷���Ϊ��
            if (x <= getX())
            {
                if (getDir() == dir::right)
                {
                    changeFlip();
                    setDir(dir::left);
                }
            }
            // ��������λ���ڽ�ɫ�Ҳ࣬���÷���Ϊ��
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