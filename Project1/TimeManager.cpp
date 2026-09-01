#include <windows.h>
#include "TimeManager.h"

TimeManager::TimeManager()
{
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
