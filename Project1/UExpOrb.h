#pragma once
#include "UItem.h"

class UExpOrb : public UItem
{
	int giveExp = 10;

	public:
	UExpOrb();
	~UExpOrb() = default;

	int GetGiveExp() { return giveExp; }



};