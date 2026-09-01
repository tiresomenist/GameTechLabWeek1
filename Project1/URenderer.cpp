#include "App.h"
#include "URenderer.h"
#include "Sphere.h"
#include "FConstant.h"
#include "D3D11Util.h"

URenderer::URenderer()
{
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