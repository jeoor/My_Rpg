#include "timer.h"


void Timer::start() { running = true; }
void Timer::stop() { running = false; }
void Timer::reset()
{

}
double Timer::getElapsedTime() const // 获取经过的时间，单位为秒
{
	return 0.0;
}
/*
double startTime = 0.0; // 计时开始时间
double endTime = 0.0;   // 计时结束时间
bool running = false;   // 是否正在计时
*/