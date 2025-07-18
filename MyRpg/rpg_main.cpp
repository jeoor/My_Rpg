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

int main()
{
    // ���ƴ���
    initWindow(WINDOWS_W, WINDOWS_H, RGB(40, 31, 48));

    // ���
	Player player(WINDOWS_W / 2, WINDOWS_H / 2);

    ExMessage msg;

    // �߾��ȼ�ʱ����ر���
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    int x = 0, y = 0;

    BeginBatchDraw();
    while (true)
    {
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        player.getMessage(msg); // ������Ϣ����
        player.updateState(); // ����״̬

        cleardevice(); // �����Ļ
        player.updateAnimation();

        if (player.haveT())
            solidcircle(player.getTx(), player.getTy(), 5); // ����Ŀ��λ��Բ��

        FlushBatchDraw();

        QueryPerformanceCounter(&end);
        double delta_time = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
        if (delta_time < FRAME_BREAK) // ָ�� FPS
        {
            Sleep((DWORD)(FRAME_BREAK - delta_time));
        }
    }
    EndBatchDraw();

    return 0;
}