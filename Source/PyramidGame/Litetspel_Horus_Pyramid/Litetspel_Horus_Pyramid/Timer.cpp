#include"pch.h"
#include"Timer.h"

using namespace std::chrono;
Timer::Timer()
{
	this->m_startTime = high_resolution_clock::now();
	this->m_stopTime = high_resolution_clock::now();
	m_isRunning = false;
	this->timeAdd = 0;
}
double Timer::timeElapsed()
{
	if (this->m_isRunning)
	{
		auto elapsed = std::chrono::duration<double>(high_resolution_clock::now() - m_startTime);
		return elapsed.count() + this->timeAdd;
	}
	else
	{
		auto elapsed = std::chrono::duration<double>(m_stopTime - m_startTime);
		return elapsed.count() + this->timeAdd;
	}
}
bool Timer::start()
{
	bool canStart = true;
	if (m_isRunning)
		canStart = false;
	else
	{
		m_startTime = high_resolution_clock::now();
		m_isRunning = true;
	}

	return canStart;
}
bool Timer::stop()
{
	bool canStop = true;
	if (m_isRunning)
	{
		m_stopTime = high_resolution_clock::now();
	}
	else
		canStop = false;
	
	return canStop;
}
void Timer::restart()
{
	m_isRunning = true;
	m_startTime = std::chrono::high_resolution_clock::now();
}

void Timer::addTime(int timeAdd)
{
	this->timeAdd += timeAdd;
}

bool Timer::isActive()
{
	return this->m_isRunning;
}
