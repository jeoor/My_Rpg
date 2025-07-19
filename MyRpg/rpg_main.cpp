// TODO: static_cast<int>((static_cast<double>bool - 0.5) * 2.0) --> bool{0, 1} --> int{-1, 1}

#include <iostream>
#include <windows.h>
#include <thread>
#include <atomic>
#include <string>
#include <mutex>
#include <wingdi.h>
#include <easyx.h>
#include <conio.h>
#include <time.h>
#include "alpha.h"
#include "edit.h"
#include "config.h"
#include "Player.h"
#include "Enemy.h"
#include "message.h"

inline static void initWindow(int w, int h, COLORREF color)
{
    initgraph(w, h, EX_SHOWCONSOLE);
    setbkcolor(color);
}

inline static void circle(int x, int y, int r, COLORREF color)
{
    setfillcolor(color);
    solidcircle(x, y, r);
}

static void setTextStyle(const wchar_t* font)
{
    LOGFONT f;
    gettextstyle(&f); // 获取当前字体设置
    f.lfHeight = 100; // 设置字体高度
    _tcscpy_s(f.lfFaceName, font); // 设置字体名称
    f.lfQuality = ANTIALIASED_QUALITY; // 关键：设置抗锯齿
    settextstyle(&f); // 应用字体样式
}

// 全局变量保存指令
std::string g_command;
std::atomic<bool> g_has_command{false};
std::mutex g_command_mutex;

// 控制台输入线程
static void ConsoleInputThread()
{
    std::string cmd;
    while (true)
    {
        std::cout << ">> "; // 输出提示符
        std::getline(std::cin, cmd);
        {
            std::lock_guard<std::mutex> lock(g_command_mutex);
            g_command = cmd;
            g_has_command = true;
        }
    }
}

int main()
{
    AddFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL);
    // 绘制窗口
    initWindow(WINDOWS_W, WINDOWS_H, RGB(40, 31, 48));

    setTextStyle(L"Silicon Carne");
    LPCTSTR text = L"MyRpg";
    int Fw = textwidth(text), Fh = textheight(text);
	int Fx = WINDOWS_W / 2, Fy = WINDOWS_H / 2; // 字体位置

    // 玩家
    Player player(WINDOWS_W / 2, WINDOWS_H / 2);
	Enemy enemy(-5, -5);

    // 将ExMessage移到堆上，减少栈空间占用
    ExMessage* msg = new ExMessage;
    wchar_t Time[32];
    SYSTEMTIME time;

    // 高精度计时器相关变量
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    // 启动控制台输入线程
    std::thread inputThread(ConsoleInputThread);
    inputThread.detach();

    BeginBatchDraw();
    while (true)
    {
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        GetLocalTime(&time);
        swprintf_s(Time, L"%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
        int TTw = textwidth(Time), TTh = textheight(Time);

        player.getMessage(msg); // 键盘消息处理
        player.updateState(); // 更新状态
		enemy.move2((int)player.getX(), (int)player.getY()); // 敌人跟随玩家
		enemy.updateState(); // 更新敌人状态

        // 检查是否有新指令
        if (g_has_command.exchange(false)) {
            std::cin.clear();
            std::string cmd;
            {
                std::lock_guard<std::mutex> lock(g_command_mutex);
                cmd = g_command;
            }
            if (cmd == "exit")
            {
				std::cout << "Exiting game..." << std::endl;
                break; // 退出游戏循环
            }
            else  if (cmd == "ls")
            {
				std::cout << player.getCurrentAnimation() << std::endl;
				std::cout << player.getX() << ", " << player.getY() << std::endl;
            }
            else if (cmd == "cls" || cmd == "clean")
            {
				system("cls"); // 清屏
            }
            else if (cmd == "help")
            {
                std::cout << "Available commands: exit, ls, help" << std::endl;
			}
            else if (cmd == "attack")
            {
                player.setAttacking(true);
            }
            else if (cmd == "flip")
            {
                player.changeFlip();
                if (player.getDir() == Character::dir::right)
                {
                    player.setDir(Character::dir::left);
                }
                else
                {
                    player.setDir(Character::dir::right);
				}
            }
            else
            {
                std::cout << "Unknown command: '" << cmd << "'" << std::endl;
				std::cout << "Type 'help' for available commands." << std::endl;
            }
        }

        cleardevice(); // 清除屏幕
        outtextxy(Fx - Fw / 2, Fy - Fh / 2, text);
        outtextxy(Fx - TTw / 2, Fy - 250 - TTh / 2, Time);

        if (player.haveT())
        {
            setfillcolor(RED);
			line(player.getX(), player.getY(), player.getTx(), player.getTy()); // 绘制目标线
            solidcircle(player.getTx(), player.getTy(), 5); // 绘制目标位置圆点
        }

        line(player.getX(), player.getY(), enemy.getX(), enemy.getY());
        if (player.getY() > enemy.getY())
        {
            enemy.updateAnimation();
            player.updateAnimation();
        }
        else
        {
            player.updateAnimation();
            enemy.updateAnimation();
        }

        FlushBatchDraw();

        QueryPerformanceCounter(&end);
        double delta_time = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
        if (delta_time < FRAME_BREAK) // 指定 FPS
        {
            Sleep((DWORD)(FRAME_BREAK - delta_time));
        }
    }
    EndBatchDraw();
    RemoveFontResourceEx(L"source/font/Silicon-Carne.ttf", FR_PRIVATE, NULL);
    delete msg; // 释放堆内存

    return 0;
}