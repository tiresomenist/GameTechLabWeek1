#include <vector>
#include "Augment.h"
#include <string>
#include <cstdio>

Augment::Augment() {
	augStruct = {
	{ AugmentType::Attack,          "Damage",       300.0f, 600.0f, 10.0f },
	{ AugmentType::AttackRange,     "moon orbit range", 50.0f, 70.0f, 10.0f },
	{ AugmentType::AttackSpeed,     "spin speed",       30.0f, 50.0f, 10.0f },
	{ AugmentType::MoveSpeed,        "Move Speed",   100.0f, 120.0f, 10.0f },
	{ AugmentType::WeaponSize,       "moon Size",  50.0f, 70.0f, 10.0f },
	{ AugmentType::HealHP,           "Heal HP",    40.0f, 100.0f, 10.0f },
	{ AugmentType::AddWeapon,        "Add moon",   1.0f, 3.0f, 50.0f },
	{ AugmentType::RocketAdd,         "***Add Rocket***",    0.0f, 0.0f, 3.0f },
	{ AugmentType::RocketSpeed,       "Rocket Speed",        20.0f, 40.0f, 20.0f },
	{ AugmentType::RocketDamage,      "Rocket Damage",       30.0f, 60.0f, 20.0f },
	{ AugmentType::RocketAttackSpeed, "Rocket Attack Speed", 30.0f, 50.0f, 20.0f }
	};
}

std::string Augment::GetAugmentText(const AugmentStruct& aug) const {

	char text[50];

	if (aug.type == AugmentType::HealHP || aug.type == AugmentType::AddWeapon) {
		sprintf_s(text, "%s\n\n\n+%.0f", aug.name, aug.value);
	}
	else if (aug.type == AugmentType::RocketAdd){
		sprintf_s(text, "%s\n\n\n", aug.name);

	} else {
		sprintf_s(text, "%s\n\n\n+%.0f%%", aug.name, aug.value);
	}

	return text;
}

AugmentStruct Augment::GetAugmentStruct() {

	int index = rand() % availableAugments.size();

	AugmentStruct aug = availableAugments[index];


	float totalWeight = 0.0f;

	for (const auto& aug : availableAugments) {
		totalWeight += aug.weight;
	}

	float randomWeight = (static_cast<float>(rand()) / RAND_MAX) * totalWeight;

	float currentWeight = 0.0f;

	for (int i = 0; i < availableAugments.size(); i++) {

		currentWeight += availableAugments[i].weight;

		if (randomWeight <= currentWeight)
		{
			AugmentStruct aug = availableAugments[i];

			float random = static_cast<float>(rand()) / RAND_MAX;

			if (random < enhanceChance)
			{
				aug.isEnhanced = true;
				aug.value = aug.enhenceValue;
			}

			availableAugments.erase(availableAugments.begin() + i);
			return aug;
		}
	}

	
}

//증강 풀 초기화
void Augment::ResetAugment(bool hasRocket) {

	availableAugments.clear();

	for (const auto& aug : augStruct)
	{	
		// 로켓이 없으면 Rocket 증강 제외
		if (!hasRocket &&
			(aug.type == AugmentType::RocketSpeed ||
				aug.type == AugmentType::RocketDamage ||
				aug.type == AugmentType::RocketAttackSpeed))
		{
			continue;
		}

		// 이미 로켓이 있으면 Add Rocket 제외
		if (hasRocket && aug.type == AugmentType::RocketAdd)
		{
			continue;
		}

		availableAugments.push_back(aug);
	}
}