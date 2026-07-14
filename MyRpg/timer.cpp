#include "timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency(&freq);
	startTime.QuadPart = 0;
	endTime.QuadPart = 0;
}

void Timer::start()
{
	running = true;
	QueryPerformanceCounter(&startTime);
}

void Timer::stop()
{
	running = false;
	QueryPerformanceCounter(&endTime);
}

void Timer::reset()
{
	running = false;
	startTime.QuadPart = 0;
	endTime.QuadPart = 0;
}

double Timer::elapsed() const
{
	if (!running)
		return static_cast<double>(endTime.QuadPart - startTime.QuadPart) / freq.QuadPart;

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return static_cast<double>(now.QuadPart - startTime.QuadPart) / freq.QuadPart;
}

bool Timer::isRunning() const { return running; }
