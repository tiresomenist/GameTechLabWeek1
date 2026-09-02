#include "Player.h"
#include <vector>

Player::Player() {
	//크기 0.05f
	radius = 0.05f;
	location = FVector(0.0f, 0.0f, 0.0f);
	health = MaxHealth;
	speed = 0.4f;
	attack = 100.0f;
};
bool Player::IsWall(float x, float y) {

	return (location.x + x < -1 || location.x + x > 1 || location.y + y < -1 || location.y + y > 1);
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
void Player::GetAttacked(float damage) {
	float thisTime = TimeManager::GetInstance()->GetcurrentTime();
	hittedTime = thisTime;
	health -= damage;

	if (health <= 0) isDead = true;
}
bool Player::IsDead() {
	return isDead;
}
float Player::GetMaxHealth() {
	return MaxHealth;
}

int Player::GetLevel() {
	return level;
}

int Player::GetExp() {
	return exp;
}

int Player::GetExpTable() {
	return expTable[level];
}

void Player::AddExp(int x) {
	if(level != maxLevel)
		exp += x;
}

bool Player::IsLevelUp() {

	if (level == maxLevel)
		return false;

	if (exp >= expTable[level]) {

		if (level == maxLevel - 1) {
			level++;
			return true;
		}
		else {
			exp -= expTable[level];
			level++;
			return true;
		}
		
	}

	return false;
}

void Player::IncreaseAttack(float x) {
	attack = attack * (1.0f + x / 100.0f);
}

void Player::IncreaseAttackRange(float x) {
	orbitRadius = orbitRadius * (1.0f + x / 100.0f);
}

void Player::IncreaseAttackSpeed(float x) {
	WeaponRotationSpeed = WeaponRotationSpeed * (1.0f + x / 100.0f);
}

void Player::IncreaseMoveSpeed(float x) {
	speed = speed * (1.0f + x / 100.0f);
}

void Player::IncreaseHealHp(float x) {
	health += x;

	if (health > MaxHealth)
		health = MaxHealth;
}

