#include <vector>
#include "Object.h"


bool Object::Intersect(Object* otherObject)
{
	if (!otherObject)return false;

		if (this == otherObject) return false;
		float Dx = location.x - otherObject->location.x;
		float Dy = location.y - otherObject->location.y;
		float Dz = location.z - otherObject->location.z;

		float Distance = Dx * Dx + Dy * Dy + Dz * Dz;
		float TargetDistance = (radius + otherObject->radius);
		return Distance <= TargetDistance * TargetDistance;
}

FVector Object::GetLocation() 
{
	return location;
}
Object::Object()
{
	renderer = new URenderer();



}
float Object::GetRadius() {
	return radius;
}

float Object::GetSpeed() {
	return speed;
}

void Object::SetLocation(float x,float y) {
	location.x = x;
	location.y = y;
}

void Object::GetAttacked(float damage) {
	health -= damage;

	char debugMessage[50];
	sprintf_s(debugMessage, "%.0f %.0f\n", location.x,location.y);
	OutputDebugStringA(debugMessage);
	if (health <= 0) { isDead = true; 
	char debugMessage[50];
	sprintf_s(debugMessage, "Enemy Hit! HP: %.0f\n", health);
	OutputDebugStringA(debugMessage);
	}
}

bool Object::IsDead() {
	return isDead;
}
float Object::GetAttack() {
	return attack;
}
float Object::GetHealth() {
	return health;
}