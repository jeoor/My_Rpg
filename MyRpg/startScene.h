#ifndef __START_SCENE_H__
#define __START_SCENE_H__
#include "scene.h"
#include "player.h"
#include <memory>
class StartScene : public Scene
{
  public:
	void onEnter() override;
	void onExit() override;
	GameState onFrame(SceneManager &sm) override;

  private:
	std::unique_ptr<Player> player;
	int circleX = WINDOWS_W / 2, circleY = WINDOWS_H - 180; // 引导圈位置(屏幕空间)
};
#endif
