#include "App.h"
#include "D3D11Util.h"
#include "MainmenuStage.h"
#include <cstdio>

#pragma comment(lib, "d3d11.lib")

#include "FConstant.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include "InputManager.h"

App* App::Ins = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT	CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Imgui를 사용하게 되면, 필요한 핸들러, imgui 라이브러리 merge후에 주석 풀기
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
	{
		return true;
	}

	switch (message)
	{
	case WM_KEYDOWN:
		OutputDebugStringA("WM_KEYDOWN Triggered!\n");
		InputManager::IMins->KeyDown(static_cast<int>(wParam));
		break;
	case WM_KEYUP:
		OutputDebugStringA("WM_KEYDOWN Triggered!\n");
		InputManager::IMins->KeyUp(static_cast<int>(wParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}


void App::Init(HINSTANCE hInstance)
{
	Initwindow(hInstance);
	InitD3D();
	InitImgui();
	InputManager::IMins = new InputManager();
	ChangeState(new MainmenuStage(this));
}

void App::mainLoop()
{
	Update();
   	Render();
}
void App::ReleaseAll()
{
	ReleaseShader();
	ReleaseDeviceAndSwapChain();
};

void App::ChangeState(IGameStage* newState)
{
	nextStage = newState;
}

void App::ReleaseShader()
{

	if (defaultInputLayout)
	{
		defaultInputLayout->Release();
		defaultInputLayout = nullptr;
	}

	if (defaultVertexShader)
	{
		defaultVertexShader->Release();
		defaultVertexShader = nullptr;
	}

	if (defaultPixelShader)
	{
		defaultPixelShader->Release();
		defaultPixelShader = nullptr;
	}

}
void App::ReleaseDeviceAndSwapChain()
{
	if (m_deviceContext)
	{
		m_deviceContext->Flush();
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

};

void App::Initwindow(HINSTANCE hInstance)
{
	WCHAR WindowClass[] = L"Break_Out";

	WCHAR Title[] = L"Break Out";


	WNDCLASSW wndclass = { 0, WndProc, 0,0,0,0,0,0,0, WindowClass };

	RegisterClassW(&wndclass);


	m_mainWindow = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, wndclass.hInstance, nullptr);

	if (m_mainWindow)
	{
		ShowWindow(m_mainWindow, SW_SHOW);
		UpdateWindow(m_mainWindow);
	}
}

void App::InitImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init((void*)m_mainWindow);
	ImGui_ImplDX11_Init(m_device, m_deviceContext);
}

void App::InitD3D()
{
	CreateDeviceandSwapchain();
	CreateShader();
	CreateFrambuffer();
	CreateRasterizerState();

	D3D11_BUFFER_DESC constantbufferdesc = {};
	constantbufferdesc.ByteWidth = sizeof(FConstant);
	constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;
	constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	m_device->CreateBuffer(&constantbufferdesc, nullptr, &constantBuffer);

	assert(constantBuffer != nullptr);
};

void App::CreateDeviceandSwapchain()
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	RECT rc;
	GetClientRect(m_mainWindow, &rc);
	swapChainDesc.BufferDesc.Width = rc.right - rc.left;
	swapChainDesc.BufferDesc.Height = rc.bottom - rc.top;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = m_mainWindow;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);
	if (FAILED(hr))
	{
		char buf[128];
		sprintf_s(buf, "D3D11CreateDeviceAndSwapChain failed: 0x%08X\n", hr);
		OutputDebugStringA(buf);
		return;
	}

	DXGI_SWAP_CHAIN_DESC actualDesc = {};
	m_swapChain->GetDesc(&actualDesc);

	ViewportInfo = { 0.0f, 0.0f,
		static_cast<float>(actualDesc.BufferDesc.Width),
		static_cast<float>(actualDesc.BufferDesc.Height),
		0.0f, 1.0f };
}

void App::CreateShader()
{
	D3D11Util::CreateVertexShaderAndInputLayout(L"defaultVS.hlsl", m_device, &defaultVertexShader, &defaultInputLayout);
	D3D11Util::CreatePixelShader(L"defaultPS.hlsl", &defaultPixelShader);
}

void App::CreateFrambuffer()
{
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_frameBuffer));


	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	m_device->CreateRenderTargetView(m_frameBuffer, &rtvDesc, &m_frameBufferRTV);
}

void App::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;

	m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
}

void App::Update()
{
	//타이머,인풋 매니저 호출
	if (nextStage != nullptr)
	{

		if (m_currentStage != nullptr)
		{
			m_currentStage->Exit();
			delete m_currentStage;
		}

		m_currentStage = nextStage;
		m_currentStage->Enter();
		nextStage = nullptr;
	}


	if (m_currentStage != nullptr)
	{
		
		//m_currentStage->Update(deltaTime);

	}
};
void App::Render()
{
	//현재 상태의 render 호출

	//화면 클리어
	m_deviceContext->ClearRenderTargetView(m_frameBufferRTV, ClearColor);


	//IA
	
	//인풋레이아웃 설정
	m_deviceContext->IASetInputLayout(defaultInputLayout);
	//topology를 trianglelist로 설정
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//RS
	m_deviceContext->RSSetViewports(1, &ViewportInfo);
	m_deviceContext->RSSetState(m_rasterizerState);


	//OM
	m_deviceContext->OMSetRenderTargets(1, &m_frameBufferRTV, nullptr);

	m_deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	m_deviceContext->VSSetShader(defaultVertexShader,0,0);
	m_deviceContext->PSSetShader(defaultPixelShader, 0, 0);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (m_currentStage != nullptr)
 		m_currentStage->Render();


	ImGui::Render();
 	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	m_swapChain->Present(1, 0);
 }

App::App()
{
	Ins = this;

};

ID3D11Buffer* App::CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vertexBufferSRD = { vertices };

	ID3D11Buffer* vertexBuffer = nullptr;
	m_device->CreateBuffer(&bufferDesc, &vertexBufferSRD, &vertexBuffer);

	return vertexBuffer;
}
App::~App()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	ReleaseAll();
}
;

