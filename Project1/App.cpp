#include "App.h"

App* App::Ins = nullptr;

void App::Init(HINSTANCE hInstance)
{
}

void App::mainLoop()
{
	Update();
	Render();
};

void App::Update()
{
	//타이머,인풋 매니저 호출

};
void App::Render()
{
	//현재 상태의 render 호출


};