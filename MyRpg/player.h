#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
class Player : public Character
{
private:
	Frame idle[4];				// ��������
	Frame attack[7];			//	��������
	Frame run[8];				// �ܲ�����
	Animation animations[3];
	bool isAttacking = false;	// �Ƿ����ڹ���

public:
	~Player() override = default;
	void init() override;

	void Idle();
	void Attack();
	void Run();
	void setAttacking(bool isATK);
	bool Attacking() const;
};

#endif