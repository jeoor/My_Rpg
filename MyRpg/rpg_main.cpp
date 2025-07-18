// TODO: ATTACK(); RBOTTODDONN

#include <iostream>
#include <windows.h>
#include <easyx.h>
#include <conio.h>
#include <time.h>
#include "alpha.h"
#include "edit.h"
#include "config.h"
#include "Player.h"
#include "kbmessage.h"

inline static void initWindow(int w, int h, COLORREF color)
{
    initgraph(w, h, EX_SHOWCONSOLE);
    setbkcolor(color);
}
/*
static void init(Character& C)
{
    C.init();
}
*/
inline static void circle(int x, int y, int r, COLORREF color)
{
    setfillcolor(color);
    solidcircle(x, y, r);
}

int main()
{
    // 绘制窗口
    initWindow(WINDOWS_W, WINDOWS_H, RGB(40, 31, 48));

    // 玩家
	Player player(WINDOWS_W / 2, WINDOWS_H / 2);
	// init(player);
	// player.set(WINDOWS_W / 2, WINDOWS_H / 2);

    ExMessage msg;

    // 高精度计时器相关变量
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    int x = 0, y = 0;
    int Tx = WINDOWS_W / 2, Ty = WINDOWS_H / 2;

    BeginBatchDraw();
    while (true)
    {
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        player.KBmove(msg); // 键盘消息处理
        if (player.isMoving())
            player.Run();
        if (!player.isMoving())
            player.Idle();
        player.Cmove(); // 移动

        // 获取鼠标消息并绘制圆点
        peekmessage(&msg, EX_MOUSE); // 只获取鼠标消息
        x = msg.x, y = msg.y;
        if (msg.message == WM_LBUTTONDOWN)
        {
            player.setKBcontrol(false); // 禁用键盘控制
            Tx = x, Ty = y; // 记录鼠标点击位置
            std::cout << "goto XY: (" << x << ", " << y << ")"
                << " current XY: (" << player.getX() << ", " << player.getY() << ")" << std::endl;
        }
        if (!player.KBcon())
            player.move2(Tx, Ty); // 移动到鼠标点击位置
        cleardevice(); // 清除屏幕
        player.playAnimation();
        if (!player.KBcon())
            circle(Tx, Ty, 5, RGB(0, 255, 0)); // 目标位置圆点
        if (player.KBcon() && !player.isMoving())
            circle(x, y, 5, RGB(255, 0, 0)); // 鼠标位置圆点

        FlushBatchDraw();

        QueryPerformanceCounter(&end);
        double delta_time = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
        if (delta_time < FRAME_BREAK) // 指定 FPS
        {
            Sleep((DWORD)(FRAME_BREAK - delta_time));
        }
    }
    EndBatchDraw();

    return 0;
}