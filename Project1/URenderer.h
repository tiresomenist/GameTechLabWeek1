#pragma once
#include  <d3d11.h>
#include <d3dcompiler.h>
#include "App.h"

class URenderer
{
	//UObject 생성자에서 vertexBuffer를 Sphere.h 파일과 App.h에있는 CreateVertexBuffer함수를 불러서 초기화
	//numVertices도 계산해서 초기화
	ID3D11Buffer* vertexBuffer;
	UINT numVertices = 0;
	ID3D11Buffer* vertexConstantBuffer;

	UINT Stride = sizeof(FVertexSimple);

	public:
	URenderer();
	~URenderer();

	void RenderPrimitive(ID3D11Buffer* vertexBuffer, UINT numVertices);









};

