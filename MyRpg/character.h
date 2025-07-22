#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <iostream>
#include <cmath>
#include <string>
#include "animation.h"
#include "message.h"
#include "utils.h"

class Character
{
public:
	enum class dir
	{
		right,
		left
	};
	Character() = default;
	virtual ~Character() = default;
	void set(double x, double y, Animation* animations, int AnimationCount);
	void set(double x, double y);
	void set(Animation* animations, int AnimationCount);
	void updateAnimation();
	virtual void updateState() = 0;
	virtual void updateSpeed();
	void Cmove();
	void move2(int x, int y);
	void setCurrentAnimation(int index);
	int getCurrentAnimation() const;
	void changeFlip();
	double getX() const;
	double getY() const;
	int getTx() const;
	int getTy() const;
	int getCenterX() const;
	int getCenterY() const;
	int getCollision() const;
	int getHeight() const;
	int getAttackOffset() const;
	int getAttackRange() const;
	int getAttackX() const;
	int getAttackY() const;
	dir getDir() const;
	void setUp(bool isMUp);
	void setDown(bool isMDown);
	void setLeft(bool isMLeft);
	void setRight(bool isMRight);
	void setDir(dir di);
	void setHeight(int h);
	bool isMoving() const;
	bool haveT() const;
	void setTarget(int x, int y);
	void setMaxSpeed(double maxspeed);
	void setAcceleration(double acceleration);
	void sethaveTarget(bool haveT);
	void setAttackOffset(int offset);
	void setAttackRange(int range);
	void setAlive(bool Alive);
	bool isAlive();
	void Hurt();
	void setHP(int hp);
	void setCanAddHP(bool canAdd);
	void addHP(int hp);
	bool canHurt() const;
	double getHPrate() const;

private:
	int collision = 3;		   // ��ײ��ⷶΧ
	int height = 9;			   // ��ɫ�߶�
	double Cx = 0.0, Cy = 0.0; // ����λ��
	int Tx = 0, Ty = 0;		   // Ŀ��λ��
	bool haveTarget = false;   // �Ƿ���Ŀ��λ��
	int currentAnimation = 0;
	Animation *animations = nullptr;
	int AnimationCount = 0;
	dir d = dir::right;
	bool flip = false; // �Ƿ�ת
	bool isMoveUp = false,
		 isMoveDown = false,
		 isMoveLeft = false,
		 isMoveRight = false;	// �Ƿ��ƶ�
	double Acceleration = 0.5;	// ���ٶ�
	double CurrentSpeed = 0.0;	// ��ǰ�ٶ�
	double MaxSpeed = 1.0;		// ����ٶ�
	int attackOffset = 0;		// ����ƫ��
	int attackRange = 0;		// ������Χ
	bool alive = true;			// �Ƿ���
	int HP = 0;					// ����Ѫ��
	int fullHP = 0;				// ��ѪʱѪ��
	bool canReduceHP = true;	// �Ƿ���Լ�Ѫ
	bool canAddHP = false;		// �Ƿ���Լ�Ѫ
	int timer = 0;				// ��Ѫ��ȴ��ʱ��
};

#endif