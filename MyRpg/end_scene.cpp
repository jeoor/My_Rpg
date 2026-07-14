#include "end_scene.h"
#include "scene_manager.h"
#include "config.h"
#include "message.h"
#include <easyx.h>
#include <cstdio>

EndScene::EndScene(int score) : lastScore(score) {}
void EndScene::onEnter() { blinkTimer = 0; showPrompt = true; }
void EndScene::onExit() {}

GameState EndScene::onFrame(SceneManager & /*sm*/)
{
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY))
		if (msg.message == WM_KEYDOWN) {
			if (msg.vkcode == VK_RETURN) return GameState::Playing;
			if (msg.vkcode == VK_ESCAPE) return GameState::Quit;
		}

	blinkTimer += DELTA;
	if (blinkTimer >= DELTA_TIME * 5) { blinkTimer = 0; showPrompt = !showPrompt; }

	cleardevice(); setbkmode(TRANSPARENT);
	LOGFONT f; gettextstyle(&f); f.lfQuality = ANTIALIASED_QUALITY;

	f.lfHeight = 100; _tcscpy_s(f.lfFaceName, L"Silicon Carne"); settextstyle(&f);
	settextcolor(RED);
	LPCTSTR t = L"Game Over";
	outtextxy((WINDOWS_W - textwidth(t)) / 2, WINDOWS_H / 3, t);

	f.lfHeight = 50; settextstyle(&f); settextcolor(WHITE);
	wchar_t st[64]; swprintf_s(st, L"SCORE: %d", lastScore);
	outtextxy((WINDOWS_W - textwidth(st)) / 2, WINDOWS_H / 2 - 30, st);

	if (showPrompt) {
		f.lfHeight = 28; settextstyle(&f); settextcolor(RGB(200, 200, 200));
		LPCTSTR p1 = L"Press ENTER to Restart";
		LPCTSTR p2 = L"Press ESC to Menu";
		outtextxy((WINDOWS_W - textwidth(p1)) / 2, WINDOWS_H * 2 / 3, p1);
		outtextxy((WINDOWS_W - textwidth(p2)) / 2, WINDOWS_H * 2 / 3 + 40, p2);
	}

	FlushBatchDraw();
	Sleep((DWORD)FRAME_BREAK);
	return GameState::Running;
}
