#include <windows.h>
#include <time.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <easyx.h>
#include "alpha.h"
#include "edit.h"
#include "config.h"
#include "player.h"
#include "enemy.h"
#include "console.h"
#include "message.h"
#pragma comment(lib, "Winmm.lib")

Enemy* addEnemy();
void initWindow(int w, int h, COLORREF color, bool showConsole);
void setTextStyle(LONG size = 100, const wchar_t *font = L"Silicon Carne");

int main()
{
	// 加载音乐
	mciSendString(L"open source/mus/load.mp3 alias load", nullptr, 0, nullptr);
	// 提示信息
	MessageBox(NULL, L"移动: WASD/ ↑↓←→/ 鼠标点击\n攻击: J/ 鼠标右键\n显示碰撞信息: F1", L"操作", MB_OK|MB_ICONASTERISK);
	mciSendString(L"play load from 0", nullptr, 0, nullptr);
	// 加载字体
	AddFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL);
	// 加载音效
	mciSendString(L"open source/mus/bgm.mp3 alias bgm", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/hit.wav alias hit", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/hurt.wav alias hurt", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/addHP.mp3 alias addHP", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/dead.mp3 alias dead", nullptr, 0, nullptr);
	// 绘制窗口
    initWindow(WINDOWS_W, WINDOWS_H, RGB(40, 31, 48), false);
	cleardevice();
	setTextStyle(80);
	LPCTSTR loading = L"LOADING...";
	outtextxy((WINDOWS_W - textwidth(loading)) / 2, (WINDOWS_H -  textheight(loading)) / 2, loading);
	// 玩家初始化
    Player player(WINDOWS_W / 2, WINDOWS_H / 2);
	// 初始化敌人列表
    std::vector<Enemy *> Enemy_list;

    // 将ExMessage移到堆上，减少栈空间占用
    ExMessage* msg = new ExMessage;
	
	// 背景文字
	LPCTSTR text = L"MyRpg";
	// 时间相关变量
    wchar_t Time[32];
	wchar_t Score[32];
    SYSTEMTIME time;
    // 高精度计时器相关变量
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

	// 初始化控制台输入线程
    // initConsole();
