#include "Player.h"
#include <vector>

Player::Player() {
	//크기 0.05f
	radius = 0.05f;
	location = FVector(0.0f, 0.0f, 0.0f);
	speed = 0.6f;
};
bool Player::IsWall(float x, float y) {

	return (location.x + x < -1 + radius || location.x + x > 1 - radius || location.y + y < -1 + radius || location.y + y > 1 - radius);
}
void Player::MoveObject(float x, float y) {

	if (IsWall(x, y)) 
	{
		// 벽 충돌 처리 함수1
	}
	else {
		location.x += x;
		location.y += y;
	}
}
float Player::GetAttackSpeed() {
	return attackSpeed;
}
float Player::GetAttackRange() {
	return 0.08f; // 공격 범위는 플레이어의 반지름의 두 배로 설정
}
float Player::GetSpeed() {
	return this->speed;
}