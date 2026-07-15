#include "startScene.h"
#include "sceneManager.h"
#include "camera.h"
#include "config.h"
#include "collision.h"
#include <easyx.h>
void StartScene::onEnter()
{
	// 相机: 世界=屏幕, 无滚动
	Camera::getInstance().init(WINDOWS_W, WINDOWS_H, WINDOWS_W, WINDOWS_H);
	// 开始场景不使用游戏地图障碍，避免点击寻路沿用上一局数据。
	gObstacles.clear();
	player = std::make_unique<Player>(WINDOWS_W / 2, WINDOWS_H / 2 - 75);
}
void StartScene::onExit() { player.reset(); }
GameState StartScene::onFrame(SceneManager & /*sm*/)
{
	// 玩家输入
	player->getMessage();

	// 检测是否走进引导圈
	int dx = (int)player->getX() - circleX;
	int dy = (int)player->getY() - circleY;
	if (dx * dx + dy * dy <= 80 * 80)
		return GameState::Playing;

	player->updateState();

	// === 渲染 ===
	cleardevice();
	setbkmode(TRANSPARENT);
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;

	// 标题
	f.lfHeight = 80;
	_tcscpy_s(f.lfFaceName, L"Silicon Carne");
	settextstyle(&f);
	settextcolor(WHITE);
	LPCTSTR t = L"MyRpg";
	outtextxy((WINDOWS_W - textwidth(t)) / 2, 30, t);

	// 引导圈 + "START" 文字
	setlinecolor(RGB(140, 110, 200));
	for (int r = START_CIRCLE_RADIUS + 5; r >= START_CIRCLE_RADIUS - 5; --r)
		circle(circleX, circleY, r);
	setlinecolor(RGB(180, 150, 240));
	circle(circleX, circleY, START_CIRCLE_RADIUS);

	f.lfHeight = START_TEXT_HEIGHT;
	settextstyle(&f);
	settextcolor(RGB(200, 170, 255));
	LPCTSTR st = L"START";
	outtextxy(circleX - textwidth(st) / 2, circleY - textheight(st) / 2, st);

	// 操作说明
	f.lfHeight = 18;
	settextstyle(&f);
	settextcolor(RGB(130, 130, 130));
	LPCTSTR c1 = L"WASD / Arrow Keys / Left Click — Move";
	LPCTSTR c2 = L"J / Right Click — Attack   F1 — Debug";
	outtextxy((WINDOWS_W - textwidth(c1)) / 2, WINDOWS_H - 55, c1);
	outtextxy((WINDOWS_W - textwidth(c2)) / 2, WINDOWS_H - 35, c2);

	if (player->havePath())
	{
		setfillcolor(RED);
		line((int)player->getX(), (int)player->getY() - player->getHeight() / 2, player->getDestinationX(),
			 player->getDestinationY());
		solidcircle(player->getDestinationX(), player->getDestinationY(), 5);
	}

	// 绘制玩家(屏幕空间, 无相机偏移)
	player->updateAnimation();

	return GameState::Running;
}
