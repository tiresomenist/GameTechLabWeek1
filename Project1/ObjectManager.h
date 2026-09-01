#pragma once
#include <vector>
#include "Object.h"
#include "Player.h"
#include "Enemy.h"

class ObjectManager
{
	std::vector<Object*>obejctList;
	//적만 존재하는 리스트
	std::vector<Enemy*> enemyList;

	
	void AddObject(Object* obj);
	void AddEnemy(Enemy* enemy);

	public:
	void CreateEnemy();
	Player* CreatePlayer();
	ObjectManager();
	void Render();
	void Update(float deltaTime);

	void EnemyMove(float deltaTime);
	void checkEnemiesIntersect();
	void checkPlayerIntersectWithEnemy();
	void intersectsPlayerWithWall();
	bool isPlayerDead() {
		if (obejctList.size() > 0) {
			return obejctList[0]->IsDead();
		}
		return false;
	}
	
	static ObjectManager* Ins;

	static ObjectManager* GetInstance()  {
		if (Ins == nullptr) {
			Ins = new ObjectManager();
		}
		return Ins;
	}

	

	void ReleaseAllObjects();
	
};