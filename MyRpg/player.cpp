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

    // �ܲ�����
    for (int i = 0; i < 8; ++i)
    {
        std::wstring path = L"source/characters/basic/basic_animesword_run_0" + my_utils::to_wstring(i + 1) + L".png";
        run[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
    }
    Animation runAnimation(run, 8, 8, 14);

    // ��������
    for (int i = 0; i < 7; ++i)
    {
        std::wstring path = L"source/characters/basic/basic_animesword_attack_0" + my_utils::to_wstring(i + 1) + L".png";
        attack[i].set((LPCTSTR)path.c_str(), 48, 24, ZOOM_RATE);
    }
    Animation attackAnimation(attack, 7, 8, 21, false);

    // ���ö���
    animations[0] = idleAnimation;
    animations[1] = runAnimation;
    animations[2] = attackAnimation;
    set(x, y, animations, 3);
    setMaxSpeed(PLAYER_MAX_SPEED);        // �����ƶ��ٶ�
    setAcceleration(PLAYER_ACCELERATION); // ���ü��ٶ�
	setHeight(9); // ���ý�ɫ�߶�
    setAttackOffset(10);
    setAttackRange(6);
}

void Player::updateState()
{
    // ����״̬����
    if (isAttacking())
    {
        // ֻ�ڸս��빥��״̬ʱ�л�����������֡
        if (getCurrentAnimation() != 2)
            Attack();
        // ���Ź�������
        if (animations[getCurrentAnimation()].haveDone())
            setAttacking(false);
        return;
    }

    // �ƶ�״̬
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
    // һ֡�ڴ���������Ϣ
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