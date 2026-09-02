#pragma once
#include <vector>
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Augment.h"
#include "FHitEffect.h"

class ObjectManager
{
	static ObjectManager* Ins;
	std::vector<Object*>objectList;
	//적만 존재하는 리스트
	std::vector<Enemy*> enemyList;

	std::vector<Weapon*> weaponList;

	std::vector<FHitEffect> hitEffects;

	void AddObject(Object* obj);
	void AddEnemy(Enemy* enemy);
	void AddWeapon(Weapon* weapon);
	float orbitAngle = 0.0f;
	int killCount = 0;
	float hitFlashAlpha = 1.0f;

	public:
	void CreateEnemy(float difficulty);
	Player* CreatePlayer();
	void CreateWeapon(float radius);
	void CreateExpOrb(float x, float y);

	ObjectManager();
	void Render();
	void RenderMainMenu();
	void Update(float deltaTime);

	void EnemyMove(float deltaTime);
	void checkEnemiesIntersect();
	void checkPlayerIntersectWithEnemy();
	void intersectsPlayerWithWall();
	void SpinWeapon(float deltaTime, float rotationSpeed);
	void SpinWeaponMainMenu(float deltaTime, float rotationSpeed);
	bool isPlayerDead() {
		if (objectList.size() > 0) {
			return objectList[0]->IsDead();
		}
		return false;
	}
	void checkWeaponIntersectWithEnemy();
	void checkPlayerIntersectWithExpOrb();


	
	ID3D11ShaderResourceView* m_lastBoundSRV = nullptr;

	static ObjectManager* GetInstance()  {
		if (Ins == nullptr) {
			Ins = new ObjectManager();
		}
		return Ins;
	}
	void AddPlayerExp(int x);
	bool IsPlayerLevelUp();
	void UpgradePlayer(AugmentStruct aug);

	void ReleaseAllObjects();

	void setKillCount(int count) { killCount = count; }
	int  getKillCount() const { return killCount; }

	float GetHitFlashAlpha() const { return hitFlashAlpha; }
	
};