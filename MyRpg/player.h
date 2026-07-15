#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "character.h"
#include <utility>
#include <vector>
class Player : public Character
{
  public:
	Player() = default;
	~Player() override = default;
	Player(int x, int y);

	void updateState() override;
	void getMessage();
	void idle();
	void attack();
	void run();
	void setAttacking(bool isAtk);
	bool isAttacking() const;
	void changeDebug();
	bool isDebug() const;
	bool havePath() const;
	int getDestinationX() const;
	int getDestinationY() const;

  private:
	void setPathTarget(int x, int y);
	void updatePath();
	void clearPath();

	Animation animations[3];
	std::vector<std::pair<int, int>> path;
	size_t pathIndex = 0;
	int destinationX = 0, destinationY = 0;
	bool pathActive = false;
	bool attacking = false; // 是否正在攻击
	bool debug = false;		// 显示调试信息
};
#endif
