#include "scene_manager.h"
#include "start_scene.h"
#include "game_scene.h"
#include "end_scene.h"

std::unique_ptr<Scene> SceneManager::createScene(GameState state)
{
	switch (state)
	{
	case GameState::Start:    return std::make_unique<StartScene>();
	case GameState::Playing:  return std::make_unique<GameScene>();
	case GameState::GameOver: return std::make_unique<EndScene>(score);
	default:                  return nullptr;
	}
}

void SceneManager::transitionTo(GameState state)
{
	if (currentScene) currentScene->onExit();
	switch (state)
	{
	case GameState::Quit: running = false; currentScene = nullptr; return;
	default:
		currentScene = createScene(state);
		currentState = state;
		if (currentScene) currentScene->onEnter();
	}
}

void SceneManager::run()
{
	transitionTo(GameState::Playing);
	while (running && currentScene)
	{
		GameState next = currentScene->onFrame(*this);
		if (next != GameState::Running) { transitionTo(next); if (!currentScene) break; }
	}
}

void SceneManager::setScore(int s) { score = s; }
int SceneManager::getScore() const { return score; }
