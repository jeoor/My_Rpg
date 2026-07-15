#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "character.h"
class Enemy : public Character
{
  public:
	Enemy() = default;
	~Enemy() override = default;
	Enemy(int x, int y);
	void updateState() override;
	void idle();
	void attack();
	void run();
	void setAttacking(bool isAtk);
	bool isAttacking() const;

  private:
	Animation animations[3];
	bool attacking = false;
};
#endif
