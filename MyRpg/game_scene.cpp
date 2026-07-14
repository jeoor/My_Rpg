#include "game_scene.h"
#include "end_scene.h"
#include "scene_manager.h"
#include "camera.h"
#include "config.h"
#include <easyx.h>
#include <cstdio>
#include <cmath>
#include <algorithm>
#pragma comment(lib, "Winmm.lib")

GameScene::GameScene() { msg = {}; }

void GameScene::onEnter()
{
	Camera::getInstance().init(WORLD_W, WORLD_H, WINDOWS_W, WINDOWS_H);
	player = std::make_unique<Player>(startX, startY + 150);
	Camera::getInstance().setTarget(player->getX(), player->getY());
	Camera::getInstance().update();
	enemyList.clear();
	score = 0; oldScore = 0; spawnTimer = 0;
	gameStarted = false;
	frameTimer.reset();
	mciSendString(L"play bgm repeat from 0", nullptr, 0, nullptr);
}

void GameScene::onExit() {}

GameState GameScene::onFrame(SceneManager &sm)
{
	frameTimer.start();

	// 1. 玩家输入
	player->getMessage(&msg);

	// 2. 检测是否进入开始区域
	if (!gameStarted)
	{
		int dx = (int)player->getX() - startX;
		int dy = (int)player->getY() - startY;
		if (dx * dx + dy * dy <= 80 * 80)
			gameStarted = true;
	}

	// 3. 死亡检测
	if (!player->isAlive())
	{
		mciSendString(L"play dead from 0", nullptr, 0, nullptr);
		sm.setScore(score);
		return GameState::GameOver;
	}

	oldScore = score;

	// 4. 敌人生成(仅在游戏开始后)
	if (gameStarted)
	{
		spawnTimer += DELTA;
		if (spawnTimer >= 5 * DELTA_TIME && enemyList.size() < MAX_ENEMIES)
			{ spawnTimer = 0; spawnEnemy(); }
	}

	// 5. 更新状态
	player->updateState();
	Camera::getInstance().setTarget(player->getX(), player->getY());
	Camera::getInstance().update();

	for (auto &enemy : enemyList)
	{
		enemy->move2((int)player->getX(), (int)player->getY());
		int dx = enemy->getAttackX() - player->getCenterX();
		int dy = enemy->getAttackY() - player->getCenterY();
		int r = enemy->getAttackRange() + player->getCollision();
		enemy->setAttacking(dx * dx + dy * dy <= r * r);
		enemy->updateState();
	}

	// 6. 战斗
	if (gameStarted)
		updateCombat();

	// 7. 清理死敌
	removeDeadEnemies();

	// 8. Y排序渲染
	std::vector<Character *> order;
	buildRenderOrder(order);

	cleardevice();
	renderGameObjects();
	renderUI();
	if (!gameStarted) renderStartZone();
	if (player->Debug()) renderDebugInfo();
	FlushBatchDraw();

	frameTimer.stop();
	double ms = frameTimer.elapsed() * 1000.0;
	if (ms < FRAME_BREAK) Sleep((DWORD)(FRAME_BREAK - ms));

	return GameState::Running;
}

void GameScene::spawnEnemy()
{
	int side = rand() % 4, x = 0, y = 0;
	switch (side) {
	case 0: y = -1; x = rand() % WORLD_W; break;
	case 1: y = WORLD_H; x = rand() % WORLD_W; break;
	case 2: x = -1; y = rand() % WORLD_H; break;
	case 3: x = WORLD_W; y = rand() % WORLD_H; break;
	}
	enemyList.emplace_back(std::make_unique<Enemy>(x, y));
}

void GameScene::updateCombat()
{
	for (auto &enemy : enemyList)
	{
		int dx = enemy->getCenterX() - player->getAttackX();
		int dy = enemy->getCenterY() - player->getAttackY();
		int r = enemy->getCollision() + player->getAttackRange();
		if (dx * dx + dy * dy <= r * r && player->isAttacking() && enemy->canHurt())
			{ mciSendString(L"play hit from 0", nullptr, 0, nullptr); enemy->Hurt(); }

		dx = enemy->getAttackX() - player->getCenterX();
		dy = enemy->getAttackY() - player->getCenterY();
		r = enemy->getAttackRange() + player->getCollision();
		if (dx * dx + dy * dy <= r * r && enemy->isAttacking() && player->canHurt())
			{ mciSendString(L"play hurt from 0", nullptr, 0, nullptr); player->Hurt(); }
	}
}

void GameScene::removeDeadEnemies()
{
	for (size_t i = 0; i < enemyList.size(); ++i)
	{
		if (!enemyList[i]->isAlive())
		{
			std::swap(enemyList[i], enemyList.back());
			enemyList.pop_back();
			score++;
			if (!(score % HP_BONUS_INTERVAL) && oldScore != score)
				{ mciSendString(L"play addHP from 0", nullptr, 0, nullptr); player->setCanAddHP(true); player->addHP(HP_BONUS_AMOUNT); }
			--i;
		}
	}
}

