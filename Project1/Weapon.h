#pragma once
#include "Object.h"

class Weapon : public Object
{
public:
	Weapon(float _radius);
	~Weapon() = default;

	virtual void MoveObject(float x, float y) override;

	void UpdateOrbit(float deltaTime, float rotationSpeed, float angle, const FVector& playerLocation, float orbitRadius);

	void UpdateState() override;


};