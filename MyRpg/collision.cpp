#define NOMINMAX
#include "collision.h"
#include <easyx.h>
#include <queue>
#include <cmath>
#include <algorithm>

// ==================== 障碍物 ====================
std::vector<Obstacle> gObstacles;
static bool hasReachableEdges(const bool reachable[GRID_H][GRID_W])
{
	int top = 0, bottom = 0, left = 0, right = 0;
	for (int x = 0; x < GRID_W; ++x)
	{
		top += reachable[0][x];
		bottom += reachable[GRID_H - 1][x];
	}
	for (int y = 0; y < GRID_H; ++y)
	{
		left += reachable[y][0];
		right += reachable[y][GRID_W - 1];
	}
	return top >= MIN_REACHABLE_EDGE_CELLS && bottom >= MIN_REACHABLE_EDGE_CELLS &&
		   left >= MIN_REACHABLE_EDGE_CELLS && right >= MIN_REACHABLE_EDGE_CELLS;
}
std::vector<Obstacle> generateObstacles(int count, int playerX, int playerY)
{
	gObstacles.clear();
	gObstacles.reserve(count);
	for (int i = 0; i < count; ++i)
	{
		int cx = rand() % (WORLD_W - 200) + 100;
		int cy = rand() % (WORLD_H - 200) + 100;

		// 玩家出生点附近保留足够大的可行走区域。
		if (abs(cx - playerX) < CELL_SIZE * 5 && abs(cy - playerY) < CELL_SIZE * 5)
		{
			--i;
			continue;
		}

		int w = 50 + rand() % 70, h = 50 + rand() % 70;
		gObstacles.push_back({cx - w / 2, cy - h / 2, w, h});
	}

	// 矩形允许重叠；碰撞检测本身就是它们的真实并集。
	// 若墙体封住边缘，撤销少量后放置的墙块以恢复连通性。
	while (true)
	{
		bool walkable[GRID_H][GRID_W] = {};
		bool reachable[GRID_H][GRID_W] = {};
		buildWalkGrid(walkable, gObstacles);
		buildReachableGrid(reachable, playerX, playerY, walkable);
		if (hasReachableEdges(reachable) || gObstacles.empty())
			break;
		gObstacles.pop_back();
	}
	return gObstacles;
}
bool collideCircleRect(int cx, int cy, int cr, const Obstacle &o)
{
	int nearX = cx < o.x ? o.x : (cx > o.x + o.w ? o.x + o.w : cx);
	int nearY = cy < o.y ? o.y : (cy > o.y + o.h ? o.y + o.h : cy);
	int dx = cx - nearX, dy = cy - nearY;
	return dx * dx + dy * dy < cr * cr;
}
bool collideObstacle(int cx, int cy, int cr, const std::vector<Obstacle> &obs)
{
	for (const auto &o : obs)
		if (collideCircleRect(cx, cy, cr, o))
			return true;
	return false;
}
bool lineBlocked(int sx, int sy, int tx, int ty, const std::vector<Obstacle> &obs)
{
	double dx = tx - sx, dy = ty - sy;
	for (const auto &o : obs)
	{
		double minX = o.x - NAV_CLEARANCE, maxX = o.x + o.w + NAV_CLEARANCE;
		double minY = o.y - NAV_CLEARANCE, maxY = o.y + o.h + NAV_CLEARANCE;
		double enter = 0.0, leave = 1.0;

		auto clip = [&enter, &leave](double start, double delta, double minValue, double maxValue)
		{
			if (abs(delta) < 1e-9)
				return start >= minValue && start <= maxValue;
			double t1 = (minValue - start) / delta;
			double t2 = (maxValue - start) / delta;
			if (t1 > t2)
				std::swap(t1, t2);
			enter = std::max(enter, t1);
			leave = std::min(leave, t2);
			return enter <= leave;
		};

		if (clip(sx, dx, minX, maxX) && clip(sy, dy, minY, maxY))
			return true;
	}
	return false;
}
void drawObstacles(const std::vector<Obstacle> &obs)
{
	for (const auto &o : obs)
	{
		setfillcolor(RGB(65, 55, 80));
		solidrectangle(o.x, o.y, o.x + o.w, o.y + o.h);
	}

	setlinecolor(RGB(90, 75, 110));
	for (const auto &o : obs)
	{
		std::vector<int> vertical = {o.y, o.y + o.h};
		std::vector<int> horizontal = {o.x, o.x + o.w};
		for (const auto &other : obs)
		{
			if (other.y > o.y && other.y < o.y + o.h)
				vertical.push_back(other.y);
			if (other.y + other.h > o.y && other.y + other.h < o.y + o.h)
				vertical.push_back(other.y + other.h);
			if (other.x > o.x && other.x < o.x + o.w)
				horizontal.push_back(other.x);
			if (other.x + other.w > o.x && other.x + other.w < o.x + o.w)
				horizontal.push_back(other.x + other.w);
		}
		std::sort(vertical.begin(), vertical.end());
		vertical.erase(std::unique(vertical.begin(), vertical.end()), vertical.end());
		std::sort(horizontal.begin(), horizontal.end());
		horizontal.erase(std::unique(horizontal.begin(), horizontal.end()), horizontal.end());

		for (int edgeX : {o.x, o.x + o.w})
			for (size_t i = 0; i + 1 < vertical.size(); ++i)
			{
				int y1 = vertical[i], y2 = vertical[i + 1];
				int midY = (y1 + y2) / 2;
				bool left = collideObstacle(edgeX - 1, midY, 1, obs);
				bool right = collideObstacle(edgeX + 1, midY, 1, obs);
				if (left != right)
					line(edgeX, y1, edgeX, y2);
			}

		for (int edgeY : {o.y, o.y + o.h})
			for (size_t i = 0; i + 1 < horizontal.size(); ++i)
			{
				int x1 = horizontal[i], x2 = horizontal[i + 1];
				int midX = (x1 + x2) / 2;
				bool top = collideObstacle(midX, edgeY - 1, 1, obs);
				bool bottom = collideObstacle(midX, edgeY + 1, 1, obs);
				if (top != bottom)
					line(x1, edgeY, x2, edgeY);
			}
	}
}
// ==================== A* 寻路 ====================
void buildWalkGrid(bool grid[GRID_H][GRID_W], const std::vector<Obstacle> &obs)
{
	for (int y = 0; y < GRID_H; ++y)
		for (int x = 0; x < GRID_W; ++x)
		{
			int cx = x * CELL_SIZE + CELL_SIZE / 2;
			int cy = y * CELL_SIZE + CELL_SIZE / 2;
			grid[y][x] = !collideObstacle(cx, cy, NAV_CLEARANCE, obs);
		}
}
void buildReachableGrid(bool reachable[GRID_H][GRID_W], int startX, int startY,
						const bool grid[GRID_H][GRID_W])
{
	for (int y = 0; y < GRID_H; ++y)
		for (int x = 0; x < GRID_W; ++x)
			reachable[y][x] = false;

	int sx = std::clamp(startX / CELL_SIZE, 0, GRID_W - 1);
	int sy = std::clamp(startY / CELL_SIZE, 0, GRID_H - 1);
	if (!grid[sy][sx])
		return;

	std::queue<std::pair<int, int>> open;
	open.push({sx, sy});
	reachable[sy][sx] = true;
	const int dirs[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
	while (!open.empty())
	{
		auto [cx, cy] = open.front();
		open.pop();
		for (int i = 0; i < 8; ++i)
		{
			int nx = cx + dirs[i][0], ny = cy + dirs[i][1];
			if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H)
				continue;
			if (reachable[ny][nx] || !grid[ny][nx])
				continue;
			if (i >= 4 && (!grid[cy][nx] || !grid[ny][cx]))
				continue;
			reachable[ny][nx] = true;
			open.push({nx, ny});
		}
	}
}
void buildNavigationField(int field[GRID_H][GRID_W], int targetX, int targetY,
						  const bool grid[GRID_H][GRID_W])
{
	for (int y = 0; y < GRID_H; ++y)
		for (int x = 0; x < GRID_W; ++x)
			field[y][x] = 99999;

	int tx = std::clamp(targetX / CELL_SIZE, 0, GRID_W - 1);
	int ty = std::clamp(targetY / CELL_SIZE, 0, GRID_H - 1);
	if (!grid[ty][tx])
		return;

	std::queue<std::pair<int, int>> open;
	open.push({tx, ty});
	field[ty][tx] = 0;
	const int dirs[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
	while (!open.empty())
	{
		auto [cx, cy] = open.front();
		open.pop();
		for (int i = 0; i < 8; ++i)
		{
			int nx = cx + dirs[i][0], ny = cy + dirs[i][1];
			if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H)
				continue;
			if (!grid[ny][nx] || field[ny][nx] != 99999)
				continue;
			if (i >= 4 && (!grid[cy][nx] || !grid[ny][cx]))
				continue;
			field[ny][nx] = field[cy][cx] + 1;
			open.push({nx, ny});
		}
	}
}
static int heuristic(int gx, int gy, int tgx, int tgy)
{
	int dx = abs(gx - tgx), dy = abs(gy - tgy);
	return 10 * std::max(dx, dy) + 4 * std::min(dx, dy);
}
static bool gridLineClear(int x1, int y1, int x2, int y2, const bool grid[GRID_H][GRID_W])
{
	int dx = abs(x2 - x1), dy = abs(y2 - y1);
	int stepX = x1 < x2 ? 1 : -1;
	int stepY = y1 < y2 ? 1 : -1;
	int error = dx - dy;

	while (true)
	{
		if (x1 < 0 || x1 >= GRID_W || y1 < 0 || y1 >= GRID_H || !grid[y1][x1])
			return false;
		if (x1 == x2 && y1 == y2)
			return true;

		int oldX = x1, oldY = y1;
		int twiceError = error * 2;
		if (twiceError > -dy)
		{
			error -= dy;
			x1 += stepX;
		}
		if (twiceError < dx)
		{
			error += dx;
			y1 += stepY;
		}
		if (x1 != oldX && y1 != oldY && (!grid[oldY][x1] || !grid[y1][oldX]))
			return false;
	}
}
std::vector<std::pair<int, int>> findPath(int sx, int sy, int tx, int ty, const bool grid[GRID_H][GRID_W])
{
	int sgx = std::clamp(sx / CELL_SIZE, 0, GRID_W - 1);
	int sgy = std::clamp(sy / CELL_SIZE, 0, GRID_H - 1);
	int tgx = std::clamp(tx / CELL_SIZE, 0, GRID_W - 1);
	int tgy = std::clamp(ty / CELL_SIZE, 0, GRID_H - 1);

	// 终点被挡 → 找最近可行走邻居
	if (!grid[tgy][tgx])
	{
		int bestD = 9999, bx = tgx, by = tgy;
		for (int dy = -2; dy <= 2; ++dy)
			for (int dx = -2; dx <= 2; ++dx)
			{
				int nx = tgx + dx, ny = tgy + dy;
				if (nx >= 0 && nx < GRID_W && ny >= 0 && ny < GRID_H && grid[ny][nx])
				{
					int d = abs(dx) + abs(dy);
					if (d < bestD)
					{
						bestD = d;
						bx = nx;
						by = ny;
					}
				}
			}
		tgx = bx;
		tgy = by;
		if (!grid[tgy][tgx])
			return {{tx, ty}};
	}
	struct Node
	{
		int gx, gy, g, f;
	};
	bool closed[GRID_H][GRID_W] = {};
	int gScore[GRID_H][GRID_W];
	std::pair<int, int> parent[GRID_H][GRID_W];
	for (int y = 0; y < GRID_H; ++y)
		for (int x = 0; x < GRID_W; ++x)
		{
			gScore[y][x] = 999999;
			parent[y][x] = {-1, -1};
		}

	auto cmp = [](const Node &a, const Node &b) { return a.f > b.f; };
	std::priority_queue<Node, std::vector<Node>, decltype(cmp)> open(cmp);
	gScore[sgy][sgx] = 0;
	open.push({sgx, sgy, 0, heuristic(sgx, sgy, tgx, tgy)});

	const int dirs[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
	const int costs[8] = {10, 10, 10, 10, 14, 14, 14, 14};

	bool found = false;
	while (!open.empty())
	{
		Node cur = open.top();
		open.pop();
		if (closed[cur.gy][cur.gx] || cur.g != gScore[cur.gy][cur.gx])
			continue;
		if (cur.gx == tgx && cur.gy == tgy)
		{
			found = true;
			break;
		}
		closed[cur.gy][cur.gx] = true;
		for (int d = 0; d < 8; ++d)
		{
			int nx = cur.gx + dirs[d][0], ny = cur.gy + dirs[d][1];
			if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H)
				continue;
			if (closed[ny][nx] || !grid[ny][nx])
				continue;
			// 防穿角
			if (d >= 4)
			{
				int ax = cur.gx + dirs[d][0], ay = cur.gy;
				int bx = cur.gx, by = cur.gy + dirs[d][1];
				if (ax < 0 || ax >= GRID_W || ay < 0 || ay >= GRID_H)
					continue;
				if (bx < 0 || bx >= GRID_W || by < 0 || by >= GRID_H)
					continue;
				if (!grid[ay][ax] || !grid[by][bx])
					continue;
			}
			int ng = cur.g + costs[d];
			if (ng >= gScore[ny][nx])
				continue;
			gScore[ny][nx] = ng;
			parent[ny][nx] = {cur.gx, cur.gy};
			open.push({nx, ny, ng, ng + heuristic(nx, ny, tgx, tgy)});
		}
	}

	// 回溯 + 路径平滑
	std::vector<std::pair<int, int>> raw;
	if (!found)
	{
		return {{tx, ty}};
	}
	for (int x = tgx, y = tgy; x >= 0 && y >= 0;)
	{
		raw.push_back({x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2});
		auto p = parent[y][x];
		x = p.first;
		y = p.second;
	}
	std::reverse(raw.begin(), raw.end());

	// 路径平滑: 仅跳过网格直线可达的路点，保留绕障拐角。
	std::vector<std::pair<int, int>> smooth;
	smooth.push_back(raw[0]);
	for (size_t current = 0; current + 1 < raw.size();)
	{
		size_t next = current + 1;
		for (size_t candidate = raw.size() - 1; candidate > current; --candidate)
		{
			int x1 = raw[current].first / CELL_SIZE;
			int y1 = raw[current].second / CELL_SIZE;
			int x2 = raw[candidate].first / CELL_SIZE;
			int y2 = raw[candidate].second / CELL_SIZE;
			if (gridLineClear(x1, y1, x2, y2, grid))
			{
				next = candidate;
				break;
			}
		}
		smooth.push_back(raw[next]);
		current = next;
	}
	return smooth;
}
