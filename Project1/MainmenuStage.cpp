#include "MainmenuStage.h"
#include "InGameStage.h"
#include "App.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include <Windows.h>

namespace
{
	// Main Menu
	constexpr float MENU_WIDTH = 420.0f;
	constexpr float MENU_HEIGHT = 360.0f;

	constexpr float BUTTON_WIDTH = 220.0f;
	constexpr float BUTTON_HEIGHT = 45.0f;

	// Credits
	constexpr float CREDIT_WIDTH = 320.0f;
	constexpr float CREDIT_HEIGHT = 260.0f;

	constexpr float CLOSE_BUTTON_WIDTH = 120.0f;
	constexpr float CLOSE_BUTTON_HEIGHT = 35.0f;

    const char* CREDIT_MEMBERS[] =
    {
        "Dongyun Hyun",
        "Hyunseo Yoo",
        "Jeongjun Park",
        "Minsu Ko"
    };
}

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

    ImGuiIO& io = ImGui::GetIO();

    ImVec2 windowSize(MENU_WIDTH, MENU_HEIGHT);

    ImVec2 windowPos(
        (io.DisplaySize.x - MENU_WIDTH) * 0.5f,
        (io.DisplaySize.y - MENU_HEIGHT) * 0.5f
    );

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("Main Menu", nullptr, flags);

    // =========================
    // Title
    // =========================

    ImGui::Dummy(ImVec2(0.0f, 35.0f));

    const char* title = "Ball Survivors";
    float titleWidth = ImGui::CalcTextSize(title).x;

    ImGui::SetCursorPosX((MENU_WIDTH - titleWidth) * 0.5f);
    ImGui::Text("%s", title);

    ImGui::Dummy(ImVec2(0.0f, 45.0f));

    // =========================
    // Start
    // =========================

    ImGui::SetCursorPosX((MENU_WIDTH - BUTTON_WIDTH) * 0.5f);

    if (ImGui::Button("START", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
    {
        m_app->ChangeState(new InGameStage());
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    // =========================
    // Credits
    // =========================

    ImGui::SetCursorPosX((MENU_WIDTH - BUTTON_WIDTH) * 0.5f);

    if (ImGui::Button("CREDITS", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
    {
        ImGui::OpenPopup("Credits");
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    // =========================
    // Exit
    // =========================

    ImGui::SetCursorPosX((MENU_WIDTH - BUTTON_WIDTH) * 0.5f);

    if (ImGui::Button("EXIT", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
    {
        PostQuitMessage(0);
    }

    // =========================
    // Credits Popup
    // =========================

    ImGui::SetNextWindowSize(
        ImVec2(CREDIT_WIDTH, CREDIT_HEIGHT),
        ImGuiCond_Always
    );

    if (ImGui::BeginPopupModal(
        "Credits",
        nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse))
    {
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        const char* creditTitle = "TEAM CREDITS";
        float creditTitleWidth = ImGui::CalcTextSize(creditTitle).x;

        ImGui::SetCursorPosX(
            (CREDIT_WIDTH - creditTitleWidth) * 0.5f
        );

        ImGui::Text("%s", creditTitle);

        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // 팀원 이름
        for (const char* member : CREDIT_MEMBERS)
        {
            float textWidth = ImGui::CalcTextSize(member).x;

            ImGui::SetCursorPosX(
                (CREDIT_WIDTH - textWidth) * 0.5f
            );

            ImGui::Text("%s", member);

            ImGui::Dummy(ImVec2(0.0f, 5.0f));
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        // Close
        ImGui::SetCursorPosX(
            (CREDIT_WIDTH - CLOSE_BUTTON_WIDTH) * 0.5f
        );

        if (ImGui::Button(
            "CLOSE",
            ImVec2(CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();


}

void MainmenuStage::Exit()
{
}
