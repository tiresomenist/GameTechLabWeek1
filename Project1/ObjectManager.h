#pragma once
#include <vector>
#include "Object.h"
#include "Player.h"
#include "Enemy.h"

class ObjectManager
{
	std::vector<Object*>obejctList;

	void AddObject(Object* obj);
	
};