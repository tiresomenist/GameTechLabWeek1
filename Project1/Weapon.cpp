#include "Weapon.h"
#include <cmath>

const float PI = 3.14159265358979323846f;
Weapon::Weapon(float _radius)
{
	radius = _radius;
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
void Weapon::UpdateOrbitMainMenu(
	float angle,
	const FVector& playerLocation,
	float orbitRadius)
{
	float orbitRadiusX = orbitRadius * 5.0f;
	float orbitRadiusY = orbitRadius * 5.0f * 0.15f;

	// 지구 중심 기준 달의 타원 궤도 위치
	float x = cosf(angle) * orbitRadiusX;
	float y = sinf(angle) * orbitRadiusY;

	// 궤도 기울이기
	float tilt = 0.25f; // 라디안, 약 14도

	float rotatedX =
		x * cosf(tilt) - y * sinf(tilt);

	float rotatedY =
		x * sinf(tilt) + y * cosf(tilt);

	// 실제 위치
	location.x = playerLocation.x + rotatedX;
	location.y = playerLocation.y + rotatedY;
}

void Weapon::UpdateState()
{
	rotation.x += 0.05f;
	rotation.y += 0.05f;
}

void Weapon::UpdateState(float deltaTime)
{
	rotation.y -= PI * 0.382f * deltaTime;
}
