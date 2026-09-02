#include <vector>
#include "Augment.h"
#include <string>
#include <cstdio>

Augment::Augment() {
	augStruct = {
	{ AugmentType::Attack,          "Damage",       300.0f, 600.0f },
	{ AugmentType::AttackRange,     "Attack Range", 50.0f, 70.0f },
	{ AugmentType::AttackSpeed,     "Attack Speed", 30.0f, 50.0f },
	{ AugmentType::MoveSpeed,        "Move Speed",   100.0f, 120.0f },
	{ AugmentType::WeaponSize,       "Weapon Size",  50.0f, 70.0f },
	{ AugmentType::HealHP,           "Heal HP",      40.0f, 100.0f },
	{ AugmentType::AddWeapon,        "Add Weapon",   1.0f, 3.0f },
	{ AugmentType::RocketAdd,         "***Add Rocket***",    1.0f, 1.0f },
	{ AugmentType::RocketSpeed,       "Rocket Speed",        20.0f, 40.0f },
	{ AugmentType::RocketDamage,      "Rocket Damage",       30.0f, 60.0f },
	{ AugmentType::RocketAttackSpeed, "Rocket Attack Speed", 20.0f, 40.0f }
	};

	ResetAugment();
}

std::string Augment::GetAugmentText(const AugmentStruct& aug) const {

	char text[50];

	if (aug.type == AugmentType::HealHP || aug.type == AugmentType::AddWeapon) {
		sprintf_s(text, "%s\n\n\n+%.0f", aug.name, aug.value);
	}
	else {
		sprintf_s(text, "%s\n\n\n+%.0f%%", aug.name, aug.value);
	}

	return text;
}

AugmentStruct Augment::GetAugmentStruct() {

	int index = rand() % availableAugments.size();

	AugmentStruct aug = availableAugments[index];

	float random = static_cast<float>(rand()) / RAND_MAX;

	if (random < enhanceChance) {
		aug.isEnhanced = true;
		aug.value = aug.enhenceValue;
	}

	availableAugments.erase(availableAugments.begin() + index);

	return aug;
}

//증강 풀 초기화
void Augment::ResetAugment() {

	availableAugments.clear();

	for (const auto& aug : augStruct)
	{
		availableAugments.push_back(aug);
	}
}