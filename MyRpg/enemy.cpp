#include "enemy.h"

Enemy::Enemy(int x, int y)
{
    // 待机动画
    for (int i = 0; i < 4; ++i)
    {
        std::wstring path = L"source/characters/mooseman/mooseman_idle_0" + my_utils::to_wstring(i + 1) + L".png";
        idle[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
    }
    Animation idleAnimation(idle, 4, 6, 15);

    // 跑步动画
    for (int i = 0; i < 4; ++i)
    {
        std::wstring path = L"source/characters/mooseman/mooseman_run_0" + my_utils::to_wstring(i + 1) + L".png";
        run[i].set((LPCTSTR)path.c_str(), 16, 16, ZOOM_RATE);
    }
    Animation runAnimation(run, 4, 5, 14);

    // 攻击动画
    for (int i = 0; i < 6; ++i)
    {
        std::wstring path = L"source/characters/mooseman/mooseman_attack_0" + my_utils::to_wstring(i + 1) + L".png";
        attack[i].set((LPCTSTR)path.c_str(), 20, 16, ZOOM_RATE);
    }
    Animation attackAnimation(attack, 6, 6, 15);

    // 设置动画
    animations[0] = idleAnimation;
    animations[1] = runAnimation;
    animations[2] = attackAnimation;
    set(x, y, animations, 3);
    setMaxSpeed(ENEMY_MAX_SPEED);
    setAcceleration(ENEMY_ACCELERATION);
    setHeight(10);
    setAttackOffset(6);
    setAttackRange(3);
    setHP(7);
}

void Enemy::updateState()
{
    // 攻击状态优先
    if (isAttacking())
    {
        if (getCurrentAnimation() != 2)
            Attack();
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

void Enemy::Idle() { setCurrentAnimation(0); }
void Enemy::Run() { setCurrentAnimation(1); }
void Enemy::Attack() { setCurrentAnimation(2); }
void Enemy::setAttacking(bool isATK)
{
    Attacking = isATK;
    if (isATK)
        setCurrentAnimation(2);
}
bool Enemy::isAttacking() const { return Attacking; }
