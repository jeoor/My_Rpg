#include "timer.h"


void Timer::start() { running = true; }
void Timer::stop() { running = false; }
void Timer::reset()
{

}
double Timer::getElapsedTime() const // ��ȡ������ʱ�䣬��λΪ��
{
	return 0.0;
}
/*
double startTime = 0.0; // ��ʱ��ʼʱ��
double endTime = 0.0;   // ��ʱ����ʱ��
bool running = false;   // �Ƿ����ڼ�ʱ
*/