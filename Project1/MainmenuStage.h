#pragma once
#include "IGameStage.h"
#include "LeaderboardManager.h"

class App;

class MainmenuStage : public IGameStage
{
public:
	MainmenuStage(App* app) 
		: m_app(app) 
	{
	}


	void Enter() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;


private:
	App* m_app;
	int selectedMenu = 0;
	bool enterPressed = false;


};