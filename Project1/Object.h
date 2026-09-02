#pragma once

#include "FVertexSimple.h"
#include "FVector.h"
#include "FConstant.h"

#include "URenderer.h"
class Object
{
public:
	Object();
	virtual ~Object() = default;
	float GetAttack();
	float GetSpeed();
	float GetHealth();
	float GetExp();
	float GetRadius();
	FVector GetLocation();
	FVector GetRotation();

	bool GetIsDead();
	bool GetIsHit();
	bool GetIsMove();

	bool Intersect(Object* otherObject); // 충돌판정
	virtual void MoveObject(float x, float y) = 0 ; // 실제움직이기

	void GetAttacked(float damage);
	virtual bool IsDead();

	void IncreaseRadius(float x);

	virtual void UpdateState(); //최종 업데이트
	void SetLocation(float x,float y);
	URenderer* renderer;

	void SetDepth(float d) { depth = d; }
	float GetDepth() const { return depth; }


protected:
	float attack;
	float speed;
	float attackSpeed;
	float health;
	float depth = 0.0f;
	
	FVector location;	
	float radius;
	FVector rotation = (0.0f, 0.0f, 0.0f);

	int level;

	bool isDead;
	bool isHit;
	bool isMove;
};