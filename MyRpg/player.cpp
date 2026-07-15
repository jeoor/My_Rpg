#include "player.h"
#include "camera.h"
#include "message.h"
#include "asset.h"
#include "collision.h"
Player::Player(int x, int y)
{
	auto &am = AssetManager::getInstance();
	Animation idleAnimation(am.getFrames(L"player_idle"), 4, 5, 15);
	Animation runAnimation(am.getFrames(L"player_run"), 8, 8, 14);
	Animation attackAnimation(am.getFrames(L"player_attack"), 7, 8, 21);

	animations[0] = idleAnimation;
	animations[1] = runAnimation;
	animations[2] = attackAnimation;
	set(x, y, animations, 3);
	setMaxSpeed(PLAYER_MAX_SPEED);
	setAcceleration(PLAYER_ACCELERATION);
	setHeight(9);
	setAttackOffset(10);
	setAttackRange(6);
	setHp(200);
}
void Player::updateState()
{
	// 攻击状态优先
	if (isAttacking())
	{
		// 只在刚进入攻击状态时切换动画并重置帧
		if (getCurrentAnimation() != 2)
			attack();
		// 播放攻击动画
		if (animations[getCurrentAnimation()].haveDone())
			setAttacking(false);
		return;
	}

	// 移动状态
	if (isMoving())
	{
		if (getCurrentAnimation() != 1)
			run();
		move();
	}
	else
	{
		if (getCurrentAnimation() != 0)
			idle();
	}

	if (pathActive)
		updatePath();
}
void Player::getMessage()
{
	// 一帧内处理所有消息
	while (Message::getInstance().poll())
	{
		ExMessage &msg = Message::getInstance().getMsg();
		if (msg.message == WM_KEYDOWN)
		{
			switch (msg.vkcode)
			{
			case VK_W:
			case VK_UP:
				clearPath();
				setUp(true);
				break;
			case VK_A:
			case VK_LEFT:
				clearPath();
				setLeft(true);
				break;
			case VK_S:
			case VK_DOWN:
				clearPath();
				setDown(true);
				break;
			case VK_D:
			case VK_RIGHT:
				clearPath();
				setRight(true);
				break;
			case VK_J:
				setAttacking(true);
				break;
			case VK_F1:
				changeDebug();
				break;
			default:
				break;
			}
		}
		if (msg.message == WM_KEYUP)
		{
			switch (msg.vkcode)
			{
			case VK_W:
			case VK_UP:
				setUp(false);
				break;
			case VK_A:
			case VK_LEFT:
				setLeft(false);
				break;
			case VK_S:
			case VK_DOWN:
				setDown(false);
				break;
			case VK_D:
			case VK_RIGHT:
				setRight(false);
				break;
			default:
				break;
			}
		}
		if (msg.message == WM_LBUTTONUP)
		{
			setPathTarget(msg.x + Camera::getInstance().getOffsetX(),
						  msg.y + Camera::getInstance().getOffsetY());
		}
		if (msg.message == WM_RBUTTONDOWN)
		{
			int x = msg.x + Camera::getInstance().getOffsetX();
			// 如果鼠标点击位置在角色左侧，设置方向为左
			if (x <= getX())
			{
				if (getDir() == dir::right)
				{
					changeFlip();
					setDir(dir::left);
				}
			}
			// 如果鼠标点击位置在角色右侧，设置方向为右
			else if (x > getX())
			{
				if (getDir() == dir::left)
				{
					changeFlip();
					setDir(dir::right);
				}
			}
			setAttacking(true);
		}
	}
}
void Player::idle() { setCurrentAnimation(0); }
void Player::run() { setCurrentAnimation(1); }
void Player::attack() { setCurrentAnimation(2); }
void Player::setAttacking(bool isAtk)
{
	attacking = isAtk;
	if (isAtk)
	{
		setCurrentAnimation(2);
		animations[2].setCurrentFrame(0);
	}
}
bool Player::isAttacking() const { return attacking; }
void Player::changeDebug() { debug = !debug; }
bool Player::isDebug() const { return debug; }
bool Player::havePath() const { return pathActive; }
int Player::getDestinationX() const { return destinationX; }
int Player::getDestinationY() const { return destinationY; }
void Player::setPathTarget(int x, int y)
{
	// 点击寻路接管移动，避免键盘方向与路径移动同时修改位置。
	stopMove();
	destinationX = x;
	destinationY = y;
	pathActive = true;

	bool grid[GRID_H][GRID_W];
	buildWalkGrid(grid, gObstacles);
	path = findPath((int)getX(), (int)getY(), x, y, grid);
	if (!path.empty() && !collideObstacle(x, y, getCollision(), gObstacles) &&
		!lineBlocked(path.back().first, path.back().second, x, y, gObstacles))
		path.push_back({x, y});
	pathIndex = 0;
	updatePath();
}
void Player::updatePath()
{
	while (pathIndex < path.size())
	{
		double dx = path[pathIndex].first - getX();
		double dy = path[pathIndex].second - getY();
		if (dx * dx + dy * dy > PATH_WAYPOINT_DISTANCE * PATH_WAYPOINT_DISTANCE)
			break;
		pathIndex++;
	}

	if (pathIndex >= path.size())
	{
		clearPath();
		return;
	}

	setTarget(path[pathIndex].first, path[pathIndex].second);
	move2(getTx(), getTy());
}
void Player::clearPath()
{
	path.clear();
	pathIndex = 0;
	pathActive = false;
	stopMove();
	setHaveTarget(false);
}
