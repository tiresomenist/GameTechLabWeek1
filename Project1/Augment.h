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
	RocketAdd,
	RocketSpeed,
	RocketDamage,
	RocketAttackSpeed,
	Count
};

struct AugmentStruct {
	AugmentType type;
	const char* name;
	float value;
	float enhenceValue;
	float weight;
	bool isEnhanced;
};

class Augment {

public:
public:
	Augment();
	AugmentStruct GetAugmentStruct();
	std::string GetAugmentText(const AugmentStruct& aug) const;
	void ResetAugment(bool hasRocket);

private:
	std::vector<AugmentStruct> augStruct;
	std::vector<AugmentStruct> availableAugments;

	// 강화 증강
	bool isEnhanced = false;
	float enhanceChance = 0.5f;

	// 로켓 증강
	bool hasRocket = false;


};

