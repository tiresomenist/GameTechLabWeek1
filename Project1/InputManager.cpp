#include <windows.h>
#include <WinUser.h>
#include "InputManager.h"

void InputManager::KeyDown(int key) {
	keys[key] = true;
}

void InputManager::KeyUp(int key) {
	keys[key] = false;
}

bool InputManager::IsKeyPressed(int key) {
	return keys[key];
}