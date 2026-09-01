#pragma once

class TimeManager
{
private:
	float currentTime = 0.0f;
	bool isRunning = false;

public:
	TimeManager();
	void TimeStart();
	void TimeUpdate(float deltatime);
	void TimeReset();
	void TimePause();
	void TimeResume();
	float GetcurrentTime();
	bool IsRunning();
};