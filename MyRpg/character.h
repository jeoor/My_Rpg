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
	void Cmove();
	void move2(int x, int y);
	void setCurrentAnimation(int index);
	int getCurrentAnimation() const;
	void changeFlip();
	double getX() const;
	double getY() const;
	int getTx() const;
	int getTy() const;
	int getW() const;
	int getH() const;
	dir& getDir();
	void setUp(bool isMUp);
	void setDown(bool isMDown);
	void setLeft(bool isMLeft);
	void setRight(bool isMRight);
	void setDir(dir di);
	bool isMoving() const;
	bool haveT() const;
	void setTarget(int x, int y);
	void setSpeed(double speed);

private:
	double Speed = 1.0;				// 移动速度
	double Cx = 0.0, Cy = 0.0;		// 人物位置
	int Tx = 0, Ty = 0;				// 目标位置
	bool haveTarget = false;		// 是否有目标位置
	int currentAnimation = 0;
	Animation* animations = nullptr;
	int AnimationCount = 0;
	int Cw = 0, Ch = 0;
	dir d = dir::right;
	bool flip = false;				// 是否翻转
	bool isMoveUp = false,
		isMoveDown = false,
		isMoveLeft = false,
		isMoveRight = false;		// 是否移动
};

#endif