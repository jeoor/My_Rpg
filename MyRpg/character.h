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
	int collision = 3;		   // 碰撞检测范围
	int height = 9;			   // 角色高度
	double Cx = 0.0, Cy = 0.0; // 人物位置
	int Tx = 0, Ty = 0;		   // 目标位置
	bool haveTarget = false;   // 是否有目标位置
	int currentAnimation = 0;
	Animation *animations = nullptr;
	int AnimationCount = 0;
	dir d = dir::right;
	bool flip = false; // 是否翻转
	bool isMoveUp = false,
		 isMoveDown = false,
		 isMoveLeft = false,
		 isMoveRight = false;	// 是否移动
	double Acceleration = 0.5;	// 加速度
	double CurrentSpeed = 0.0;	// 当前速度
	double MaxSpeed = 1.0;		// 最大速度
	int attackOffset = 0;		// 攻击偏移
	int attackRange = 0;		// 攻击范围
	bool alive = true;			// 是否存活
	int HP = 0;					// 人物血量
	int fullHP = 0;				// 满血时血量
	bool canReduceHP = true;	// 是否可以减血
	bool canAddHP = false;		// 是否可以减血
	int timer = 0;				// 减血冷却计时器
};

#endif