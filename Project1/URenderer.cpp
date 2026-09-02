#include "App.h"
#include "URenderer.h"
#include "Sphere.h"
#include "FConstant.h"
#include "D3D11Util.h"
#include <cmath>
#include <algorithm>

const float PI = 3.14159265358979323846f;

URenderer::URenderer()
{
	for (auto& vertex : sphere_vertices)
	{
		// vertex.x, vertex.y, vertex.z 사용
		// ↓
		// vertex.u 계산
		// vertex.v 계산

		vertex.u = 0.5f + (atan2(vertex.z, vertex.x) / (2 * PI));
		vertex.v = 0.5f - (asin(std::clamp(vertex.y, -1.0f, 1.0f)) / PI);
	}

	vertexBuffer = App::Ins->CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
	numVertices = sizeof(sphere_vertices) / sizeof(FVertexSimple);
}

void URenderer::RenderPrimitive(FVector location,float radius)
{
	FConstant constantData;
	constantData.offset = location;
	constantData.radius = radius;

	D3D11Util::UpdateConstantBuffer(App::Ins->GetDeviceContext(), App::Ins->GetConstantBuffer(), constantData);


	UINT Offset = 0;
	App::Ins->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &Stride, &Offset);

	App::Ins->GetDeviceContext()->Draw(numVertices, 0);
}
;