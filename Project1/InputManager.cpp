#include <WinUser.h>
class InputManager 
{
public:

	void KeyDown(int key) {
		keys[key] = true;
	}

	void KeyUp(int key) {
		keys[key] = false;
	}

	bool IsKeyPressed(int key) {
		return keys[key];
	}

private:
	bool keys[256] = {};

};

