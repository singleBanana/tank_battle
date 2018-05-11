#ifndef _HELPERCLASS_H
#define _HELPERCLASS_H

#include <vector>
#include <string>
using namespace std;

//��������
class Base_Cycle
{
public:
	virtual ~Base_Cycle();

	virtual bool ReachCycleStart(bool firstcallok = true) = 0;
};

//�Ե��ú�������Ϊ���ڼ���
class Time_Cycle :public Base_Cycle
{
	int m_cycle;
	int m_cyclecount;

public:
	Time_Cycle(int cycle);

	bool ReachCycleStart(bool firstcallok = true);
};

//����Ϊ���ڼ���
class Second_Cycle :public Base_Cycle
{
	float m_cycle;
	float m_lasttime;

public:
	Second_Cycle(float cycle);

	bool ReachCycleStart(bool firstcallok = true);
};

//�ڴ���־��
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