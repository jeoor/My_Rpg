#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "scene.h"
#include "game_state.h"
#include <memory>

class SceneManager
{
public:
	void run();
	void setScore(int score);
	int getScore() const;

private:
	std::unique_ptr<Scene> createScene(GameState state);
	void transitionTo(GameState state);

	std::unique_ptr<Scene> currentScene;
	GameState currentState = GameState::Running;
	int score = 0;
	bool running = true;
};

#endif
