#pragma once
#include "IGameStage.h"
#include "ObjectManager.h"
#include "TimeManager.h"

class InGameStage : public IGameStage
{
	void Enter() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;
	void intersects(Object* object);
	void intersectsToPlayer();
	std::vector<Object*>* objectList = nullptr;
	Player* player=nullptr;
	void CheckHitCollision(float AttackRange);
	void intersectsPlayerWithWall();
	TimeManager timeManager;

};


