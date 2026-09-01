#pragma once

class InputManager
{
public:
	InputManager();
	void KeyDown(int key);
	void KeyUp(int key);
	bool IsKeyPressed(int key);
	static InputManager* IMins;
	static InputManager* GetInstance() {
		if (IMins == nullptr) {
			IMins = new InputManager();
		}
		return IMins;
	}
private:
	bool keys[256] = {};

};
