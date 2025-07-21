#ifndef __ENEMY_ATLAS_H__
#define __ENEMY_ATLAS_H__

#include <string>
#include "utils.h"
#include "animation.h"
class Atlas
{
public:
	Atlas();
	~Atlas() = default;
	Frame idle[4];	 // 待机动画
	Frame run[4];	 // 跑步动画
	Frame attack[6]; //	攻击动画
	Animation animations[3];
};

#endif