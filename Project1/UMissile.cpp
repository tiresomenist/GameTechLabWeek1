#include "UMissile.h"

UMissile::UMissile(float _dmg,float _speed,FVector _moveDir)
{
	dmg = _dmg;
	speed = _speed;
	moveDir = _moveDir;
	radius = 0.1f;
	lifetime = 0.0f;
}

void UMissile::MoveObject(float x, float y)
{
}

void UMissile::Reset(float _dmg, float _speed, FVector _moveDir)
{
	dmg = _dmg;
	speed = _speed;
	moveDir = _moveDir;
	isInActive = false;
	lifetime = 0.0f;
}

void UMissile::UpdateLocation(float deltaTime)
{
	location.x += moveDir.x * speed * deltaTime;
	location.y += moveDir.y * speed * deltaTime;
}

void UMissile::AddLifetime(float deltaTime)
{
	lifetime += deltaTime;

}
