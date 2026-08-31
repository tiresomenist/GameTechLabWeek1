#include "InGameStage.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"

void InGameStage::Enter()
{
	//여기서 플레이어 캐릭터를 생성

}

void InGameStage::Update(float deltaTime)
{
	Render();
}

void InGameStage::Render()
{
	//ObjectManager에서 오브젝트 랜더함수 호출

	ImGui::Begin("In-Game Menu");

	ImGui::End();
}

void InGameStage::Exit()
{
	//자원 전부 해제

}
