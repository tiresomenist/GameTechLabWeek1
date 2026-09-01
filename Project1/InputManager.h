#pragma once

class InputManager
{
public:
	void KeyDown(int key);
	void KeyUp(int key);
	bool IsKeyPressed(int key);

private:
	bool keys[256] = {};
};
