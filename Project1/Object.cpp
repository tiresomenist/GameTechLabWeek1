#include <vector>
#include "Object.h"


bool Object::Intersect(Object* otherObject)
{
	if (!otherObject)return false;

		if (this == otherObject) return false;
		float Dx = location.x - otherObject->location.x;
		float Dy = location.y - otherObject->location.y;
		float Dz = location.z - otherObject->location.z;

		float Distance = Dx * Dx + Dy * Dy + Dz * Dz;
		float TargetDistance = (radius + otherObject->radius);
		return Distance <= TargetDistance * TargetDistance;
}
FVertexSimple Object::GetLocation() {
	return location;
}
float Object::GetRadius() {
	return radius;
}

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

Player ::Player() {
		//크기 0.05f
		radius = 0.05f;
		location = { 0.0f, 0.0f, 0.0f };
};
bool Player::IsWall(float x, float y) {

	return (location.x+x >= -1 && location.x+x <= 1 && location.y+y >= -1 && location.y+y <= 1);
}
void Player::MoveObject(float x, float y) {
	if (IsWall(x, y)) {
		// 벽 충돌 처리 함수
	} else {
		location.x += x;
		location.y += y;
	}
}
float Player::GetAttackSpeed() {
	return attackSpeed;
}
float Player::GetAttackRange() {
	return attackRange;
}
