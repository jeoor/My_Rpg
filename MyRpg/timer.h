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
	double getElapsedTime() const; // ��ȡ������ʱ�䣬��λΪ��
private:
	double startTime = 0.0; // ��ʱ��ʼʱ��
	double endTime = 0.0;   // ��ʱ����ʱ��
	bool running = false;   // �Ƿ����ڼ�ʱ
};

#endif

