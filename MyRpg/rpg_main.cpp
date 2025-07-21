#include <windows.h>
#include <time.h>
#include <vector>
#include <cmath>
#include <easyx.h>
#include "alpha.h"
#include "edit.h"
#include "config.h"
#include "player.h"
#include "enemy.h"
#include "console.h"
#include "message.h"

void initWindow(int w, int h, COLORREF color, bool showConsole);
void setTextStyle(LONG size, const wchar_t *font);

int main()
{
	// 加载字体
    AddFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL);
    // 绘制窗口
    initWindow(WINDOWS_W, WINDOWS_H, RGB(40, 31, 48), true);

	setTextStyle(100, L"Silicon Carne");
    LPCTSTR text = L"MyRpg";
    int Fw = textwidth(text), Fh = textheight(text);
	int Fx = WINDOWS_W / 2, Fy = WINDOWS_H / 2; // 字体位置

	// 玩家初始化
    Player player(WINDOWS_W / 2, WINDOWS_H / 2);

    std::vector<Enemy *> Enemy_list;

    // 将ExMessage移到堆上，减少栈空间占用
    ExMessage* msg = new ExMessage;

	// 时间相关变量
    wchar_t Time[32];
    SYSTEMTIME time;

    // 高精度计时器相关变量
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

	// 初始化控制台输入线程
    initConsole();

    BeginBatchDraw();
    while (true)
    {
		// 本循环所用时间计算
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start); // 获取循环开始时时间

		// 开始绘制前处理

		// 时间相关相关
        GetLocalTime(&time); // 获取系统时间
        swprintf_s(Time, L"%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
        int TTw = textwidth(Time), TTh = textheight(Time); // 为下面计算绘制位置做准备

		// 增加敌人
		if (Enemy_list.size() < 10)
			Enemy_list.emplace_back(new Enemy(rand() % WINDOWS_W, rand() % WINDOWS_H)); // 随机生成敌人

		// 玩家信息处理
        player.getMessage(msg);		// 键盘消息处理
        peekConsole(player);		// 控制台消息处理
        player.updateState();		// 更新状态

        for (auto& enemy : Enemy_list) {
			enemy->move2((int)player.getX(), (int)player.getY()); // 敌人跟随玩家

			//更新攻击状态
			int delta_x = enemy->getAttackX() - player.getCenterX();
			int delta_y = enemy->getAttackY() - player.getCenterY();
			int range = enemy->getAttackRange() + player.getCollision();
			if (delta_x * delta_x + delta_y * delta_y <= range * range)
				enemy->setAttacking(true);
			else
				enemy->setAttacking(false);

			enemy->updateState(); // 更新敌人状态
		}

		// 攻击检测
		for (auto& enemy : Enemy_list)
		{
			int delta_x = enemy->getCenterX() - player.getAttackX();
			int delta_y = enemy->getCenterY() - player.getAttackY();
			int range = enemy->getCollision() + player.getAttackRange();
			if (delta_x * delta_x + delta_y * delta_y <= range * range && player.isAttacking())
				enemy->Hurt();
		}

		for (int i = 0; i < Enemy_list.size(); ++i)
		{
			if (!Enemy_list[i]->isAlive())
			{
				Enemy *temp = Enemy_list[i];
				std::swap(Enemy_list[i], Enemy_list.back());
				Enemy_list.pop_back();
				delete temp;
			}
		}

        cleardevice(); // 处理完毕 开始绘制

		// 字体放在最后面
        outtextxy(Fx - Fw / 2, Fy - Fh / 2, text);
        outtextxy(Fx - TTw / 2, Fy - 250 - TTh / 2, Time);
		// 更新玩家
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
		// 绘制连线
        for (auto& enemy : Enemy_list)
        {
            line(player.getCenterX(), player.getCenterY(), enemy->getCenterX(), enemy->getCenterY());
            circle(enemy->getCenterX(), enemy->getCenterY(), enemy->getCollision());// 绘制敌人碰撞范围
            circle(enemy->getAttackX(), enemy->getAttackY(), enemy->getAttackRange()); // 绘制敌人攻击范围
        }
        circle(player.getCenterX(), player.getCenterY(), player.getCollision());// 绘制玩家碰撞范围
		circle(player.getAttackX(), player.getAttackY(), player.getAttackRange()); // 绘制玩家攻击范围

        FlushBatchDraw(); // 完成当前帧绘制

        QueryPerformanceCounter(&end); // 获取循环结束时间
        double delta_time = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart; // 计算循环所用时间
        if (delta_time < FRAME_BREAK) // 指定 FPS
        {
            Sleep((DWORD)(FRAME_BREAK - delta_time)); // 动态休眠
        }
    }
    EndBatchDraw();

    RemoveFontResourceEx(L"source/font/Silicon-Carne.ttf", FR_PRIVATE, NULL); // 释放字体
    delete msg; // 释放堆内存

    return 0;
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