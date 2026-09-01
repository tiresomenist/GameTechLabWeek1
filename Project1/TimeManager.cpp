#include <windows.h>
#include "TimeManager.h"

TimeManager* TimeManager::Ins = nullptr;

TimeManager::TimeManager()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&prevTime);
	currentTime = 0.0f;
	isRunning = false;
}

void TimeManager::TimeStart()
{
	if (isRunning == false)
		isRunning = true;
}

void TimeManager::TimeUpdate(float deltatime)
{
	if (isRunning == true) {
		currentTime += deltatime;
	}
}

void TimeManager::TimeReset()
{
	isRunning = false;
	currentTime = 0.0f;
}

void TimeManager::TimePause()
{
	if (isRunning == true) {
		isRunning = false;
	}
}

void TimeManager::TimeResume()
{
	if (isRunning == false) {
		isRunning = true;
	}
}

float TimeManager::GetcurrentTime()
{
	return currentTime;
}

bool TimeManager::IsRunning()
{
	return isRunning;
}

void TimeManager::Update()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	double actualDeltaTime = static_cast<double>(currentTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;
	targetFrameTime = 1.0 / targetFPS;


	if (actualDeltaTime < targetFrameTime)
	{
		double remainingTime = targetFrameTime - actualDeltaTime;

		Sleep(static_cast<DWORD>(remainingTime * 1000));

		QueryPerformanceCounter(&currentTime);
		deltaTime = static_cast<double>(currentTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;
	}
	else
	{
		deltaTime = actualDeltaTime;
	}

	
	if (deltaTime > 1 / 30.0f)
	{
		deltaTime = 1 / 30.0f;
	}

	prevTime = currentTime;
}




