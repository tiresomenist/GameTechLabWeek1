#include <windows.h>
#include <WinUser.h>
#include "InputManager.h"

InputManager* InputManager::IMins = nullptr;

InputManager::InputManager() {
	IMins = this;
}

void InputManager::KeyDown(int key) {
	if (!keys[key]) {
		keyTriggered[key] = true;
	}

	keys[key] = true;
}

void InputManager::KeyUp(int key) {
	keys[key] = false;
}

bool InputManager::IsKeyPressed(int key) {
	return keys[key];
}

bool InputManager::IsKeyTriggered(int key) {
	if (keyTriggered[key]) {

		keyTriggered[key] = false;
		return true;
	}

	return false;
}

void InputManager::KeyReset() {
	memset(keys, 0, sizeof(keys));
	memset(keyTriggered, 0, sizeof(keyTriggered));
}