void GameScene::buildRenderOrder(std::vector<Character *> &out)
{
	out.clear(); out.reserve(enemyList.size() + 1);
	out.push_back(player.get());
	for (auto &e : enemyList) out.push_back(e.get());
	std::sort(out.begin(), out.end(), [](Character *a, Character *b) { return a->getY() < b->getY(); });
}

void GameScene::renderGameObjects()
{
	std::vector<Character *> order; buildRenderOrder(order);
	HDC hdc = GetImageHDC(nullptr); POINT oldOrigin;
	SetViewportOrgEx(hdc, -Camera::getInstance().getOffsetX(), -Camera::getInstance().getOffsetY(), &oldOrigin);

	LOGFONT f; gettextstyle(&f); f.lfHeight = 100;
	_tcscpy_s(f.lfFaceName, L"Silicon Carne"); f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f); settextcolor(RGB(60, 50, 70));
	LPCTSTR bg = L"MyRpg";
	outtextxy(WORLD_W / 2 - textwidth(bg) / 2, WORLD_H / 2 - textheight(bg) / 2, bg);

	if (player->haveT()) {
		setfillcolor(RED);
		line((int)player->getX(), (int)player->getY() - player->getHeight() / 2, player->getTx(), player->getTy());
		solidcircle(player->getTx(), player->getTy(), 5);
	}
	for (auto *c : order) c->updateAnimation();
	SetViewportOrgEx(hdc, oldOrigin.x, oldOrigin.y, nullptr);
}

void GameScene::renderUI()
{
	setbkmode(TRANSPARENT);
	wchar_t buf[32]; swprintf_s(buf, L"SCORE: %d", score);
	LOGFONT f; gettextstyle(&f); f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = (!(score % HP_BONUS_INTERVAL) && score) ? 25 : 20;
	_tcscpy_s(f.lfFaceName, L"Silicon Carne"); settextstyle(&f);
	settextcolor((!(score % HP_BONUS_INTERVAL) && score) ? RED : WHITE);
	outtextxy(0, 0, buf);

	SYSTEMTIME t; GetLocalTime(&t);
	swprintf_s(buf, L"%02d:%02d:%02d", t.wHour, t.wMinute, t.wSecond);
	f.lfHeight = 20; settextstyle(&f); settextcolor(WHITE);
	outtextxy(WINDOWS_W - textwidth(buf) - 10, 0, buf);
}

void GameScene::renderDebugInfo()
{
	HDC hdc = GetImageHDC(nullptr); POINT oldOrigin;
	SetViewportOrgEx(hdc, -Camera::getInstance().getOffsetX(), -Camera::getInstance().getOffsetY(), &oldOrigin);
	for (auto &enemy : enemyList) {
		setlinecolor(RGB(255, 100, 100));
		line(player->getCenterX(), player->getCenterY(), enemy->getCenterX(), enemy->getCenterY());
		circle(enemy->getCenterX(), enemy->getCenterY(), enemy->getCollision());
		circle(enemy->getAttackX(), enemy->getAttackY(), enemy->getAttackRange());
	}
	setlinecolor(RGB(100, 255, 100));
	circle(player->getCenterX(), player->getCenterY(), player->getCollision());
	circle(player->getAttackX(), player->getAttackY(), player->getAttackRange());
	setlinecolor(RGB(100, 100, 255)); rectangle(0, 0, WORLD_W, WORLD_H);
	SetViewportOrgEx(hdc, oldOrigin.x, oldOrigin.y, nullptr);
}

void GameScene::renderStartZone()
{
	HDC hdc = GetImageHDC(nullptr); POINT oldOrigin;
	SetViewportOrgEx(hdc, -Camera::getInstance().getOffsetX(), -Camera::getInstance().getOffsetY(), &oldOrigin);

	// 发光圆圈
	for (int r = 90; r >= 60; r -= 10)
	{
		setlinecolor(RGB(255, 215, 0));
		circle(startX, startY, r);
	}

	// "START" 文字
	LOGFONT f; gettextstyle(&f);
	f.lfHeight = 60; _tcscpy_s(f.lfFaceName, L"Silicon Carne"); f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f); settextcolor(RGB(255, 215, 0));
	setbkmode(TRANSPARENT);
	LPCTSTR t = L"START";
	int tw = textwidth(t), th = textheight(t);
	outtextxy(startX - tw / 2, startY - th / 2, t);

	// 引导提示(屏幕空间)
	SetViewportOrgEx(hdc, oldOrigin.x, oldOrigin.y, nullptr);
	f.lfHeight = 20; settextstyle(&f); settextcolor(RGB(200, 200, 200));
	LPCTSTR hint = L"Walk into the START zone to begin";
	outtextxy((WINDOWS_W - textwidth(hint)) / 2, WINDOWS_H - 30, hint);
}
