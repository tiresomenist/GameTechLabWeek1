#include "MainmenuStage.h"
#include "InGameStage.h"
#include "App.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include <Windows.h>
#include "SoundManager.h"
#include "SettingsUI.h"
#include "LeaderboardUI.h"

#define SOUND_KEY_BGM L"bgm_2"

namespace
{
	// Main Menu
	constexpr float MENU_WIDTH = 420.0f;
	constexpr float MENU_HEIGHT = 500.0f;

	constexpr float BUTTON_WIDTH = 240.0f;
	constexpr float BUTTON_HEIGHT = 45.0f;

	// Credits
	constexpr float CREDIT_WIDTH = 340.0f;
	constexpr float CREDIT_HEIGHT = 300.0f;

	constexpr float CLOSE_BUTTON_WIDTH = 220.0f;
	constexpr float CLOSE_BUTTON_HEIGHT = 40.0f;

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
	USoundManager::GetInstance()->PlayBGM(
		SOUND_KEY_BGM,
		true
	);
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

	ImVec2 windowSize(
		MENU_WIDTH,
		MENU_HEIGHT
	);

	ImVec2 windowPos(
		(io.DisplaySize.x - MENU_WIDTH) * 0.5f,
		(io.DisplaySize.y - MENU_HEIGHT) * 0.5f
	);

	ImGui::SetNextWindowPos(
		windowPos,
		ImGuiCond_Always
	);

	ImGui::SetNextWindowSize(
		windowSize,
		ImGuiCond_Always
	);

	ImGui::PushStyleColor(
		ImGuiCol_WindowBg,
		ImVec4(0.02f, 0.04f, 0.06f, 0.35f)
	);

	ImGui::Begin(
		"Main Menu",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse
	);

	// =========================
	// Title
	// =========================

	ImGui::Dummy(
		ImVec2(0.0f, 30.0f)
	);

	// 게임 제목
	ImGui::SetWindowFontScale(2.4f);

	const char* title = "BALL SURVIVORS";
	float titleWidth =
		ImGui::CalcTextSize(title).x;

	ImGui::SetCursorPosX(
		(MENU_WIDTH - titleWidth) * 0.5f
	);

	ImGui::TextColored(
		ImVec4(
			0.35f,
			0.85f,
			0.90f,
			1.0f
		),
		"%s",
		title
	);

	ImGui::SetWindowFontScale(1.0f);

	// 작은 서브타이틀
	const char* subtitle =
		"SURVIVE THE SWARM";

	float subtitleWidth =
		ImGui::CalcTextSize(subtitle).x;

	ImGui::SetCursorPosX(
		(MENU_WIDTH - subtitleWidth) * 0.5f
	);

	ImGui::TextDisabled(
		"%s",
		subtitle
	);

	ImGui::Dummy(
		ImVec2(0.0f, 45.0f)
	);

	// 버튼 스타일
	ImGui::PushStyleVar(
		ImGuiStyleVar_FrameRounding,
		7.0f
	);

	ImGui::PushStyleColor(
		ImGuiCol_Button,
		ImVec4(
			0.10f,
			0.45f,
			0.55f,
			1.0f
		)
	);

	ImGui::PushStyleColor(
		ImGuiCol_ButtonHovered,
		ImVec4(
			0.15f,
			0.60f,
			0.70f,
			1.0f
		)
	);

	ImGui::PushStyleColor(
		ImGuiCol_ButtonActive,
		ImVec4(
			0.20f,
			0.75f,
			0.85f,
			1.0f
		)
	);

	// =========================
	// Start
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	if (ImGui::Button(
		"START",
		ImVec2(
			BUTTON_WIDTH,
			BUTTON_HEIGHT)))
	{
		m_app->ChangeState(
			new InGameStage(m_app)
		);
	}

	ImGui::Dummy(
		ImVec2(0.0f, 10.0f)
	);

	// =========================
	// Leaderboard
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	if (ImGui::Button(
		"LEADERBOARD",
		ImVec2(
			BUTTON_WIDTH,
			BUTTON_HEIGHT)))
	{
		ImGui::OpenPopup(
			"Leaderboard"
		);
	}

	ImGui::Dummy(
		ImVec2(0.0f, 10.0f)
	);

	// =========================
	// Settings
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	if (ImGui::Button(
		"SETTINGS",
		ImVec2(
			BUTTON_WIDTH,
			BUTTON_HEIGHT)))
	{
		ImGui::OpenPopup(
			"Settings"
		);
	}

