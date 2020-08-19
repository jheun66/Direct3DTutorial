#include "Framework.h"

Timer* Timer::instance = nullptr;

Timer::Timer()
	:curTicks(0), lastFPSUpdate(0),
	frameCount(0), runningTime(0), framePerSecond(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond);
	// Ƣ�°� ����?
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTicks);

	// ������ ���� ���̱�����
	tickCount = 1.0f / ticksPerSecond;
}

Timer::~Timer()
{

}

void Timer::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&curTicks);
	timeElapsed = (float)(curTicks - lastTicks) * tickCount;
	runningTime += timeElapsed;

	frameCount++;
	if (curTicks - lastFPSUpdate >= ticksPerSecond)
	{
		float tempCurTime = (float)curTicks * tickCount;
		float tempLastTime = (float)lastFPSUpdate * tickCount;
		framePerSecond = (float)frameCount / (tempCurTime - tempLastTime);

		lastFPSUpdate = curTicks;
		frameCount = 0;
	}

	lastTicks = curTicks;
}
