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