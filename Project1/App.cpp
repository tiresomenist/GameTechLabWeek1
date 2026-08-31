#include "App.h"

void mainLoop()
{
	Update();
	Render();
};

void Update()
{
	//타이머,인풋 매니저 호출

};
void Render()
{
	//현재 상태의 render 호출
	m_currentStage->Render();

};