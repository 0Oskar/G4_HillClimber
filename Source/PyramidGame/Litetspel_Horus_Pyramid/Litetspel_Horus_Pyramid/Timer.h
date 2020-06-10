#pragma once
#include<chrono>

class Timer
{
private:
	bool m_isRunning;
	std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	std::chrono::time_point<std::chrono::steady_clock> m_stopTime;
	int m_timeAdd;
	bool m_remeber;
	double m_resumeValue;
public:
	Timer(const bool remember = false);
	double timeElapsed() const;
	bool start();
	bool stop();
	void restart();
	void addTime(const int timeAdd);
	bool isActive() const;
	
};