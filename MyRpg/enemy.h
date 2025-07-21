#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "character.h"

class Enemy : public Character
{
public:
	Enemy() = default;
	~Enemy() = default;
	Enemy(int x, int y);
	void updateState() override;
	void Idle();
	void Attack();
	void Run();
	void setAttacking(bool isATK);
	bool isAttacking() const;

private:
	Frame idle[4];	 // 待机动画
	Frame run[4];	 // 跑步动画
	Frame attack[6]; //	攻击动画
	Animation animations[3];
	bool Attacking = false; // 是否正在攻击
};

#endif
