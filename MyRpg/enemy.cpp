#include "enemy.h"
#include "ai.h"
#include "collision.h"
#include "asset.h"
Enemy::Enemy(int x, int y)
{
	auto &am = AssetManager::getInstance();
	Animation idleAnim(am.getFrames(L"enemy_idle"), 4, 6, 15);
	Animation runAnim(am.getFrames(L"enemy_run"), 4, 5, 14);
	Animation atkAnim(am.getFrames(L"enemy_attack"), 6, 6, 15);
	animations[0] = idleAnim;
	animations[1] = runAnim;
	animations[2] = atkAnim;
	set(x, y, animations, 3);
	setMaxSpeed(ENEMY_MAX_SPEED);
	setAcceleration(ENEMY_ACCELERATION);
	setHeight(10);
	setAttackOffset(6);
	setAttackRange(3);
	setHp(7);
}
void Enemy::updateState()
{
	// 只负责动画状态, 移动由 FishAI::navigate() 处理
	if (isAttacking())
	{
		if (getCurrentAnimation() != 2)
			attack();
		if (animations[getCurrentAnimation()].haveDone())
			setAttacking(false);
		return;
	}
	if (isMoving())
	{
		if (getCurrentAnimation() != 1)
			run();
	}
	else
	{
		if (getCurrentAnimation() != 0)
			idle();
	}
}
void Enemy::idle() { setCurrentAnimation(0); }
void Enemy::run() { setCurrentAnimation(1); }
void Enemy::attack() { setCurrentAnimation(2); }
void Enemy::setAttacking(bool isAtk)
{
	if (isAtk && !attacking)
		setCurrentAnimation(2);
	attacking = isAtk;
}
bool Enemy::isAttacking() const { return attacking; }
