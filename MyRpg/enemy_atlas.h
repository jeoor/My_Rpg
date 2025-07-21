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
	Frame idle[4];	 // ��������
	Frame run[4];	 // �ܲ�����
	Frame attack[6]; //	��������
	Animation animations[3];
};

#endif