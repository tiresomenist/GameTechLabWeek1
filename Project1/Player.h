#pragma once
#include "Object.h"
#include "TimeManager.h"

class Player : public Object
{
public:
	Player();
	virtual ~Player() = default; // = default 추가
	bool IsWall(float x, float y);
	float GetAttackSpeed();
	float GetAttackRange();
	void MoveObject(float x, float y)override; // 실제움직이기
	float GetWeaponRotationSpeed() const { return WeaponRotationSpeed; } // 무기 회전 속도 반환	
	float GetOrbitRadius() const { return orbitRadius; } // 무기 궤도 반지름 반환
	void GetAttacked(float damage);
	bool IsDead()override;
	float hittedTime = 0.0f;
	float MaxHealth = 200.0f;
	float GetMaxHealth();

private:
	float WeaponRotationSpeed = 6.0f;
	float orbitRadius = 0.2f;
	

};