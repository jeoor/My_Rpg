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
	// ��������
    AddFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL);
    // ���ƴ���
    initWindow(WINDOWS_W, WINDOWS_H, RGB(40, 31, 48), true);

	setTextStyle(100, L"Silicon Carne");
    LPCTSTR text = L"MyRpg";
    int Fw = textwidth(text), Fh = textheight(text);
	int Fx = WINDOWS_W / 2, Fy = WINDOWS_H / 2; // ����λ��

	// ��ҳ�ʼ��
    Player player(WINDOWS_W / 2, WINDOWS_H / 2);

    std::vector<Enemy *> Enemy_list;

    // ��ExMessage�Ƶ����ϣ�����ջ�ռ�ռ��
    ExMessage* msg = new ExMessage;

	// ʱ����ر���
    wchar_t Time[32];
    SYSTEMTIME time;

    // �߾��ȼ�ʱ����ر���
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

	// ��ʼ������̨�����߳�
    initConsole();

    BeginBatchDraw();
    while (true)
    {
		// ��ѭ������ʱ�����
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start); // ��ȡѭ����ʼʱʱ��

		// ��ʼ����ǰ����

		// ʱ��������
        GetLocalTime(&time); // ��ȡϵͳʱ��
        swprintf_s(Time, L"%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
        int TTw = textwidth(Time), TTh = textheight(Time); // Ϊ����������λ����׼��

		// ���ӵ���
		if (Enemy_list.size() < 10)
			Enemy_list.emplace_back(new Enemy(rand() % WINDOWS_W, rand() % WINDOWS_H)); // ������ɵ���

		// �����Ϣ����
        player.getMessage(msg);		// ������Ϣ����
        peekConsole(player);		// ����̨��Ϣ����
        player.updateState();		// ����״̬

        for (auto& enemy : Enemy_list) {
			enemy->move2((int)player.getX(), (int)player.getY()); // ���˸������

			//���¹���״̬
			int delta_x = enemy->getAttackX() - player.getCenterX();
			int delta_y = enemy->getAttackY() - player.getCenterY();
			int range = enemy->getAttackRange() + player.getCollision();
			if (delta_x * delta_x + delta_y * delta_y <= range * range)
				enemy->setAttacking(true);
			else
				enemy->setAttacking(false);

			enemy->updateState(); // ���µ���״̬
		}

		// �������
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

        cleardevice(); // ������� ��ʼ����

		// ������������
        outtextxy(Fx - Fw / 2, Fy - Fh / 2, text);
        outtextxy(Fx - TTw / 2, Fy - 250 - TTh / 2, Time);
		// �������
		player.updateAnimation();
		if (player.haveT()) // �������Ŀ��λ��Բ��
        {
            setfillcolor(RED);
			line(static_cast<int>(player.getX()), static_cast<int>(player.getY()) - player.getHeight() / 2, static_cast<int>(player.getTx()), static_cast<int>(player.getTy())); // ����Ŀ����
            solidcircle(player.getTx(), player.getTy(), 5);
        }

		// ���Ƶ���
		for (auto& enemy : Enemy_list)
			enemy->updateAnimation(); // ���µ��˶���
		// ��������
        for (auto& enemy : Enemy_list)
        {
            line(player.getCenterX(), player.getCenterY(), enemy->getCenterX(), enemy->getCenterY());
            circle(enemy->getCenterX(), enemy->getCenterY(), enemy->getCollision());// ���Ƶ�����ײ��Χ
            circle(enemy->getAttackX(), enemy->getAttackY(), enemy->getAttackRange()); // ���Ƶ��˹�����Χ
        }
        circle(player.getCenterX(), player.getCenterY(), player.getCollision());// ���������ײ��Χ
		circle(player.getAttackX(), player.getAttackY(), player.getAttackRange()); // ������ҹ�����Χ

        FlushBatchDraw(); // ��ɵ�ǰ֡����

        QueryPerformanceCounter(&end); // ��ȡѭ������ʱ��
        double delta_time = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart; // ����ѭ������ʱ��
        if (delta_time < FRAME_BREAK) // ָ�� FPS
        {
            Sleep((DWORD)(FRAME_BREAK - delta_time)); // ��̬����
        }
    }
    EndBatchDraw();

    RemoveFontResourceEx(L"source/font/Silicon-Carne.ttf", FR_PRIVATE, NULL); // �ͷ�����
    delete msg; // �ͷŶ��ڴ�

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
	gettextstyle(&f);                  // ��ȡ��ǰ��������
	f.lfHeight = size;                 // ��������߶�
	_tcscpy_s(f.lfFaceName, font);     // ������������
	f.lfQuality = ANTIALIASED_QUALITY; // �ؼ������ÿ����
	settextstyle(&f);                  // Ӧ��������ʽ
}