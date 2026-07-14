#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "scene.h"
#include "player.h"
#include "enemy.h"
#include "timer.h"
#include <vector>
#include <memory>

class GameScene : public Scene
{
public:
	GameScene();
	void onEnter() override;
	void onExit() override;
	GameState onFrame(SceneManager &sm) override;

private:
	void spawnEnemy();
	void updateCombat();
	void removeDeadEnemies();
	void buildRenderOrder(std::vector<Character *> &out);
	void renderUI();
	void renderDebugInfo();
	void renderGameObjects();
	void renderStartZone();

	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Enemy>> enemyList;
	ExMessage msg;
	Timer frameTimer;
	int score = 0, oldScore = 0, spawnTimer = 0;
	bool gameStarted = false;
	int startX = WORLD_W / 2, startY = WORLD_H / 2 + 150;
};

#endif
