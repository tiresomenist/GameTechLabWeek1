#pragma once
#include "UItem.h"

class UExpOrb : public UItem
{
	int giveExp = 10;
	int dir = 1;
	int limit = 3.141592f / 2.2f;
	public:
	UExpOrb();
	~UExpOrb() = default;

	int GetGiveExp() { return giveExp; }

	void UpdateState() override
	{
		
		//rotation.x += 0.05f;
		rotation.y += 0.03f * dir;
		if (rotation.y >= limit)
		{
			dir = -1;
		}
		else if (rotation.y <= -limit)
		{
			dir = 1;
		}
	}


};