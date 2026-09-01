#pragma once

class TimeManager
{
private:
	float currentTime = 0.0f;
	bool isRunning = false;
	float targetFPS = 30.0f;
	LARGE_INTEGER prevTime;
	LARGE_INTEGER frequency;
	float deltaTime = 0.0f;
	float targetFrameTime = 1.0f / targetFPS;

public:
	static TimeManager* Ins;
	TimeManager();
	void TimeStart();
	void TimeUpdate(float deltatime);
	void TimeReset();
	void TimePause();
	void TimeResume();
	float GetcurrentTime();
	bool IsRunning();
	void Update();
	static TimeManager* GetInstance() { return  Ins; }
	float GetDeltaTime() { return  deltaTime; }

};