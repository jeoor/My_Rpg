#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
	public:
	Timer() = default;
	~Timer() = default;
	void start();
	void stop();
	void reset();
	double getElapsedTime() const; // 获取经过的时间，单位为秒
private:
	double startTime = 0.0; // 计时开始时间
	double endTime = 0.0;   // 计时结束时间
	bool running = false;   // 是否正在计时
};

#endif

