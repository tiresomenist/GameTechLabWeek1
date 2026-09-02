#include "Enemy.h"
#include <vector>



Enemy::Enemy(float difficulty) 
{
	Reset(difficulty);
};

void Enemy::Reset(float difficulty)
{
	pendingRemove = false;
	radius = 0.03f;
	isHit = false;
	invincibleTimer = 0.0f;
	hitFlashAmount = 0.0f;
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
	speed = 0.2f + (0.05f * difficulty);
	attack = 1.0f + (0.05f * difficulty);
	float MaxHealth = 30.0f + (30.0f * difficulty);
	health = MaxHealth;
}

void Enemy::MoveObject(float x, float y) {
	location.x += x;
	location.y += y;
}

bool Enemy::IsDead() {
	return isDead;
}

void Enemy::InvincibleTimerUpdate(float deltaTime)
{
	if(isHit)
	{
		invincibleTimer += deltaTime;
		hitFlashAmount = 1.0f - (invincibleTimer / flashDuration);
		if (invincibleTimer >= invinciblethreshold)
		{
			isHit = false;
			invincibleTimer = 0.0f;
			hitFlashAmount = 0.0f;
		}
		if (invincibleTimer >= flashDuration)
		{
			hitFlashAmount = 0.0f;
		}
	}

}
void Enemy::UpdateState()
{
	rotation.x += 0.05f;
	rotation.y += 0.05f;
}