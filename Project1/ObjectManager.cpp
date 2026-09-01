#include "ObjectManager.h"

ObjectManager* ObjectManager::Ins = nullptr;

void ObjectManager::AddEnemy(Enemy* enemy)
{
	enemyList.push_back(enemy);
}

void ObjectManager::AddObject(Object* obj)
{
	obejctList.push_back(obj);
	
}


void ObjectManager::CreateEnemy()
{
	Enemy* newEnemy = new Enemy();
	AddEnemy(newEnemy);
}

Player* ObjectManager::CreatePlayer()
{
	Player* newPlayer = new Player();	
	AddObject(newPlayer);
	return newPlayer;
}



ObjectManager::ObjectManager()
{
	Ins = this;
}
void ObjectManager::Render()
{
	if (obejctList.size() > 0)
		for (auto object : obejctList)
		{
			object->renderer->RenderPrimitive(object->GetLocation(),object->GetRadius());
		}


}
;

