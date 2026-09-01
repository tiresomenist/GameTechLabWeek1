#include "Enemy.h"
#include <vector>



Enemy::Enemy() {
	//크기 0.03f
	radius = 0.03f;
	switch (rand() % 8) {
	case 0: location = { -0.9f, 0.0f, 0.0f }; break;
	case 1: location = { -0.9f, -0.9f, 0.0f }; break;
	case 2: location = { 0.0f, -0.9f, 0.0f }; break;
	case 3: location = { 0.9f, -0.9f, 0.0f }; break;
	case 4: location = { 0.9f, 0.0f, 0.0f }; break;
	case 5: location = { 0.9f, 0.9f, 0.0f }; break;
	case 6: location = { -0.9f, 0.9f, 0.0f }; break;
	case 7: location = { 0.0f, 0.9f, 0.0f }; break;
	}
};
void Enemy::MoveObject(float x, float y) {
	location.x += x;
	location.y += y;
}