#include <vector>
#include "Augment.h"
#include <string>
#include <cstdio>

Augment::Augment() {
	augStruct[0] = { AugmentType::Attack,      "Attack",       300.0f };
	augStruct[1] = { AugmentType::AttackRange, "Attack Range", 50.0f };
	augStruct[2] = { AugmentType::AttackSpeed, "Attack Speed", 30.0f };
	augStruct[3] = { AugmentType::MoveSpeed,   "Move Speed",   100.0f };
	augStruct[4] = { AugmentType::WeaponSize,  "Weapon Size",  50.0f };
	augStruct[5] = { AugmentType::HealHP,      "Heal HP",      40.0f };
	augStruct[6] = { AugmentType::AddWeapon,   "Add Weapon",   1.0f };

	ResetAugment();
}

std::string Augment::GetAugmentText(const AugmentStruct& aug) {

	char text[50];

	if (aug.type == AugmentType::HealHP || aug.type == AugmentType::AddWeapon) {
		sprintf_s(text, "%s\n+%.0f", aug.name, aug.value);
	}
	else {
		sprintf_s(text, "%s\n+%.0f%%", aug.name, aug.value);
	}

	return text;
}

AugmentStruct Augment::GetAugmentStruct() {

	AugmentStruct aug;
	int index;

	index = rand() % augNum.size();
	aug = augStruct[augNum[index]];

	augNum.erase(augNum.begin() + index);

	return aug;
}

//증강 풀 초기화
void Augment::ResetAugment() {
	augNum.clear();

	for(int i=0;i< static_cast<int>(AugmentType::Count);i++)
		augNum.push_back(i);
}