#include "game.h"
#include "config.h"
#include "sceneManager.h"
#include "asset.h"
#include <easyx.h>
#pragma comment(lib, "Winmm.lib")
static void initWindow()
{
	initgraph(WINDOWS_W, WINDOWS_H);
	setbkcolor(RGB(40, 31, 48));
}
class BatchDrawGuard
{
  public:
	BatchDrawGuard() { BeginBatchDraw(); }
	~BatchDrawGuard() { EndBatchDraw(); }
	BatchDrawGuard(const BatchDrawGuard &) = delete;
	BatchDrawGuard &operator=(const BatchDrawGuard &) = delete;
};
Game &Game::getInstance()
{
	static Game instance;
	return instance;
}
Game::Game()
{
	loadResources();
	initWindow();
}
Game::~Game()
{
	mciSendString(L"close bgm", nullptr, 0, nullptr);
	mciSendString(L"close hit", nullptr, 0, nullptr);
	mciSendString(L"close hurt", nullptr, 0, nullptr);
	mciSendString(L"close addHP", nullptr, 0, nullptr);
	mciSendString(L"close dead", nullptr, 0, nullptr);
	RemoveFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL);
	closegraph();
}
int Game::run()
{
	BatchDrawGuard batchDraw;
	SceneManager sm;
	sm.run();
	return 0;
}
void Game::loadResources()
{
	AssetManager::getInstance().init(); // 预加载全部 Frame
	AddFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL);
	mciSendString(L"open source/mus/bgm.mp3 alias bgm", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/hit.wav alias hit", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/hurt.wav alias hurt", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/addHP.mp3 alias addHP", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/dead.mp3 alias dead", nullptr, 0, nullptr);
}
