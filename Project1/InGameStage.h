#pragma once
#include "IGameStage.h"
#include "App.h"
#include "MainmenuStage.h"
#include "ObjectManager.h"
#include "TimeManager.h"

class InGameStage : public IGameStage
{
	void Enter() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;




};


