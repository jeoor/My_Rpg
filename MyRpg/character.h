#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <iostream>
#include <cmath>
#include <string>
#include "animation.h"
#include "kbmessage.h"
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
	void set(int x, int y, Animation* animations, int AnimationCount);
	void set(int x, int y);
	void set(Animation* animations, int AnimationCount);
	void playAnimation();
	void Cmove();
	void KBmove(ExMessage& msg);
	void move2(int x, int y);
	void setCurrentAnimation(int index);
	void changeFilp();
	int getX() const;
	int getY() const;
	int getW() const;
	int getH() const;
	dir& getDir();
	void setUp(bool isMUp);
	void setDown(bool isMDown);
	void setLeft(bool isMLeft);
	void setRight(bool isMRight);
	void setDir(dir di);
	bool isMoving() const;
	void setKBcontrol(bool isKB);
	bool KBcon() const;

private:
	int Cx = 0, Cy = 0;				// 人物位置
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
	bool KBcontrol = true;			// 正在键盘控制
};

#endif