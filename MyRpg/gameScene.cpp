#include "gameScene.h"
#include "endScene.h"
#include "sceneManager.h"
#include "camera.h"
#include "config.h"
#include "collision.h"
#include "ai.h"
#include "enemy.h"
#include "asset.h"
#include <easyx.h>
#include <cstdio>
#include <cmath>
#include <algorithm>
#pragma comment(lib, "Winmm.lib")
GameScene::GameScene() {}
void GameScene::onEnter()
{
	Camera::getInstance().init(WORLD_W, WORLD_H, WINDOWS_W, WINDOWS_H);
	player = std::make_unique<Player>(WORLD_W / 2, WORLD_H / 2 + 150);
	gObstacles.clear();
	generateObstacles(60, (int)player->getX(), (int)player->getY());
	buildWalkGrid(walkGrid, gObstacles);
	buildReachableGrid(reachableGrid, (int)player->getX(), (int)player->getY(), walkGrid);
	updateNavigationField();
	Camera::getInstance().setTarget(player->getX(), player->getY());
	Camera::getInstance().update();
	enemyList.clear();
	enemyPointers.clear();
	enemyPointers.reserve(MAX_ENEMIES);
	renderOrder.reserve(MAX_ENEMIES + 1);
	score = 0;
	oldScore = 0;
	spawnTimer = 0;
	escWasDown = false;
	mciSendString(L"close bgm", nullptr, 0, nullptr);
	mciSendString(L"play bgm repeat from 0", nullptr, 0, nullptr);
}
void GameScene::onExit() {}
GameState GameScene::onFrame(SceneManager &sm)
{
	// 检查 ESC 暂停(用 GetAsyncKeyState 避免吃掉消息)
	bool escDown = GetAsyncKeyState(0x1B) & 0x8000;
	if (escDown && !escWasDown)
		paused = !paused;
	escWasDown = escDown;

	// 暂停状态 — 只渲染，不更新
	if (paused)
	{
		cleardevice();
		setbkmode(TRANSPARENT);
		LOGFONT f;
		gettextstyle(&f);
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfHeight = 80;
		_tcscpy_s(f.lfFaceName, L"Silicon Carne");
		settextstyle(&f);
		settextcolor(RGB(200, 180, 255));
		LPCTSTR t = L"PAUSED";
		outtextxy((WINDOWS_W - textwidth(t)) / 2, (WINDOWS_H - textheight(t)) / 2, t);
		return GameState::Running;
	}

	// 1. 玩家输入
	player->getMessage();

	// 2. 死亡检测
	if (!player->isAlive())
	{
		mciSendString(L"play dead from 0", nullptr, 0, nullptr);
		sm.setScore(score);
		return GameState::GameOver;
	}

	oldScore = score;

	// 4. 敌人生成
	spawnTimer += DELTA;
	if (spawnTimer >= SPAWN_INTERVAL * DELTA_TIME && enemyList.size() < MAX_ENEMIES)
	{
		spawnTimer = 0;
		spawnEnemy();
	}

	// 5. 更新状态
	player->updateState();
	updateNavigationField();
	Camera::getInstance().setTarget(player->getX(), player->getY());
	Camera::getInstance().update();

	enemyPointers.clear();
	for (auto &enemy : enemyList)
		enemyPointers.push_back(enemy.get());
	for (auto &enemy : enemyList)
	{
		int dx = enemy->getAttackX() - player->getCenterX();
		int dy = enemy->getAttackY() - player->getCenterY();
		int r = enemy->getAttackRange() + player->getCollision();
		bool inRange = dx * dx + dy * dy <= r * r;
		FishAI::navigate(*enemy, (int)player->getX(), (int)player->getY(), inRange, gObstacles, walkGrid,
						 navigationField, enemyPointers);
		enemy->setAttacking(inRange);
		enemy->updateState();
	}

	// 6. 战斗
	updateCombat();

	// 7. 清理死敌
	removeDeadEnemies();

	// 8. 渲染
	cleardevice();
	renderGameObjects();
	renderUI(sm.getFps());
	if (player->isDebug())
		renderDebugInfo();

	return GameState::Running;
}
void GameScene::updateNavigationField()
{
	int gridX = std::clamp((int)player->getX() / CELL_SIZE, 0, GRID_W - 1);
	int gridY = std::clamp((int)player->getY() / CELL_SIZE, 0, GRID_H - 1);
	if (gridX == navigationTargetX && gridY == navigationTargetY)
		return;
	buildNavigationField(navigationField, (int)player->getX(), (int)player->getY(), walkGrid);
	navigationTargetX = gridX;
	navigationTargetY = gridY;
}
void GameScene::spawnEnemy()
{
	std::vector<std::pair<int, int>> spawnCells;
	for (int x = 0; x < GRID_W; ++x)
	{
		if (reachableGrid[0][x])
			spawnCells.push_back({x, 0});
		if (reachableGrid[GRID_H - 1][x])
			spawnCells.push_back({x, GRID_H - 1});
	}
	for (int y = 1; y < GRID_H - 1; ++y)
	{
		if (reachableGrid[y][0])
			spawnCells.push_back({0, y});
		if (reachableGrid[y][GRID_W - 1])
			spawnCells.push_back({GRID_W - 1, y});
	}
	if (spawnCells.empty())
		return;

	auto cell = spawnCells[rand() % spawnCells.size()];
	int x = cell.first * CELL_SIZE + CELL_SIZE / 2;
	int y = cell.second * CELL_SIZE + CELL_SIZE / 2;
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
		{
			PlaySound(L"source/mus/hit.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			enemy->hurt();
		}

		dx = enemy->getAttackX() - player->getCenterX();
		dy = enemy->getAttackY() - player->getCenterY();
		r = enemy->getAttackRange() + player->getCollision();
		if (dx * dx + dy * dy <= r * r && enemy->isAttacking() && player->canHurt())
		{
			PlaySound(L"source/mus/hurt.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			player->hurt();
		}
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
			{
				mciSendString(L"play addHP from 0", nullptr, 0, nullptr);
				player->setCanAddHp(true);
				player->addHp(HP_BONUS_AMOUNT);
			}
			--i;
		}
	}
}
void GameScene::buildRenderOrder()
{
	renderOrder.clear();
	renderOrder.push_back(player.get());
	for (auto &e : enemyList)
		renderOrder.push_back(e.get());
	std::sort(renderOrder.begin(), renderOrder.end(),
			  [](Character *a, Character *b) { return a->getY() < b->getY(); });
}
void GameScene::renderGameObjects()
{
	buildRenderOrder();
	HDC hdc = GetImageHDC(nullptr);
	POINT oldOrigin;
	SetViewportOrgEx(hdc, -Camera::getInstance().getOffsetX(), -Camera::getInstance().getOffsetY(),
					 &oldOrigin);

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 100;
	_tcscpy_s(f.lfFaceName, L"Silicon Carne");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(RGB(60, 50, 70));
	LPCTSTR bg = L"MyRpg";
	outtextxy(WORLD_W / 2 - textwidth(bg) / 2, WORLD_H / 2 - textheight(bg) / 2, bg);

	// 障碍物
	drawObstacles(gObstacles);

	if (player->havePath())
	{
		setfillcolor(RED);
		line((int)player->getX(), (int)player->getY() - player->getHeight() / 2, player->getDestinationX(),
			 player->getDestinationY());
		solidcircle(player->getDestinationX(), player->getDestinationY(), 5);
	}
	for (auto *c : renderOrder)
		c->updateAnimation();
	SetViewportOrgEx(hdc, oldOrigin.x, oldOrigin.y, nullptr);
}
void GameScene::renderUI(int fps)
{
	setbkmode(TRANSPARENT);
	wchar_t buf[32];
	swprintf_s(buf, L"SCORE: %d", score);
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = (!(score % HP_BONUS_INTERVAL) && score) ? 25 : 20;
	_tcscpy_s(f.lfFaceName, L"Silicon Carne");
	settextstyle(&f);
	settextcolor((!(score % HP_BONUS_INTERVAL) && score) ? RED : WHITE);
	outtextxy(0, 0, buf);

	SYSTEMTIME t;
	GetLocalTime(&t);
	swprintf_s(buf, L"%02d:%02d:%02d", t.wHour, t.wMinute, t.wSecond);
	f.lfHeight = 20;
	settextstyle(&f);
	settextcolor(WHITE);
	outtextxy(WINDOWS_W - textwidth(buf) - 10, 0, buf);

	swprintf_s(buf, L"FPS: %d", fps);
	f.lfHeight = 18;
	settextstyle(&f);
	settextcolor(RGB(150, 150, 150));
	outtextxy(10, WINDOWS_H - textheight(buf) - 10, buf);
}
void GameScene::renderDebugInfo()
{
	HDC hdc = GetImageHDC(nullptr);
	POINT oldOrigin;
	SetViewportOrgEx(hdc, -Camera::getInstance().getOffsetX(), -Camera::getInstance().getOffsetY(),
					 &oldOrigin);
	for (auto &enemy : enemyList)
	{
		setlinecolor(RGB(255, 100, 100));
		line(player->getCenterX(), player->getCenterY(), enemy->getCenterX(), enemy->getCenterY());
		circle(enemy->getCenterX(), enemy->getCenterY(), enemy->getCollision());
		circle(enemy->getAttackX(), enemy->getAttackY(), enemy->getAttackRange());
	}
	setlinecolor(RGB(100, 255, 100));
	circle(player->getCenterX(), player->getCenterY(), player->getCollision());
	circle(player->getAttackX(), player->getAttackY(), player->getAttackRange());
	setlinecolor(RGB(100, 100, 255));
	rectangle(0, 0, WORLD_W, WORLD_H);
	SetViewportOrgEx(hdc, oldOrigin.x, oldOrigin.y, nullptr);
}
