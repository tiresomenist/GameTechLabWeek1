#pragma once
#include "Object.h"

class Enemy : public Object
{
public:
	Enemy(float difficulty);
	virtual ~Enemy() = default; // = default 추가
	void MoveObject(float x, float y)override; // 실제움직이기
	bool IsDead()override;
	void InvincibleTimerUpdate(float deltaTime);

	void UpdateState();
	
	bool isHit = false;

	float invinciblethreshold = 1.0f;
	float invincibleTimer = 0.0f;

	float hitFlashAmount = 0.0f;
	float flashDuration = 0.1f;       // 흰색이 사라지는 시간

	void Reset(float difficulty);


	bool pendingRemove = false; // 제거 예정 여부

	bool GetisHit() const
	{
		return isHit;
	}

	void SetisHit(bool value)
	{
		isHit = value;
	};

	bool GetPendingRemove() const
	{
		return pendingRemove;
	}

	void SetPendingRemove(bool value)
	{
		pendingRemove = value;
	}

	void SetHitFlashAmount(float amount)
	{
		hitFlashAmount = amount;
	}

	float GetHitFlashAmount() const
	{
		return hitFlashAmount;
	}

private:
	
};