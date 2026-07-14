#ifndef __SCENE_H__
#define __SCENE_H__

#include "game_state.h"
class SceneManager;

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void onEnter() = 0;
	virtual void onExit() = 0;
	virtual GameState onFrame(SceneManager &sm) = 0;
};

#endif
