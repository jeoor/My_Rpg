#include "enemy.h"

Enemy::Enemy(int x, int y)
{
    // ��������
    for (int i = 0; i < 4; ++i)
    {
        std::wstring path = L"source/characters/mooseman/mooseman_idle_0" + my_utils::to_wstring(i + 1) + L".png";
        idle[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
    }
    Animation idleAnimation(idle, 4, 6, 15);

    // �ܲ�����
    for (int i = 0; i < 4; ++i)
    {
        std::wstring path = L"source/characters/mooseman/mooseman_run_0" + my_utils::to_wstring(i + 1) + L".png";
        run[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
    }
    Animation runAnimation(run, 4, 5, 14);

    // ��������
    for (int i = 0; i < 6; ++i)
    {
        std::wstring path = L"source/characters/mooseman/mooseman_attack_0" + my_utils::to_wstring(i + 1) + L".png";
        attack[i].set((LPCTSTR)path.c_str(), 20, 16, ZOOM_RATE);
    }
    Animation attackAnimation(attack, 6, 6, 15, false);


    // ���ö���
    animations[0] = idleAnimation;
    animations[1] = runAnimation;
    animations[2] = attackAnimation;
    set(x, y, animations, 3);
	setMaxSpeed(ENEMY_MAX_SPEED); // �����ƶ��ٶ�
	setAcceleration(ENEMY_ACCELERATION); // ���ü��ٶ�
}

void Enemy::updateState()
{
    // ����״̬����
    if (Attacking())
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

void Enemy::Idle() { setCurrentAnimation(0); }
void Enemy::Run() { setCurrentAnimation(1); }
void Enemy::Attack() { setCurrentAnimation(2); }
void Enemy::setAttacking(bool isATK)
{
    isAttacking = isATK;
    if (isATK)
    {
        setCurrentAnimation(2);
        animations[2].setCurrentFrame(0);
    }
}
bool Enemy::Attacking() const { return isAttacking; }