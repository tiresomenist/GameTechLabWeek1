#pragma once
#include "App.h"
#include "URenderer.h"
#include "Sphere.h"
#include "FConstant.h"
#include "D3D11Util.h"
#include <cmath>
#include <algorithm>
#include "EPrimitives.h"

const float PI = 3.14159265358979323846f;

URenderer::URenderer()
{
	
}

void URenderer::RenderPrimitive(FVector location,float radius, FVector rotation, Priv primitiveType)
{
	FConstant constantData;
	constantData.offset = location;
	constantData.scale = FVector(radius, radius, radius);
	constantData.Rotation = rotation;
	D3D11Util::UpdateConstantBuffer(App::Ins->GetDeviceContext(), App::Ins->GetConstantBuffer(), constantData);


	UINT offset = 0;
	if (Priv::Sphere == primitiveType)
	{
		App::Ins->GetDeviceContext()->Draw(App::Ins->GetSphereNumVertices(), 0);
	}
	else if (Priv::Plane == primitiveType)
	{
		App::Ins->GetDeviceContext()->Draw(App::Ins->GetPlaneNumVertices(), 0);
	}
}

void URenderer::RenderPlane(FVector location, float scaleX, float scaleY, FVector rotation)
{
	FConstant constantData;

	constantData.offset = location;
	constantData.scale = FVector(scaleX, scaleY, 1.0f);
	constantData.Rotation = rotation;

	D3D11Util::UpdateConstantBuffer(App::Ins->GetDeviceContext(), App::Ins->GetConstantBuffer(), constantData);

	App::Ins->GetDeviceContext()->Draw(App::Ins->GetPlaneNumVertices(),	0);
}
