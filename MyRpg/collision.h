#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "config.h"
#include <vector>
// ===== 障碍物 =====
struct Obstacle
{
	int x, y, w, h;
};
extern std::vector<Obstacle> gObstacles;

// 随机障碍生成；矩形可重叠，碰撞与渲染按并集处理
std::vector<Obstacle> generateObstacles(int count, int playerX, int playerY);
// 圆与矩形碰撞 (最近点法)
bool collideCircleRect(int cx, int cy, int cr, const Obstacle &o);
// 遍历检测
bool collideObstacle(int cx, int cy, int cr, const std::vector<Obstacle> &obs);
// 射线检测: 从(sx,sy)到(tx,ty)是否被障碍挡住
bool lineBlocked(int sx, int sy, int tx, int ty, const std::vector<Obstacle> &obs);
void drawObstacles(const std::vector<Obstacle> &obs);

// ===== A* 寻路 =====
constexpr int CELL_SIZE = 50;
constexpr int GRID_W = WORLD_W / CELL_SIZE;
constexpr int GRID_H = WORLD_H / CELL_SIZE;

// 根据障碍构建可行走网格
void buildWalkGrid(bool grid[GRID_H][GRID_W], const std::vector<Obstacle> &obs);
void buildReachableGrid(bool reachable[GRID_H][GRID_W], int startX, int startY,
						const bool grid[GRID_H][GRID_W]);
void buildNavigationField(int field[GRID_H][GRID_W], int targetX, int targetY,
						  const bool grid[GRID_H][GRID_W]);

// A*: 返回世界坐标路点 (已做路径平滑)
std::vector<std::pair<int, int>> findPath(int sx, int sy, int tx, int ty, const bool grid[GRID_H][GRID_W]);

#endif
