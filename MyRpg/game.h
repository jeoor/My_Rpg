#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>
class Game
{
  public:
	static Game &getInstance();
	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;
	int run();

  private:
	Game();
	~Game();
	void loadResources();
};
#endif
