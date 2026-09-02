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
#include "InputManager.h"

#define SOUND_KEY_BGM L"bgm_2"


enum
{
	MENU_START,
	MENU_LEADERBOARD,
	MENU_SETTINGS,
	MENU_CREDITS,
	MENU_EXIT,
	MENU_COUNT
};

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

	if (player == nullptr) {
		player = ObjectManager::GetInstance()->CreatePlayer();
	}

	TimeManager::GetInstance()->TimeReset();
	TimeManager::GetInstance()->TimeStart();
	player->SetWeaponRadius(0.1f);

	ObjectManager::GetInstance()->CreateWeapon(player->GetWeaponRadius());

	player->ScalePlayerRadius(10.0f);
}

void MainmenuStage::Update(float deltaTime)
{
	TimeManager::GetInstance()->TimeUpdate(deltaTime);

	ObjectManager::GetInstance()->SpinWeaponMainMenu(
		deltaTime,
		player->GetWeaponRotationSpeed() * 0.2f
	);

	//공들 회전
	ObjectManager::GetInstance()->Update(deltaTime);

	if (InputManager::GetInstance()->IsKeyTriggered(VK_UP) || InputManager::GetInstance()->IsKeyTriggered('W')) {
		selectedMenu--;

		if (selectedMenu < 0) {
			selectedMenu = MENU_COUNT - 1;
		}
	}

	if (InputManager::GetInstance()->IsKeyTriggered(VK_DOWN) || InputManager::GetInstance()->IsKeyTriggered('S')) {
		selectedMenu++;

		if (selectedMenu >= MENU_COUNT) {
			selectedMenu = 0;
		}
	}

	enterPressed = InputManager::GetInstance()->IsKeyTriggered(VK_RETURN);

}

