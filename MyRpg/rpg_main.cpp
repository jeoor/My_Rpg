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
	// ��������
	mciSendString(L"open source/mus/load.mp3 alias load", nullptr, 0, nullptr);
	// ��ʾ��Ϣ
	MessageBox(NULL, L"�ƶ�: WASD/ ��������/ �����\n����: J/ ����Ҽ�\n��ʾ��ײ��Ϣ: F1", L"����", MB_OK|MB_ICONASTERISK);
	mciSendString(L"play load from 0", nullptr, 0, nullptr);
	// ��������
	AddFontResourceEx(L"source/fonts/Silicon-Carne.ttf", FR_PRIVATE, NULL);
	// ������Ч
	mciSendString(L"open source/mus/bgm.mp3 alias bgm", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/hit.wav alias hit", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/hurt.wav alias hurt", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/addHP.mp3 alias addHP", nullptr, 0, nullptr);
	mciSendString(L"open source/mus/dead.mp3 alias dead", nullptr, 0, nullptr);
	// ���ƴ���
    initWindow(WINDOWS_W, WINDOWS_H, RGB(40, 31, 48), false);
	cleardevice();
	setTextStyle(80);
	LPCTSTR loading = L"LOADING...";
	outtextxy((WINDOWS_W - textwidth(loading)) / 2, (WINDOWS_H -  textheight(loading)) / 2, loading);
	// ��ҳ�ʼ��
    Player player(WINDOWS_W / 2, WINDOWS_H / 2);
	// ��ʼ�������б�
    std::vector<Enemy *> Enemy_list;

    // ��ExMessage�Ƶ����ϣ�����ջ�ռ�ռ��
    ExMessage* msg = new ExMessage;
	
	// ��������
	LPCTSTR text = L"MyRpg";
	// ʱ����ر���
    wchar_t Time[32];
	wchar_t Score[32];
    SYSTEMTIME time;
    // �߾��ȼ�ʱ����ر���
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

	// ��ʼ������̨�����߳�
    // initConsole();
// ��Ϸ��ʼ	
GAMEBEGIN:
	int timer = 0;		// ����ˢ�¼�ʱ��
	int oldScore = 0;	// ��һ֡�ķ���
	int score = 0;		// ������¼
	// ����BGM
	mciSendString(L"play bgm repeat from 0", nullptr, 0, nullptr);
    
	BeginBatchDraw();
    while (true)
    {
		// ��ѭ������ʱ�����
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start); // ��ȡѭ����ʼʱʱ��

		// ��ʼ����ǰ����

		// ��������˳�ѭ��
		if (!player.isAlive())
		{
			mciSendString(L"play dead from 0", nullptr, 0, nullptr);
			// �ͷ����еĵ���
			while (!Enemy_list.empty())
			{
				Enemy* temp = Enemy_list.back();
				Enemy_list.pop_back();
				delete temp;
			}
			if (MessageBox(NULL, L"����! �Ƿ����¿�ʼ?", L"����", MB_YESNO|MB_ICONHAND) == IDYES)
			{
				player.set(static_cast<double>(WINDOWS_W / 2), static_cast<double>(WINDOWS_H / 2));
				player.setHP(200);
				goto GAMEBEGIN;
			}
			else
				break;
		}
		
		// ��¼��һ֡����
		oldScore = score;

		// ʱ��������
        GetLocalTime(&time); // ��ȡϵͳʱ��
        swprintf_s(Time, L"%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
        int TTw = textwidth(Time), TTh = textheight(Time); // Ϊ����������λ����׼��

		// ���ӵ���
		timer += DELTA;
		if(timer >= 5 * DELTA_TIME)
		{
			if (Enemy_list.size() < 10)
			{
				timer = 0;
				Enemy_list.emplace_back(addEnemy()); // ������ɵ���
			}
		}
		// ��ҽ�����Ϣ
        player.getMessage(msg);		// ������Ϣ����
        // peekConsole(player);		// ����̨��Ϣ����
        player.updateState();		// �������״̬

		// ���µ���״̬
        for (auto& enemy : Enemy_list) 
		{
			enemy->move2((int)player.getX(), (int)player.getY()); // ���˸������

			// ���µ��˹���״̬
			int delta_x = enemy->getAttackX() - player.getCenterX();
			int delta_y = enemy->getAttackY() - player.getCenterY();
			int range = enemy->getAttackRange() + player.getCollision();
			if (delta_x * delta_x + delta_y * delta_y <= range * range)
				enemy->setAttacking(true);
			else
				enemy->setAttacking(false);

			enemy->updateState(); // ���µ���״̬
		}

		// ���˼��
		for (auto& enemy : Enemy_list)
		{
			// ����������
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
			// ����������
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
		// ��������ĵ���
		for (size_t i = 0; i < Enemy_list.size(); ++i)
		{
			if (!Enemy_list[i]->isAlive())
			{
				Enemy *temp = Enemy_list[i];
				std::swap(Enemy_list[i], Enemy_list.back());
				Enemy_list.pop_back();
				delete temp;
				score++; // ɱ�����˼ӷ�
			}
		}

		// ����Yֵ����
		std::sort(Enemy_list.begin(), Enemy_list.end(), [](Enemy* a, Enemy* b) {return a->getY() < b->getY(); });

		// ���ӷ�
		if (!(score % 10) && oldScore != score)
		{
			mciSendString(L"play addHP from 0", nullptr, 0, nullptr);
			player.setCanAddHP(true);
			player.addHP(20);
		}
		// ������ر���
		swprintf_s(Score, L"SCORE: %d", score);
        
		cleardevice(); // ������� ��ʼ����

		// ������������
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
		int Fx = WINDOWS_W / 2, Fy = WINDOWS_H / 2; // ����λ��
        outtextxy(Fx - Fw / 2, Fy - Fh / 2, text);
		setTextStyle(100);
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
		// ��������(������Ϣ)
		if (player.Debug())
		{
			for (auto& enemy : Enemy_list)
			{
				line(player.getCenterX(), player.getCenterY(), enemy->getCenterX(), enemy->getCenterY());
				circle(enemy->getCenterX(), enemy->getCenterY(), enemy->getCollision());// ���Ƶ�����ײ��Χ
				circle(enemy->getAttackX(), enemy->getAttackY(), enemy->getAttackRange()); // ���Ƶ��˹�����Χ
			}
			circle(player.getCenterX(), player.getCenterY(), player.getCollision());// ���������ײ��Χ
			circle(player.getAttackX(), player.getAttackY(), player.getAttackRange()); // ������ҹ�����Χ
		}
        FlushBatchDraw(); // ��ɵ�ǰ֡����

        QueryPerformanceCounter(&end); // ��ȡѭ������ʱ��
        double delta_time = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart; // ����ѭ������ʱ��
        if (delta_time < FRAME_BREAK) // ָ�� FPS
        {
            Sleep((DWORD)(FRAME_BREAK - delta_time)); // ��̬����
        }
    }
    EndBatchDraw();
	// �ͷ���Դ
    RemoveFontResourceEx(L"source/font/Silicon-Carne.ttf", FR_PRIVATE, NULL); // �ͷ�����
    delete msg; // �ͷŶ��ڴ�

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
	gettextstyle(&f);                  // ��ȡ��ǰ��������
	f.lfHeight = size;                 // ��������߶�
	_tcscpy_s(f.lfFaceName, font);     // ������������
	f.lfQuality = ANTIALIASED_QUALITY; // �ؼ������ÿ����
	settextstyle(&f);                  // Ӧ��������ʽ
}