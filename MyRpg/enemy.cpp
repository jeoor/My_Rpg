#include "enemy.h"

Enemy::Enemy(int x, int y)
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

void Enemy::updateState()
{
    // ����״̬����
    if (Attacking())
    {
        // ֻ�ڸս��빥��״̬ʱ�л�����������֡
        if (getCurrentAnimation() != 1)
            Attack();
        // ���Ź�������
        if (animations[getCurrentAnimation()].haveDone())
            setAttacking(false);
        return;
    }

    // �ƶ�״̬
    if (isMoving())
    {
        if (getCurrentAnimation() != 2)
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
void Enemy::Idle() { setCurrentAnimation(0); }
void Enemy::Attack() { setCurrentAnimation(1); }
void Enemy::Run() { setCurrentAnimation(2); }
void Enemy::setAttacking(bool isATK)
{
    isAttacking = isATK;
    if (isATK)
    {
        setCurrentAnimation(1);
        animations[1].setCurrentFrame(0);
    }
}
bool Enemy::Attacking() const { return isAttacking; }