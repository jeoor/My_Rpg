#ifndef __END_SCENE_H__
#define __END_SCENE_H__
#include "scene.h"
class EndScene : public Scene
{
  public:
	explicit EndScene(int score);
	void onEnter() override;
	void onExit() override;
	GameState onFrame(SceneManager &sm) override;

  private:
	int lastScore;
	int blinkTimer = 0;
	bool showPrompt = true;
};
#endif
