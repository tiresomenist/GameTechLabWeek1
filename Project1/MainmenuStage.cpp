#include "MainmenuStage.h"
#include "InGameStage.h"
#include "App.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include <Windows.h>

void MainmenuStage::Enter()
{


}

void MainmenuStage::Update(float deltaTime)
{
}

void MainmenuStage::Render()
{
	//메인 메뉴는 여기서 Imgui만 만져서 화면 띄우면 됨, imgui를 능숙하게 다뤄야하는데
	//ai를 써야할거같습니다.

	// 이후 ImGui UI 컨트롤 추가는 ImGui::NewFrame()과 ImGui::Render() 사이인 여기에 위치합니다. 

	ImGui::Begin("Main Menu");

	ImGui::Text("BALL PHYSICS SIMULATOR");
	ImGui::Separator();

	if (ImGui::Button("Start"))
	{
		m_app->ChangeState(new InGameStage());
	}

	if (ImGui::Button("Credits"))
	{
		ImGui::OpenPopup("Credits");
	}

	if (ImGui::BeginPopup("Credits"))
	{
		ImGui::Text("TEAM CREDITS");
		ImGui::Separator();

		ImGui::Text("현동윤");
		ImGui::Text("유현서");
		ImGui::Text("박정준");
		ImGui::Text("고민수");

		ImGui::EndPopup();
	}

	if (ImGui::Button("Exit"))
	{
		PostQuitMessage(0);
	}

	ImGui::End();


}

void MainmenuStage::Exit()
{
}
