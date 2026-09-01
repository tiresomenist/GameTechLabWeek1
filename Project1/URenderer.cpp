#include "App.h"
#include "URenderer.h"
#include "Sphere.h"

URenderer::URenderer()
{
	vertexBuffer = App::Ins->CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
	numVertices = sizeof(sphere_vertices) / sizeof(FVertexSimple);



}
void URenderer::RenderPrimitive(ID3D11Buffer* vertexBuffer, UINT numVertices)
{
	UINT offset = 0;
	App::Ins->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &Stride, &offset);

	App::Ins->GetDeviceContext()->Draw(numVertices, 0);
}
;