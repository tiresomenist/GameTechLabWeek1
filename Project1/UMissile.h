#pragma once
#include  "Object.h"

class UMissile : public Object
{
private:
	float lifetime = 0.0f;
	float duration = 10.0f;

public:
	UMissile(float _dmg, float _speed, FVector _moveDir);
	~UMissile() = default;

	virtual void MoveObject(float x, float y) override;
	void Reset(float _dmg, float _speed, FVector _moveDir);
	void UpdateLocation(float deltaTime);
	float GetDMG() { return dmg; };
	void AddLifetime(float deltaTime);
	float GetLifetime() {return lifetime;};
	float GetDuration() { return duration; };


	float dmg;
	float speed;
	FVector moveDir;
	bool isInActive = false;

};