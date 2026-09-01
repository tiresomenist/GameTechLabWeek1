#pragma once
#include "IGameStage.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Augment.h"

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
    Player* player = nullptr;

    void CheckHitCollision(float AttackRange);
    void intersectsPlayerWithWall();

    InputManager inputManager;
    float countTimeForEnemy = 0.0f;
    float countTimeForPlayer = 0.0f;
    int difficulty;

private:
    App* m_app = nullptr;

    int gameResult = 0;
    // 0 = 진행중
    // 1 = Clear
    // 2 = Game Over

	//클리어,오버시 사운드를 재생하면 Render 함수 내에서 계속해서 체크하고 재생되기때문에 사운드가 반복됨 
    // 이전 상태를 저장해서 이전 상태와 현재 상태가 다를때만 사운드를 재생하도록 함.
    int PrevGameResult = 0;

    bool openResultPopup = false;
	bool openPausePopup = false;
    bool openAugmentPopup = false;
    bool isAugmnetSelected = false;

    //증강 3개
    Augment augment;
    AugmentStruct aug1;
    AugmentStruct aug2;
    AugmentStruct aug3;

    void RenderAugmentModal();
    void RenderHUD(int minutes, int seconds);
	void RenderPauseModal();
	void RenderResultModal(int minutes, int seconds);
};