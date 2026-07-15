#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <iostream>
#include <cmath>
#include <string>
#include "animation.h"
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
	void set(double x, double y, Animation *anims, int animationTotal);
	void set(double x, double y);
	void set(Animation *anims, int animationTotal);
	void updateAnimation();
	virtual void updateState() = 0;
	virtual void updateSpeed();
	void move();
	void move2(int x, int y, bool slowDown = true);
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
	void stopMove();
	void setDir(dir di);
	void setHeight(int h);
	bool isMoving() const;
	bool haveT() const;
	void setTarget(int x, int y);
	void setMaxSpeed(double value);
	void setAcceleration(double value);
	void setHaveTarget(bool haveT);
	void setAttackOffset(int offset);
	void setAttackRange(int range);
	bool isAlive();
	void hurt();
	void setHp(int value);
	void setCanAddHp(bool canAdd);
	void addHp(int amount);
	bool canHurt() const;
	double getHpRate() const;

  private:
	int collision = 3;					   // 碰撞检测范围
	int height = 9;						   // 角色高度
	double currentX = 0.0, currentY = 0.0; // 人物位置
	int targetX = 0, targetY = 0;		   // 目标位置
	bool haveTarget = false;			   // 是否有目标位置
	int currentAnimation = 0;
	Animation *animations = nullptr;
	int animationCount = 0;
	dir d = dir::right;
	bool flip = false; // 是否翻转
	bool isMoveUp = false, isMoveDown = false, isMoveLeft = false,
		 isMoveRight = false;  // 是否移动
	double acceleration = 0.5; // 加速度
	double currentSpeed = 0.0; // 当前速度
	double maxSpeed = 1.0;	   // 最大速度
	int attackOffset = 0;	   // 攻击偏移
	int attackRange = 0;	   // 攻击范围
	int hp = 0;				   // 人物血量
	int fullHp = 0;			   // 满血时血量
	bool canReduceHp = true;   // 是否可以减血
	bool canAddHp = false;	   // 是否可以回血
	int timer = 0;			   // 减血冷却计时器
};
#endif
