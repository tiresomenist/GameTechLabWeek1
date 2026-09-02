#include <vector>
#include "Augment.h"

Augment::Augment() {
	augStruct[0] = { AugmentType::Attack,      "Attack",       10.0f };
	augStruct[1] = { AugmentType::AttackRange, "Attack Range", 0.05f };
	augStruct[2] = { AugmentType::AttackSpeed, "Attack Speed", 0.5f };
	augStruct[3] = { AugmentType::MoveSpeed,   "Move Speed",   1.0f };
	augStruct[4] = { AugmentType::WeaponSize,  "Weapon Size",  1.0f };
	augStruct[5] = { AugmentType::HealHP,      "Heal HP",      40.0f };
}

AugmentStruct Augment::GetAugmentStruct() {

	AugmentStruct aug;
	int index;

	index = rand() % augNum.size();
	aug = augStruct[augNum[index]];

	augNum.erase(augNum.begin() + index);

	return aug;
}

void Augment::ResetAugment() {
	augNum.clear();
	augNum.push_back(0);
	augNum.push_back(1);
	augNum.push_back(2);
	augNum.push_back(3);
	augNum.push_back(4);
	augNum.push_back(5);
}

void Augment::UpgradePlayer(AugmentStruct aug, Player* player) {
	switch (aug.type)
	{
	case AugmentType::Attack:
		// 공격력 증가
		player->IncreaseAttack(aug.value);
		break;

	case AugmentType::AttackRange:
		// 공격 범위 증가
		player->IncreaseAttackRange(aug.value);
		break;

	case AugmentType::AttackSpeed:
		// 공격 속도 증가
		player->IncreaseAttackSpeed(aug.value);
		break;

	case AugmentType::MoveSpeed:
		// 이동 속도 증가
		player->IncreaseMoveSpeed(aug.value);
		break;

	case AugmentType::WeaponSize:
		// 무기 크기 증가
		player->IncreaseWeaponSize(aug.value);
		break;

	case AugmentType::HealHP:
		// 체력 회복
		player->IncreaseHealHp(aug.value);
		break;
	}
	
}