// 游戏开始	
GAMEBEGIN:
	int timer = 0;		// 敌人刷新计时器
	int oldScore = 0;	// 上一帧的分数
	int score = 0;		// 分数记录
	// 播放BGM
	mciSendString(L"play bgm repeat from 0", nullptr, 0, nullptr);
    
	BeginBatchDraw();
    while (true)
    {
		// 本循环所用时间计算
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start); // 获取循环开始时时间

		// 开始绘制前处理

		// 玩家死亡退出循环
		if (!player.isAlive())
		{
			mciSendString(L"play dead from 0", nullptr, 0, nullptr);
			// 释放所有的敌人
			while (!Enemy_list.empty())
			{
				Enemy* temp = Enemy_list.back();
				Enemy_list.pop_back();
				delete temp;
			}
			if (MessageBox(NULL, L"死亡! 是否重新开始?", L"死亡", MB_YESNO|MB_ICONHAND) == IDYES)
			{
				player.set(static_cast<double>(WINDOWS_W / 2), static_cast<double>(WINDOWS_H / 2));
				player.setHP(200);
				goto GAMEBEGIN;
			}
			else
				break;
		}
		
		// 记录上一帧分数
		oldScore = score;

		// 时间相关相关
        GetLocalTime(&time); // 获取系统时间
        swprintf_s(Time, L"%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
        int TTw = textwidth(Time), TTh = textheight(Time); // 为下面计算绘制位置做准备

		// 增加敌人
		timer += DELTA;
		if(timer >= 5 * DELTA_TIME)
		{
			if (Enemy_list.size() < 10)
			{
				timer = 0;
				Enemy_list.emplace_back(addEnemy()); // 随机生成敌人
			}
		}
		// 玩家接收信息
        player.getMessage(msg);		// 键盘消息处理
        // peekConsole(player);		// 控制台消息处理
        player.updateState();		// 更新玩家状态

		// 更新敌人状态
        for (auto& enemy : Enemy_list) 
		{
			enemy->move2((int)player.getX(), (int)player.getY()); // 敌人跟随玩家

			// 更新敌人攻击状态
			int delta_x = enemy->getAttackX() - player.getCenterX();
			int delta_y = enemy->getAttackY() - player.getCenterY();
			int range = enemy->getAttackRange() + player.getCollision();
			if (delta_x * delta_x + delta_y * delta_y <= range * range)
				enemy->setAttacking(true);
			else
				enemy->setAttacking(false);

			enemy->updateState(); // 更新敌人状态
		}

		// 受伤检测
		for (auto& enemy : Enemy_list)
		{
			// 检测敌人受伤
			int delta_x = enemy->getCenterX() - player.getAttackX();
			int delta_y = enemy->getCenterY() - player.getAttackY();
			int range = enemy->getCollision() + player.getAttackRange();
			if (delta_x * delta_x + delta_y * delta_y <= range * range && player.isAttacking())
			{
				if (enemy->canHurt())
				{
					mciSendString(L"play hit from 0", nullptr, 0, nullptr);
					enemy->Hurt();
				}
			}
			// 检测玩家受伤
			delta_x = enemy->getAttackX() - player.getCenterX();
			delta_y = enemy->getAttackY() - player.getCenterY();
			range = enemy->getAttackRange() + player.getCollision();
			if (delta_x * delta_x + delta_y * delta_y <= range * range && enemy->isAttacking())
				if (player.canHurt())
				{
					mciSendString(L"play hurt from 0", nullptr, 0, nullptr);
					player.Hurt();
				}
		}
		// 清除死掉的敌人
		for (size_t i = 0; i < Enemy_list.size(); ++i)
		{
			if (!Enemy_list[i]->isAlive())
			{
				Enemy *temp = Enemy_list[i];
				std::swap(Enemy_list[i], Enemy_list.back());
				Enemy_list.pop_back();
				delete temp;
				score++; // 杀死敌人加分
			}
		}

		// 按照Y值排序
		std::sort(Enemy_list.begin(), Enemy_list.end(), [](Enemy* a, Enemy* b) {return a->getY() < b->getY(); });

		// 检测加分
		if (!(score % 10) && oldScore != score)
		{
			mciSendString(L"play addHP from 0", nullptr, 0, nullptr);
			player.setCanAddHP(true);
			player.addHP(20);
		}
		// 分数相关变量
		swprintf_s(Score, L"SCORE: %d", score);
        
		cleardevice(); // 处理完毕 开始绘制

		// 字体放在最后面
		if (!(score % 10) && score)
		{
			settextcolor(RED);
			setTextStyle(25);
		}
		else
		{
			settextcolor(WHITE);
			setTextStyle(20);
		}
		outtextxy(0, 0, Score);
		settextcolor(WHITE);
		setTextStyle(100);
		int Fw = textwidth(text), Fh = textheight(text);
		int Fx = WINDOWS_W / 2, Fy = WINDOWS_H / 2; // 字体位置
        outtextxy(Fx - Fw / 2, Fy - Fh / 2, text);
		setTextStyle(100);
		outtextxy(Fx - TTw / 2, Fy - 250 - TTh / 2, Time);

		// 绘制玩家
		player.updateAnimation();
		if (player.haveT()) // 绘制玩家目标位置圆点
        {
            setfillcolor(RED);
			line(static_cast<int>(player.getX()), static_cast<int>(player.getY()) - player.getHeight() / 2, static_cast<int>(player.getTx()), static_cast<int>(player.getTy())); // 绘制目标线
            solidcircle(player.getTx(), player.getTy(), 5);
        }

		// 绘制敌人
		for (auto& enemy : Enemy_list)
			enemy->updateAnimation(); // 更新敌人动画
		// 绘制连线(调试信息)
		if (player.Debug())
		{
			for (auto& enemy : Enemy_list)
			{
				line(player.getCenterX(), player.getCenterY(), enemy->getCenterX(), enemy->getCenterY());
				circle(enemy->getCenterX(), enemy->getCenterY(), enemy->getCollision());// 绘制敌人碰撞范围
				circle(enemy->getAttackX(), enemy->getAttackY(), enemy->getAttackRange()); // 绘制敌人攻击范围
			}
			circle(player.getCenterX(), player.getCenterY(), player.getCollision());// 绘制玩家碰撞范围
			circle(player.getAttackX(), player.getAttackY(), player.getAttackRange()); // 绘制玩家攻击范围
		}
        FlushBatchDraw(); // 完成当前帧绘制

        QueryPerformanceCounter(&end); // 获取循环结束时间
        double delta_time = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart; // 计算循环所用时间
        if (delta_time < FRAME_BREAK) // 指定 FPS
        {
            Sleep((DWORD)(FRAME_BREAK - delta_time)); // 动态休眠
        }
    }
    EndBatchDraw();
	// 释放资源
    RemoveFontResourceEx(L"source/font/Silicon-Carne.ttf", FR_PRIVATE, NULL); // 释放字体
    delete msg; // 释放堆内存

    return 0;
}

Enemy* addEnemy()
{
	int side = rand() % 4;
	int x = 0, y = 0;
	switch (side)
	{
	case 0:
		y = -1;
		x = rand() % WINDOWS_W;
		break;
	case 1:
		y = WINDOWS_H;
		x = rand() % WINDOWS_W;
		break;
	case 2:
		x = -1;
		y = rand() % WINDOWS_H;
		break;
	case 3:
		x = WINDOWS_W;
		y = y = rand() % WINDOWS_H;
		break;
	}
	return new Enemy(x, y);
}

void initWindow(int w, int h, COLORREF color, bool showConsole)
{
	showConsole ? initgraph(w, h, EX_SHOWCONSOLE) : initgraph(w, h);
	setbkcolor(color);
}

void setTextStyle(LONG size, const wchar_t *font)
{
	LOGFONT f;
	gettextstyle(&f);                  // 获取当前字体设置
	f.lfHeight = size;                 // 设置字体高度
	_tcscpy_s(f.lfFaceName, font);     // 设置字体名称
	f.lfQuality = ANTIALIASED_QUALITY; // 关键：设置抗锯齿
	settextstyle(&f);                  // 应用字体样式
}