#pragma once
#include "Player.h"
#include "Weapon.h"

enum class AugmentType
{
	Attack,
	AttackRange,
	AttackSpeed,
	MoveSpeed,
	WeaponSize,
	HealHP
};

struct AugmentStruct {
	AugmentType type;
	const char* name;
	float value;
};

class Augment {

public:
	Augment();

	AugmentStruct GetAugmentStruct();
	
	void UpgradePlayer(AugmentStruct aug, Player *player);
	void ResetAugment();

private:
	AugmentStruct augStruct[6];
	std::vector<int> augNum = { 0,1,2,3,4,5 };
};