#pragma once
#include <vector>
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"

class ObjectManager
{
	std::vector<Object*>obejctList;
	//적만 존재하는 리스트
	std::vector<Enemy*> enemyList;

	std::vector<Weapon*> weaponList;

	void AddObject(Object* obj);
	void AddEnemy(Enemy* enemy);
	void AddWeapon(Weapon* weapon);

	float orbitAngle = 0.0f;

	public:
	void CreateEnemy(float difficulty);
	Player* CreatePlayer();
	void CreateWeapon();


	ObjectManager();
	void Render();
	void Update(float deltaTime);

	void EnemyMove(float deltaTime);
	void checkEnemiesIntersect();
	void checkPlayerIntersectWithEnemy();
	void intersectsPlayerWithWall();
	void SpinWeapon(float deltaTime, float rotationSpeed);
	bool isPlayerDead() {
		if (obejctList.size() > 0) {
			return obejctList[0]->IsDead();
		}
		return false;
	}
	void checkWeaponIntersectWithEnemy();


	
	static ObjectManager* Ins;

	static ObjectManager* GetInstance()  {
		if (Ins == nullptr) {
			Ins = new ObjectManager();
		}
		return Ins;
	}

	

	void ReleaseAllObjects();
	
};