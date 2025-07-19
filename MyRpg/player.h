#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "character.h"
class Player : public Character
{
public:
	Player() = default;
	~Player() = default;
	Player(int x, int y);
	
	void updateState() override;
	void getMessage(ExMessage *msg);
	void Idle();
	void Attack();
	void Run();
	void setAttacking(bool isATK);
	bool Attacking() const;

private:
	Frame idle[4];				// 待机动画
	Frame run[8];				// 跑步动画
	Frame attack[7];			//	攻击动画
	Animation animations[3];
	bool isAttacking = false;	// 是否正在攻击
};

#endif