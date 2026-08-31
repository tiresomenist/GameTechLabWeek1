#pragma once
struct FVector
{
	float x, y, z;
	FVector(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
};
class Object
{
public:
	float GetAttack();
	float GetSpeed();
	float GetAttackSpeed();
	float GetHealth();
	float GetExp();
	float GetRadius();

	FVector GetLocation();

	bool GetIsDead();
	bool GetIsHit();
	bool GetIsMove();

	bool Intersect(Object* otherObject); // 충돌판정
	void MoveObject(float x, float y); // 실제움직이기

	void DeadObject(); // 생사 여부

	void UpdateState(); //최종 업데이트

private:
	float attack;
	float speed;
	float attackSpeed;
	float health;
	float exp;
	
	FVector location;
	float radius;

	int level;

	bool isDead;
	bool isHit;
	bool isMove;
};