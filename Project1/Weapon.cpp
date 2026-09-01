#include "Weapon.h"
#include <cmath>
Weapon::Weapon()
{
	radius = 0.02f;
	location = FVector(0.0f, 0.0f, 0.0f);
}

void Weapon::MoveObject(float x, float y)
{
}

void Weapon::UpdateOrbit(float deltaTime, float rotationSpeed, float angle,const FVector& playerLocation, float orbitRadius)
{
	location.x = playerLocation.x + cosf(angle) * orbitRadius;
	location.y = playerLocation.y + sinf(angle) * orbitRadius;


}