void MainmenuStage::Render()
{
	// =========================
	// Main Menu Background
	// =========================

	ObjectManager::GetInstance()->RenderMainMenu();

	//메인 메뉴는 여기서 Imgui만 만져서 화면 띄우면 됨, imgui를 능숙하게 다뤄야하는데
	//ai를 써야할거같습니다.

	// 이후 ImGui UI 컨트롤 추가는 ImGui::NewFrame()과 ImGui::Render() 사이인 여기에 위치합니다.

	ImGuiIO& io = ImGui::GetIO();

	const float BUTTON_GAP = 10.0f;
	const float PANEL_PADDING = 20.0f;

	ImVec2 windowSize(
		MENU_WIDTH,
		MENU_HEIGHT
	);

	ImVec2 windowPos(
		(io.DisplaySize.x - MENU_WIDTH) * 0.5f,
		(io.DisplaySize.y - MENU_HEIGHT) * 0.5f + 100.0f
	);

	ImGui::SetNextWindowPos(
		windowPos,
		ImGuiCond_Always
	);

	ImGui::SetNextWindowSize(
		windowSize,
		ImGuiCond_Always
	);

	ImGui::Begin(
		"Main Menu",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoScrollWithMouse
	);

	// =========================
	// 기본 버튼 스타일
	// =========================

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

	// START 버튼의 실제 시작 위치 저장
	ImVec2 startPos =
		ImGui::GetCursorScreenPos();

	ImGui::PushStyleColor(
		ImGuiCol_Button,
		selectedMenu == MENU_START
		? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
		: ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
	);

	bool startPressed = ImGui::Button(
		"START",
		ImVec2(
			BUTTON_WIDTH,
			BUTTON_HEIGHT
		)
	);

	ImGui::PopStyleColor();

	if (
		startPressed ||
		(selectedMenu == MENU_START && enterPressed)
		)
	{
		m_app->ChangeState(
			new InGameStage(m_app)
		);
	}

	ImGui::Dummy(
		ImVec2(
			0.0f,
			BUTTON_GAP
		)
	);

	// =========================
	// Leaderboard
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	ImGui::PushStyleColor(
		ImGuiCol_Button,
		selectedMenu == MENU_LEADERBOARD
		? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
		: ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
	);

	bool leaderboardPressed =
		ImGui::Button(
			"LEADERBOARD",
			ImVec2(
				BUTTON_WIDTH,
				BUTTON_HEIGHT
			)
		);

	ImGui::PopStyleColor();

	if (
		leaderboardPressed ||
		(
			selectedMenu == MENU_LEADERBOARD &&
			enterPressed &&
			!ImGui::IsPopupOpen("Leaderboard")
			)
		)
	{
		ImGui::OpenPopup(
			"Leaderboard"
		);
	}

	ImGui::Dummy(
		ImVec2(
			0.0f,
			BUTTON_GAP
		)
	);

	// =========================
	// Settings
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	ImGui::PushStyleColor(
		ImGuiCol_Button,
		selectedMenu == MENU_SETTINGS
		? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
		: ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
	);

	bool settingsPressed =
		ImGui::Button(
			"SETTINGS",
			ImVec2(
				BUTTON_WIDTH,
				BUTTON_HEIGHT
			)
		);

	ImGui::PopStyleColor();

	if (
		settingsPressed ||
		(
			selectedMenu == MENU_SETTINGS &&
			enterPressed &&
			!ImGui::IsPopupOpen("Settings")
			)
		)
	{
		ImGui::OpenPopup(
			"Settings"
		);
	}

	ImGui::Dummy(
		ImVec2(
			0.0f,
			BUTTON_GAP
		)
	);

	// =========================
	// Credits
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	ImGui::PushStyleColor(
		ImGuiCol_Button,
		selectedMenu == MENU_CREDITS
		? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
		: ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
	);

	bool creditsPressed =
		ImGui::Button(
			"CREDITS",
			ImVec2(
				BUTTON_WIDTH,
				BUTTON_HEIGHT
			)
		);

	ImGui::PopStyleColor();

	if (
		creditsPressed ||
		(
			selectedMenu == MENU_CREDITS &&
			enterPressed &&
			!ImGui::IsPopupOpen("Credits")
			)
		)
	{
		ImGui::OpenPopup(
			"Credits"
		);
	}

	ImGui::Dummy(
		ImVec2(
			0.0f,
			BUTTON_GAP
		)
	);

	// =========================
	// Exit
	// =========================

	ImGui::SetCursorPosX(
		(MENU_WIDTH - BUTTON_WIDTH) * 0.5f
	);

	ImGui::PushStyleColor(
		ImGuiCol_Button,
		selectedMenu == MENU_EXIT
		? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
		: ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
	);

	bool exitPressed =
		ImGui::Button(
			"EXIT",
			ImVec2(
				BUTTON_WIDTH,
				BUTTON_HEIGHT
			)
		);

	// EXIT 버튼의 실제 아래쪽 좌표
	ImVec2 exitMax =
		ImGui::GetItemRectMax();

	ImGui::PopStyleColor();

	// =========================
	// 버튼 뒤 패널
	// =========================

	// BackgroundDrawList를 쓰므로
	// 코드상 지금 그려도 버튼 뒤쪽에 위치함
	ImDrawList* backgroundDrawList =
		ImGui::GetBackgroundDrawList();

	ImVec2 panelMin(
		startPos.x - PANEL_PADDING,
		startPos.y - PANEL_PADDING
	);

	ImVec2 panelMax(
		startPos.x + BUTTON_WIDTH + PANEL_PADDING,
		exitMax.y + PANEL_PADDING
	);

	backgroundDrawList->AddRectFilled(
		panelMin,
		panelMax,
		IM_COL32(
			5,
			12,
			18,
			100
		),
		12.0f
	);

	if (
		exitPressed ||
		(selectedMenu == MENU_EXIT && enterPressed)
		)
	{
		PostQuitMessage(0);
	}

	// 기본 버튼 스타일 3개 복구
	ImGui::PopStyleColor(3);

	// =========================
	// Credits Popup
	// =========================

	ImGui::SetNextWindowPos(
		ImVec2(
			io.DisplaySize.x * 0.5f,
			io.DisplaySize.y * 0.5f
		),
		ImGuiCond_Appearing,
		ImVec2(
			0.5f,
			0.5f
		)
	);

	ImGui::SetNextWindowSize(
		ImVec2(
			CREDIT_WIDTH,
			CREDIT_HEIGHT
		),
		ImGuiCond_Always
	);

	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowPadding,
		ImVec2(
			25.0f,
			22.0f
		)
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

	if (
		ImGui::BeginPopupModal(
			"Credits",
			nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse
		)
		)
	{
		// =========================
		// Credits Title
		// =========================

		ImGui::SetWindowFontScale(
			1.8f
		);

		const char* creditTitle =
			"TEAM CREDITS";

		float creditTitleWidth =
			ImGui::CalcTextSize(
				creditTitle
			).x;

		ImGui::SetCursorPosX(
			(
				ImGui::GetWindowWidth() -
				creditTitleWidth
				) * 0.5f
		);

		ImGui::Text(
			"%s",
			creditTitle
		);

		ImGui::SetWindowFontScale(
			1.0f
		);

		const char* creditSubtitle =
			"DEVELOPED BY";

		float creditSubtitleWidth =
			ImGui::CalcTextSize(
				creditSubtitle
			).x;

		ImGui::SetCursorPosX(
			(
				ImGui::GetWindowWidth() -
				creditSubtitleWidth
				) * 0.5f
		);

		ImGui::TextDisabled(
			"%s",
			creditSubtitle
		);

		ImGui::Dummy(
			ImVec2(
				0.0f,
				20.0f
			)
		);

		// =========================
		// Team Members
		// =========================

		for (const char* member : CREDIT_MEMBERS)
		{
			float textWidth =
				ImGui::CalcTextSize(
					member
				).x;

			ImGui::SetCursorPosX(
				(
					ImGui::GetWindowWidth() -
					textWidth
					) * 0.5f
			);

			ImGui::Text(
				"%s",
				member
			);

			ImGui::Dummy(
				ImVec2(
					0.0f,
					7.0f
				)
			);
		}

		ImGui::Dummy(
			ImVec2(
				0.0f,
				15.0f
			)
		);

		// =========================
		// Close
		// =========================

		ImGui::SetCursorPosX(
			(
				ImGui::GetWindowWidth() -
				CLOSE_BUTTON_WIDTH
				) * 0.5f
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

		if (
			ImGui::Button(
				"CLOSE",
				ImVec2(
					CLOSE_BUTTON_WIDTH,
					CLOSE_BUTTON_HEIGHT
				)
			)
			)
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::PopStyleColor(3);

		// ESC 키 입력 시 Credits 닫기
		if (
			ImGui::IsKeyPressed(
				ImGuiKey_Escape
			)
			)
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

}

void MainmenuStage::Exit()
{
	ObjectManager::GetInstance()->ReleaseAllObjects();
} 