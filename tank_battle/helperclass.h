#ifndef _HELPERCLASS_H
#define _HELPERCLASS_H

#include <vector>
#include <string>
using namespace std;

//计数基类
class Base_Cycle
{
public:
	virtual ~Base_Cycle();

	virtual bool ReachCycleStart(bool firstcallok = true) = 0;
};

//以调用函数次数为周期计数
class Time_Cycle :public Base_Cycle
{
	int m_cycle;
	int m_cyclecount;

public:
	Time_Cycle(int cycle);

	bool ReachCycleStart(bool firstcallok = true);
};

//以秒为周期计数
class Second_Cycle :public Base_Cycle
{
	float m_cycle;
	float m_lasttime;

public:
	Second_Cycle(float cycle);

	bool ReachCycleStart(bool firstcallok = true);
};

//内存日志类
class MemoryLogger
{
	static MemoryLogger* m_instance;

	vector<string> m_logs;

	MemoryLogger();

public:
	static MemoryLogger* GetInstance();

	void Log(const char* str);
	void GetLog(vector<string>& out);
	void Clear();
};

#endif