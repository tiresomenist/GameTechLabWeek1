#pragma once
#include <windows.h>

#include <d3dcompiler.h>
#include <d3d11.h>

#include "IGameStage.h"
#include "FVertexSimple.h"

class App 
{
	public:
	void ChangeState(IGameStage* newState);

	//여기서 device,context,swapchain 등등 들고있어야함
	private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;

	//게임 현재 스테이지를 들고있음
	IGameStage* m_currentStage;
	IGameStage* nextStage = nullptr;


	//뷰포트 정보(화면에 콘텐츠가 실제로 보이는 영역)
	D3D11_VIEWPORT ViewportInfo;

	HWND m_mainWindow;

	//기본 쉐이더들
	ID3D11VertexShader* defaultVertexShader;
	ID3D11PixelShader* defaultPixelShader;
	ID3D11PixelShader* texturePixelShader;
	ID3D11SamplerState* textureSampler;
	ID3D11InputLayout* defaultInputLayout;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11Texture2D* texture;

	//달 텍스쳐
	ID3D11ShaderResourceView* moonTextureSRV;
	ID3D11Texture2D* moonTexture;

	//메테오 텍스쳐
	ID3D11ShaderResourceView* meteorTextureSRV;
	ID3D11Texture2D* meteorTexture;

	//경험치 구슬 텍스쳐
	ID3D11ShaderResourceView* expOrbTextureSRV;
	ID3D11Texture2D* expOrbTexture;


	//프레임 버퍼
	ID3D11Texture2D* m_frameBuffer;
	//프레임버퍼 RTV
	ID3D11RenderTargetView* m_frameBufferRTV;
	//래스터라이저 상태
	ID3D11RasterizerState* m_rasterizerState;

	//상수 버퍼
	ID3D11Buffer* constantBuffer;

	ID3D11Buffer* m_bgVertexBuffer;
	//배경 텍스쳐
	ID3D11Texture2D* m_bgTexture;
	ID3D11ShaderResourceView* m_bgSRV;

	ID3D11Buffer* SphereVertexBuffer;
	UINT SphereNumVertices = 0;

	ID3D11Buffer* PlaneVertexBuffer;
	UINT PlaneNumVertices = 0;

	ID3D11BlendState* m_blendState;

	//화면 클리어 컬러
	float ClearColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };

	//내부 초기화 함수들
	
	void InitD3D();
	void Initwindow(HINSTANCE hInstance);
	void InitImgui();

	//자원 생성 함수
	void CreateDeviceandSwapchain();
	void CreateShader();
	void CreateTexture(const LPCWSTR& fileName, ID3D11Texture2D** texture, ID3D11ShaderResourceView** srv);
	void CreateFrambuffer();
	void CreateRasterizerState();

	//자원 해제 함수
	void ReleaseShader();
	void ReleaseDeviceAndSwapChain();

	
	//타이머,인풋 업데이트
	
	void Update();
	void Render();

	public:

	App();

	~App();

	//싱글톤
	static App* Ins;

	ID3D11Buffer* GetSphereVertexBuffer() const
	{
		return SphereVertexBuffer;
	}

	ID3D11Buffer* GetPlaneVertexBuffer() const
	{
		return PlaneVertexBuffer;
	}

	UINT GetSphereNumVertices() const
	{
		return SphereNumVertices;
	}

	UINT GetPlaneNumVertices() const
	{
		return PlaneNumVertices;
	}


	//HINSTANCE는 윈도우 초기화용
	void Init(HINSTANCE hInstance);
	
	void mainLoop();

	ID3D11Device* GetDevice() const
	{
		return m_device;
	}

	ID3D11DeviceContext* GetDeviceContext() const
	{
		return m_deviceContext;
	}

	//버텍스버퍼 생성
	ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth);

	ID3D11Buffer* GetConstantBuffer() const
	{
		return constantBuffer;
	}

	ID3D11ShaderResourceView* GetEarthTextureSRV() const { return textureSRV; }

	ID3D11ShaderResourceView* GetMoonTextureSRV()
	{
		return moonTextureSRV;
	}

	ID3D11ShaderResourceView* GetMeteorTextureSRV()
	{
		return meteorTextureSRV;
	}

	ID3D11ShaderResourceView* GetExpOrbTextureSRV()
	{
		return expOrbTextureSRV;
	}

	ID3D11BlendState* GetBlendState() const
	{
		return m_blendState;
	}

	void ReleaseAll();


};