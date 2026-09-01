#include "Player.h"
#include <vector>

Player::Player() {
	//크기 0.05f
	radius = 0.05f;
	location = { 0.0f, 0.0f, 0.0f };
};
bool Player::IsWall(float x, float y) {

	return (location.x + x >= -1 && location.x + x <= 1 && location.y + y >= -1 && location.y + y <= 1);
}
void Player::MoveObject(float x, float y) {
	if (IsWall(x, y)) {
		// 벽 충돌 처리 함수
	}
	else {
		location.x += x;
		location.y += y;
	}
}
