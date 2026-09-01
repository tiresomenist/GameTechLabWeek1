#include "Object.h"

struct FVector {
	float x, y, z;
};

class Player : public Object
{


public:
	float GetAttack();
	float GetSpeed();
	float GetAttackSpeed();
	float GetHealth();
	float GetExp();
	float GetAttackRange();


	FVector GetLocation();
	float radius();

	int GetLevel();

	bool GetIsDead();
	bool GetIsHit();
	bool GetIsMove();

	bool Intersect(Object* otherObject); // 충돌판정
	void IsMoveObject(); // 실제움직이기
	void IsDeadObject(); // 생사 여부
	void UpdateState(); //최종 업데이트

private:
	float attack;
	float speed;
	float attackSpeed;
	float health;
	float exp;
	float attackRange;

	FVector location;
	float radius;

	int level;

	bool isDead;
	bool isHit;
	bool isMove;
};