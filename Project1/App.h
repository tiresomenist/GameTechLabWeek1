#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <windows.h>
#include "IGameStage.h"

class App 
{
	//fsm을 돌리면
	//


	//여기서 device,context,swapchain 등등 들고있어야함
	private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	IDXGISwapChain* m_swapChain;

	//게임 현재 스테이지를 들고있음
	IGameStage* m_currentStage;

	//내부 초기화 함수들
	
	void InitD3D();
	void Initwindow(HINSTANCE hInstance);

	//타이머,인풋 업데이트
	
	void Update();
	void Render();
	public:

	//싱글톤
	static App* Ins;
	//HINSTANCE는 윈도우 초기화용
	void Init(HINSTANCE hInstance);
	
	void mainLoop();

	



};