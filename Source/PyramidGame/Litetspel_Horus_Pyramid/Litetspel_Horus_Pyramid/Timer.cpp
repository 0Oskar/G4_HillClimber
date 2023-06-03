#include"pch.h"
#include"Timer.h"

using namespace std::chrono;
Timer::Timer(const bool remember)
{
	m_startTime = high_resolution_clock::now();
	m_stopTime = high_resolution_clock::now();
	m_isRunning = false;
	m_timeAdd = 0;
	m_remeber = remember;
	m_resumeValue = 0;
}
double Timer::timeElapsed() const
{
	if (m_isRunning)
	{
		auto elapsed = std::chrono::duration<double>(high_resolution_clock::now() - m_startTime);
		return elapsed.count() + m_timeAdd;
	}
	else
	{
		auto elapsed = std::chrono::duration<double>(m_stopTime - m_startTime);
		return elapsed.count() + m_timeAdd;
	}
}
bool Timer::start()
{
	bool canStart = true;
	if (m_isRunning)
		canStart = false;
	else
	{
		if (m_remeber)
		{
			addTime((int)m_resumeValue);
		}
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
		if (m_remeber)
		{
			auto time = std::chrono::duration_cast<std::chrono::seconds>(high_resolution_clock::now() - m_startTime);
			m_resumeValue = (double)time.count();
		}
		m_stopTime = high_resolution_clock::now();
		m_isRunning = false;
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

void Timer::addTime(const int timeAdd)
{
	m_timeAdd += timeAdd;
}

bool Timer::isActive() const
{
	return m_isRunning;
}
