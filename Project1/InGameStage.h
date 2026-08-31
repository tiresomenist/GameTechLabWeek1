#pragma once
#include "IGameStage.h"

class InGameStage : public IGameStage
{
	void Enter() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;




};


