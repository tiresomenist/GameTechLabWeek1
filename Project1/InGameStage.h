#pragma once
#include "IGameStage.h"
#include "ObjectManager.h"
#include "TimeManager.h"

class App;

class InGameStage : public IGameStage
{
public:
    InGameStage(App* app);

    void Enter() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Exit() override;

    void intersects(Object* object);
    void intersectsToPlayer();

    std::vector<Object*>* objectList = nullptr;
    Object* player = nullptr;

    void CheckHitCollision(float AttackRange);
    void intersectsPlayerWithWall();

    TimeManager timeManager;

private:
    App* m_app = nullptr;
};