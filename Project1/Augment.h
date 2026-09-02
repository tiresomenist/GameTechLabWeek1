#pragma once
#include "Player.h"
#include "Weapon.h"
#include <string>


enum class AugmentType
{
	Attack,        // 공격력 
	AttackRange,   // 공격범위
	AttackSpeed,   // 공격속도
	MoveSpeed,     // 이동속도
	WeaponSize,    // 무기크기
	HealHP,        // 체력회복
	AddWeapon,     // 무기추가
	Count
};

struct AugmentStruct {
	AugmentType type;
	const char* name;
	float value;
};

class Augment {

public:
public:
	Augment();
	AugmentStruct GetAugmentStruct();
	std::string GetAugmentText(const AugmentStruct& aug) const;
	void ResetAugment();

private:
	AugmentStruct augStruct[static_cast<int>(AugmentType::Count)];
	std::vector<int> augNum;
};

