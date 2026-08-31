class TimeManager
{
public:
	void TimeStart() {
		if (isRunning == false)
			isRunning = true;
	}

	void TimeUpdate(float deltatime) {
		if (isRunning == true) {
			currentTime += deltatime;
		}
	}

	void TimeReset() {
		isRunning = false;
		currentTime = 0.0f;
	}

	void TimePause() {
		if (isRunning == true) {
			isRunning = false;
		}
	}

	void TimeResume() {
		if (isRunning == false) {
			isRunning = true;
		}
	}

	float GetCurrentTime() {
		return currentTime;
	}

	bool IsRunning() {
		return isRunning;
	}

private:
	float currentTime = 0.0f;
	bool isRunning = false;

};