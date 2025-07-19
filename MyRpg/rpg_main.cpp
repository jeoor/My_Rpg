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
    gettextstyle(&f); // ��ȡ��ǰ��������
    f.lfHeight = 100; // ��������߶�
    _tcscpy_s(f.lfFaceName, font); // ������������
    f.lfQuality = ANTIALIASED_QUALITY; // �ؼ������ÿ����
    settextstyle(&f); // Ӧ��������ʽ
}

// ȫ�ֱ�������ָ��
std::string g_command;
std::atomic<bool> g_has_command{false};
std::mutex g_command_mutex;

// ����̨�����߳�
static void ConsoleInputThread()
{
    std::string cmd;
    while (true)
    {
        std::cout << ">> "; // �����ʾ��
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
    // ���ƴ���
    initWindow(WINDOWS_W, WINDOWS_H, RGB(40, 31, 48));

    setTextStyle(L"Silicon Carne");
    LPCTSTR text = L"MyRpg";
    int Fw = textwidth(text), Fh = textheight(text);
	int Fx = WINDOWS_W / 2, Fy = WINDOWS_H / 2; // ����λ��

    // ���
    Player player(WINDOWS_W / 2, WINDOWS_H / 2);
	Enemy enemy(-5, -5);

    // ��ExMessage�Ƶ����ϣ�����ջ�ռ�ռ��
    ExMessage* msg = new ExMessage;
    wchar_t Time[32];
    SYSTEMTIME time;

    // �߾��ȼ�ʱ����ر���
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    // ��������̨�����߳�
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

        player.getMessage(msg); // ������Ϣ����
        player.updateState(); // ����״̬
		enemy.move2((int)player.getX(), (int)player.getY()); // ���˸������
		enemy.updateState(); // ���µ���״̬

        // ����Ƿ�����ָ��
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
                break; // �˳���Ϸѭ��
            }
            else  if (cmd == "ls")
            {
				std::cout << player.getCurrentAnimation() << std::endl;
				std::cout << player.getX() << ", " << player.getY() << std::endl;
            }
            else if (cmd == "cls" || cmd == "clean")
            {
				system("cls"); // ����
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

        cleardevice(); // �����Ļ
        outtextxy(Fx - Fw / 2, Fy - Fh / 2, text);
        outtextxy(Fx - TTw / 2, Fy - 250 - TTh / 2, Time);

        if (player.haveT())
        {
            setfillcolor(RED);
			line(player.getX(), player.getY(), player.getTx(), player.getTy()); // ����Ŀ����
            solidcircle(player.getTx(), player.getTy(), 5); // ����Ŀ��λ��Բ��
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
        if (delta_time < FRAME_BREAK) // ָ�� FPS
        {
            Sleep((DWORD)(FRAME_BREAK - delta_time));
        }
    }
    EndBatchDraw();
    RemoveFontResourceEx(L"source/font/Silicon-Carne.ttf", FR_PRIVATE, NULL);
    delete msg; // �ͷŶ��ڴ�

    return 0;
}