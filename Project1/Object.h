#pragma once
class Object
{
public:
	float GetAttack();
	float GetSpeed();
	float GetAttackSpeed();
	float GetHealth();
	float GetExp();

	bool GetIsDead();
	bool GetIsHit();
	bool GetIsMove();

	bool Intersect(Object* otherObject); // 충돌판정
	void MoveObject(); // 실제움직이기

	void DeadObject(); // 생사 여부
	
	void UpdateState(); //최종 업데이트

private:
	float attack;
	float speed;
	float attackSpeed;
	float health;
	float exp;

	int level;

	bool isDead;
	bool isHit;
	bool isMove;
};