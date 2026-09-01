#pragma once

#include "FVector.h"
class Object
{
public:
	virtual ~Object() = default;
	float GetAttack();
	float GetSpeed();
	float GetHealth();
	float GetExp();
	float GetRadius();

	FVector GetLocation();

	bool GetIsDead();
	bool GetIsHit();
	bool GetIsMove();

	bool Intersect(Object* otherObject); // 충돌판정
	virtual void MoveObject(float x, float y) = 0 ; // 실제움직이기

	void DeadObject(); // 생사 여부

	void UpdateState(); //최종 업데이트

protected:
	float attack;
	float speed;
	float attackSpeed;
	float health;
	float exp;
	
	FVector location;
	float radius;

	int level;

	bool isDead;
	bool isHit;
	bool isMove;
};