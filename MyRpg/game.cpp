#include "game.h"
#include "config.h"
#include "scene_manager.h"
#include <easyx.h>
#pragma comment(lib, "Winmm.lib")

static void initWindow() { initgraph(WINDOWS_W, WINDOWS_H); setbkcolor(RGB(40, 31, 48)); }

Game &Game::getInstance() { static Game instance; return instance; }
Game::Game()  { loadResources(); initWindow(); }
Game::~Game() { RemoveFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL); }

int Game::run()
{
	BeginBatchDraw();
	SceneManager sm; sm.run();
	EndBatchDraw();
	return 0;
}

void Game::loadResources()
{
	AddFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL);
	mciSendString(L"open source/mus/bgm.mp3 alias bgm", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/hit.wav alias hit", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/hurt.wav alias hurt", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/addHP.mp3 alias addHP", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/dead.mp3 alias dead", nullptr, 0, nullptr);
}
