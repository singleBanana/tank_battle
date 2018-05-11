#include "helperclass.h"

#include "gameplay.h"

Base_Cycle::~Base_Cycle()
{

}

Time_Cycle::Time_Cycle(int cycle)
{
	m_cycle = cycle;
	m_cyclecount = -1;
}

bool Time_Cycle::ReachCycleStart(bool firstcallok)
{
	bool ret = false;

	if (m_cyclecount < 0 && firstcallok) //判断第一次
	{
		m_cyclecount = 0;
		ret = true;
	}
	else
	{
		if (m_cyclecount == m_cycle - 1) //周期开始
		{
			m_cyclecount = 0;
			ret = true;
		}
		else
		{
			++m_cyclecount;
		}
	}

	return ret;
}

Second_Cycle::Second_Cycle(float cycle)
{
	m_cycle = cycle;
	m_lasttime = -1;
}

bool Second_Cycle::ReachCycleStart(bool firstcallok)
{
	bool ret = false;

	float et = GamePlay::GetInstance()->GetElapsedTime();
	if (m_lasttime < 0 && firstcallok) //判断第一次
	{
		m_lasttime = et;
		ret = true;
	}
	else
	{
		if (m_lasttime < 0)
		{
			m_lasttime = et;
		}
		if (et - m_lasttime > m_cycle) //周期开始
		{
			m_lasttime = et;
			ret = true;
		}
	}

	return ret;
}

MemoryLogger* MemoryLogger::m_instance = nullptr;

MemoryLogger::MemoryLogger()
{

}

MemoryLogger* MemoryLogger::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new MemoryLogger;
	}

	return m_instance;
}

void MemoryLogger::Log(const char* str)
{
	m_logs.push_back(str);
}

void MemoryLogger::GetLog(vector<string>& out)
{
	out = m_logs;
}

void MemoryLogger::Clear()
{
	m_logs.clear();
}