	ImGui::Dummy(
		ImVec2(0.0f, 10.0f)
	);

	// =========================
	// Credits
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	if (ImGui::Button(
		"CREDITS",
		ImVec2(
			BUTTON_WIDTH,
			BUTTON_HEIGHT)))
	{
		ImGui::OpenPopup(
			"Credits"
		);
	}

	ImGui::Dummy(
		ImVec2(0.0f, 10.0f)
	);

	// =========================
	// Exit
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	if (ImGui::Button(
		"EXIT",
		ImVec2(
			BUTTON_WIDTH,
			BUTTON_HEIGHT)))
	{
		PostQuitMessage(0);
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	// =========================
	// Credits Popup
	// =========================

	// 화면 중앙에 배치
	ImGui::SetNextWindowPos(
		ImVec2(
			io.DisplaySize.x * 0.5f,
			io.DisplaySize.y * 0.5f
		),
		ImGuiCond_Appearing,
		ImVec2(0.5f, 0.5f)
	);

	ImGui::SetNextWindowSize(
		ImVec2(
			CREDIT_WIDTH,
			CREDIT_HEIGHT
		),
		ImGuiCond_Always
	);

	// Credits 모달 배경
	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowPadding,
		ImVec2(25.0f, 22.0f)
	);

	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowRounding,
		14.0f
	);

	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowBorderSize,
		1.0f
	);

	ImGui::PushStyleVar(
		ImGuiStyleVar_FrameRounding,
		7.0f
	);

	ImGui::PushStyleColor(
		ImGuiCol_PopupBg,
		ImVec4(
			0.04f,
			0.05f,
			0.07f,
			0.99f
		)
	);

	ImGui::PushStyleColor(
		ImGuiCol_Border,
		ImVec4(
			0.15f,
			0.50f,
			0.60f,
			0.8f
		)
	);

	if (ImGui::BeginPopupModal(
		"Credits",
		nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse))
	{
		// 최상단 타이틀
		ImGui::SetWindowFontScale(1.8f);

		const char* creditTitle =
			"TEAM CREDITS";

		float creditTitleWidth =
			ImGui::CalcTextSize(
				creditTitle
			).x;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() -
				creditTitleWidth) * 0.5f
		);

		ImGui::Text(
			"%s",
			creditTitle
		);

		ImGui::SetWindowFontScale(1.0f);

		const char* creditSubtitle =
			"DEVELOPED BY";

		float creditSubtitleWidth =
			ImGui::CalcTextSize(
				creditSubtitle
			).x;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() -
				creditSubtitleWidth) * 0.5f
		);

		ImGui::TextDisabled(
			"%s",
			creditSubtitle
		);

		ImGui::Dummy(
			ImVec2(0.0f, 20.0f)
		);

		// 팀원 이름
		for (const char* member : CREDIT_MEMBERS)
		{
			float textWidth =
				ImGui::CalcTextSize(
					member
				).x;

			ImGui::SetCursorPosX(
				(ImGui::GetWindowWidth() -
					textWidth) * 0.5f
			);

			ImGui::Text(
				"%s",
				member
			);

			ImGui::Dummy(
				ImVec2(0.0f, 7.0f)
			);
		}

		ImGui::Dummy(
			ImVec2(0.0f, 15.0f)
		);

		// Close
		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() -
				CLOSE_BUTTON_WIDTH) * 0.5f
		);

		ImGui::PushStyleColor(
			ImGuiCol_Button,
			ImVec4(
				0.10f,
				0.45f,
				0.55f,
				1.0f
			)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonHovered,
			ImVec4(
				0.15f,
				0.60f,
				0.70f,
				1.0f
			)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonActive,
			ImVec4(
				0.20f,
				0.75f,
				0.85f,
				1.0f
			)
		);

		if (ImGui::Button(
			"CLOSE",
			ImVec2(
				CLOSE_BUTTON_WIDTH,
				CLOSE_BUTTON_HEIGHT)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::PopStyleColor(3);

		// ESC 키 입력 시 Credits 닫기
		if (ImGui::IsKeyPressed(
			ImGuiKey_Escape))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(4);

	// =========================
	// Leaderboard Popup
	// =========================

	LeaderboardUI::Render();

	// =========================
	// Settings Popup
	// =========================

	SettingsUI::Render();

	ImGui::End();

	ImGui::PopStyleColor();

}

void MainmenuStage::Exit()
{
} 