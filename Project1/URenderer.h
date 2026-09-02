#pragma once
#include  <d3d11.h>
#include <d3dcompiler.h>
#include "App.h"
#include "FVector.h"
#include "EPrimitives.h"

class URenderer
{
	//UObject 생성자에서 vertexBuffer를 Sphere.h 파일과 App.h에있는 CreateVertexBuffer함수를 불러서 초기화
	//numVertices도 계산해서 초기화


	ID3D11Buffer* vertexConstantBuffer;

	UINT Stride = sizeof(FVertexSimple);

	public:
	URenderer();
	~URenderer();

	void RenderPrimitive(FVector location, float radius, Priv primitiveType);


	//ID3D11Buffer* GetVertexBuffer() const
	//{
	//	return vertexBuffer;
	//}

	//UINT GetStride()
	//{
	//	return Stride;
	//}





};

