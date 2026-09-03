#pragma once

class InputManager
{
public:
	InputManager();
	void KeyDown(int key);
	void KeyUp(int key);
	bool IsKeyPressed(int key);
	bool IsKeyTriggered(int key);
	void KeyReset();
	static InputManager* IMins;
	static InputManager* GetInstance() {
		if (IMins == nullptr) {
			IMins = new InputManager();
		}
		return IMins;
	}
private:
	bool keys[256] = {};
	bool keyTriggered[256] = {};

};
