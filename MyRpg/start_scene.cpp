#include "start_scene.h"
#include "scene_manager.h"
#include "config.h"
#include <easyx.h>

void StartScene::onEnter() { blinkTimer = 0; showPrompt = true; }
void StartScene::onExit() {}

GameState StartScene::onFrame(SceneManager & /*sm*/)
{
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY | EX_MOUSE))
		if (msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONDOWN)
			return GameState::Playing;

	blinkTimer += DELTA;
	if (blinkTimer >= DELTA_TIME * 5) { blinkTimer = 0; showPrompt = !showPrompt; }

	cleardevice(); setbkmode(TRANSPARENT);
	LOGFONT f; gettextstyle(&f); f.lfQuality = ANTIALIASED_QUALITY;

	f.lfHeight = 120; _tcscpy_s(f.lfFaceName, L"Silicon Carne"); settextstyle(&f);
	settextcolor(WHITE);
	LPCTSTR t = L"MyRpg";
	int tw = textwidth(t), th = textheight(t);
	outtextxy((WINDOWS_W - tw) / 2, WINDOWS_H / 2 - th - 30, t);

	if (showPrompt) {
		f.lfHeight = 30; settextstyle(&f); settextcolor(RGB(200, 200, 200));
		LPCTSTR p = L"Press Any Key to Start";
		outtextxy((WINDOWS_W - textwidth(p)) / 2, WINDOWS_H / 2 + 30, p);
	}

	f.lfHeight = 22; settextstyle(&f); settextcolor(RGB(150, 150, 150));
	LPCTSTR c1 = L"Move: WASD / Arrow Keys / Mouse Left Click";
	LPCTSTR c2 = L"Attack: J / Mouse Right Click  Debug: F1";
	outtextxy((WINDOWS_W - textwidth(c1)) / 2, WINDOWS_H - 100, c1);
	outtextxy((WINDOWS_W - textwidth(c2)) / 2, WINDOWS_H - 70, c2);

	FlushBatchDraw();
	Sleep((DWORD)FRAME_BREAK);
	return GameState::Running;
}
