#include "endScene.h"
#include "sceneManager.h"
#include "config.h"
#include "message.h"
#include <easyx.h>
#include <cstdio>
EndScene::EndScene(int score) : lastScore(score) {}
void EndScene::onEnter()
{
	blinkTimer = 0;
	showPrompt = true;
}
void EndScene::onExit() {}
GameState EndScene::onFrame(SceneManager & /*sm*/)
{
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY))
		if (msg.message == WM_KEYDOWN)
		{
			if (msg.vkcode == VK_RETURN)
				return GameState::Playing;
			if (msg.vkcode == VK_ESCAPE)
				return GameState::Quit;
		}

	blinkTimer += DELTA;
	if (blinkTimer >= DELTA_TIME * 5)
	{
		blinkTimer = 0;
		showPrompt = !showPrompt;
	}

	cleardevice();
	setbkmode(TRANSPARENT);
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;

	// 标题
	f.lfHeight = 72;
	_tcscpy_s(f.lfFaceName, L"Silicon Carne");
	settextstyle(&f);
	LPCTSTR t = L"GAME OVER";
	settextcolor(RGB(65, 20, 25));
	outtextxy((WINDOWS_W - textwidth(t)) / 2 + 4, 124, t);
	settextcolor(RGB(210, 45, 45));
	outtextxy((WINDOWS_W - textwidth(t)) / 2, 120, t);

	setlinecolor(RGB(110, 75, 90));
	line(WINDOWS_W / 2 - 210, 225, WINDOWS_W / 2 + 210, 225);

	// 分数卡片
	setfillcolor(RGB(38, 30, 48));
	solidrectangle(WINDOWS_W / 2 - 190, 275, WINDOWS_W / 2 + 190, 385);
	setlinecolor(RGB(110, 85, 135));
	rectangle(WINDOWS_W / 2 - 190, 275, WINDOWS_W / 2 + 190, 385);

	f.lfHeight = 22;
	settextstyle(&f);
	settextcolor(RGB(150, 135, 165));
	LPCTSTR label = L"FINAL SCORE";
	outtextxy((WINDOWS_W - textwidth(label)) / 2, 292, label);

	f.lfHeight = 48;
	settextstyle(&f);
	settextcolor(WHITE);
	wchar_t st[32];
	swprintf_s(st, L"%d", lastScore);
	outtextxy((WINDOWS_W - textwidth(st)) / 2, 325, st);

	if (showPrompt)
	{
		f.lfHeight = 24;
		settextstyle(&f);
		settextcolor(RGB(210, 200, 220));
		LPCTSTR p1 = L"Press ENTER to Restart";
		LPCTSTR p2 = L"Press ESC to Menu";
		outtextxy((WINDOWS_W - textwidth(p1)) / 2, 485, p1);
		settextcolor(RGB(145, 130, 155));
		outtextxy((WINDOWS_W - textwidth(p2)) / 2, 530, p2);
	}

	return GameState::Running;
}
