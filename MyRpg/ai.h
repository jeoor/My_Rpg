#ifndef __AI_H__
#define __AI_H__

#include "collision.h"
#include <vector>

class Enemy;
// 敌人导航: 直线追踪，受阻时使用 A* 绕墙。
namespace FishAI
{
void navigate(Enemy &self, int playerX, int playerY, bool inAttackRange,
			  const std::vector<Obstacle> &obstacles, const bool walkGrid[GRID_H][GRID_W],
			  const int navigationField[GRID_H][GRID_W], const std::vector<Enemy *> &allies);

} // namespace FishAI
#endif
