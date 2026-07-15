#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "scene.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
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
	void updateNavigationField();
	void buildRenderOrder();
	void renderUI(int fps);
	void renderDebugInfo();
	void renderGameObjects();

	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Enemy>> enemyList;
	std::vector<Enemy *> enemyPointers;
	std::vector<Character *> renderOrder;
	bool walkGrid[GRID_H][GRID_W] = {};
	bool reachableGrid[GRID_H][GRID_W] = {};
	int navigationField[GRID_H][GRID_W] = {};
	int navigationTargetX = -1, navigationTargetY = -1;
	int score = 0, oldScore = 0, spawnTimer = 0;
	bool paused = false;
	bool escWasDown = false;
};
#endif
