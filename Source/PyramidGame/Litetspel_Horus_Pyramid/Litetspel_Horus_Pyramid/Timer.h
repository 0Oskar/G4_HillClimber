#pragma once
#include<chrono>

class Timer
{
private:
	bool m_isRunning;
	std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	std::chrono::time_point<std::chrono::steady_clock> m_stopTime;
	int timeAdd;
public:
	Timer();
	double timeElapsed();
	bool start();
	bool stop();
	void restart();
	void addTime(int timeAdd);
	bool isActive();
	
};