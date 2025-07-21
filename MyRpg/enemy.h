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
	Frame idle[4];	 // ��������
	Frame run[4];	 // �ܲ�����
	Frame attack[6]; //	��������
	Animation animations[3];
	bool Attacking = false; // �Ƿ����ڹ���
};

#endif
