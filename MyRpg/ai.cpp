#include "ai.h"
#include "enemy.h"
#include "collision.h"
#include <algorithm>
#include <cmath>
namespace FishAI
{
static void moveWithSeparation(Enemy &self, int targetX, int targetY, const std::vector<Enemy *> &allies)
{
	double ex = self.getX(), ey = self.getY();
	double dx = targetX - ex, dy = targetY - ey;
	double length = std::sqrt(dx * dx + dy * dy);
	double moveX = 0, moveY = 0;
	if (length > 1.0)
	{
		moveX = dx / length * FISH_STEER_DISTANCE;
		moveY = dy / length * FISH_STEER_DISTANCE;
	}

	for (const auto *other : allies)
	{
		if (other == &self)
			continue;
		double awayX = ex - other->getX();
		double awayY = ey - other->getY();
		double distance = std::sqrt(awayX * awayX + awayY * awayY);
		if (distance < 1.0 || distance >= FISH_VISUAL_RANGE)
			continue;
		if (distance < FISH_SEPARATION_RANGE)
		{
			double force = (FISH_SEPARATION_RANGE - distance) / FISH_SEPARATION_RANGE;
			moveX += awayX / distance * FISH_STEER_DISTANCE * force;
			moveY += awayY / distance * FISH_STEER_DISTANCE * force;
		}
	}

	if (moveX != 0 || moveY != 0)
		self.move2((int)(ex + moveX), (int)(ey + moveY), false);
}
void navigate(Enemy &self, int playerX, int playerY, bool inAttackRange,
			  const std::vector<Obstacle> &obstacles, const bool walkGrid[GRID_H][GRID_W],
			  const int navigationField[GRID_H][GRID_W], const std::vector<Enemy *> &allies)
{
	int ex = (int)self.getX(), ey = (int)self.getY();
	if (inAttackRange)
	{
		moveWithSeparation(self, ex, ey, allies);
		return;
	}

	bool blocked = !obstacles.empty() && lineBlocked(ex, ey, playerX, playerY, obstacles);
	bool touchingWall = collideObstacle(ex, ey, (int)(self.getCollision() * 1.2), obstacles);
	if (!blocked && !touchingWall)
	{
		moveWithSeparation(self, playerX, playerY, allies);
		return;
	}

	int gx = std::clamp(ex / CELL_SIZE, 0, GRID_W - 1);
	int gy = std::clamp(ey / CELL_SIZE, 0, GRID_H - 1);
	int bestX = gx, bestY = gy;
	int bestCost = navigationField[gy][gx];
	const int dirs[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
	for (int i = 0; i < 8; ++i)
	{
		int nx = gx + dirs[i][0], ny = gy + dirs[i][1];
		if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H)
			continue;
		if (!walkGrid[ny][nx] || navigationField[ny][nx] >= bestCost)
			continue;
		if (i >= 4 && (!walkGrid[gy][nx] || !walkGrid[ny][gx]))
			continue;
		bestX = nx;
		bestY = ny;
		bestCost = navigationField[ny][nx];
	}

	// 当前格没有可下降邻居时，在两格内寻找可直达的较低导航格。
	if (bestX == gx && bestY == gy)
		for (int dy = -2; dy <= 2; ++dy)
			for (int dx = -2; dx <= 2; ++dx)
			{
				int nx = gx + dx, ny = gy + dy;
				if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H)
					continue;
				if (!walkGrid[ny][nx] || navigationField[ny][nx] >= bestCost)
					continue;
				int targetX = nx * CELL_SIZE + CELL_SIZE / 2;
				int targetY = ny * CELL_SIZE + CELL_SIZE / 2;
				if (lineBlocked(ex, ey, targetX, targetY, obstacles))
					continue;
				bestX = nx;
				bestY = ny;
				bestCost = navigationField[ny][nx];
			}

	if (bestX != gx || bestY != gy)
		moveWithSeparation(self, bestX * CELL_SIZE + CELL_SIZE / 2, bestY * CELL_SIZE + CELL_SIZE / 2,
						   allies);
	else
		self.stopMove();
}
} // namespace FishAI
