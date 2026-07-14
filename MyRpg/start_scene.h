#ifndef __START_SCENE_H__
#define __START_SCENE_H__
#include "scene.h"

class StartScene : public Scene
{
public:
	void onEnter() override;
	void onExit() override;
	GameState onFrame(SceneManager &sm) override;
private:
	int blinkTimer = 0;
	bool showPrompt = true;
};

#endif
