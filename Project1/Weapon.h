#pragma once
#include "Object.h"

class Weapon : public Object
{
public:
	Weapon(float _radius);
	~Weapon() = default;

	void MoveObject(float x, float y) override;

	void UpdateOrbit(float deltaTime, float rotationSpeed, float angle, const FVector& playerLocation, float orbitRadius);

	void UpdateOrbitMainMenu(float angle, const FVector& playerLocation, float orbitRadius);

	void UpdateState() override;
	void UpdateState(float deltaTime);


};