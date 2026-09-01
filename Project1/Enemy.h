#pragma once
#include "Object.h"

class Enemy : public Object
{
public:
	Enemy();
	virtual ~Enemy() = default; // = default 추가
	void MoveObject(float x, float y)override; // 실제움직이기
	bool IsDead()override;
	void InvincibleTimerUpdate(float deltaTime);
	
	bool isHit = false;

	float invinciblethreshold = 1.0f;
	float invincibleTimer = 0.0f;

	bool GetisHit() const
	{
		return isHit;
	}

	void SetisHit(bool value)
	{
		isHit = value;
	};

private:
	
};