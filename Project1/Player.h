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
	void ScalePlayerRadius(float scale) { radius = 0.05f * scale; } // 플레이어의 반지름 배율 조정

	int GetLevel();
	int GetExp();
	int GetExpTable();
	void AddExp(int x);
	bool IsLevelUp();
	float GetWeaponRadius() const { return weaponRadius; } // 플레이어의 무기 반지름 반환

	void IncreaseAttack(float x);
	void IncreaseAttackRange(float x);
	void IncreaseAttackSpeed(float x);
	void IncreaseMoveSpeed(float x);
	void IncreaseHealHp(float x);
	void SetWeaponRadius(float _r) { weaponRadius = _r; } // 플레이어의 무기 반지름 설정
	void UpdateState() override; // 플레이어 상태 업데이트

private:
	float WeaponRotationSpeed = 6.0f;
	float orbitRadius = 0.2f;
	
	int exp = 0;
	int level = 1;
	int maxLevel = 10;
	int expTable[11] = { 0, 10, 20, 20, 20, 204, 205, 206, 207, 208, 208};
	float weaponRadius = 0.02f;
};