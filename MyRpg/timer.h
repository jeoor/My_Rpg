#ifndef __TIMER_H__
#define __TIMER_H__

#include <windows.h>

class Timer
{
public:
	Timer();

	void start();
	void stop();
	void reset();
	double elapsed() const;     // Elapsed time in seconds
	bool isRunning() const;

private:
	LARGE_INTEGER freq;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	bool running = false;
};
#endif
