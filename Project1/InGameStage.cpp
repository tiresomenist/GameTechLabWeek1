#include "InGameStage.h"
#include "App.h"
#include "MainmenuStage.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"


#include "ObjectManager.h"
#include "SoundManager.h"
#include "Augment.h"
#include "SettingsUI.h"
#include "LeaderboardUI.h"

enum
{
	PAUSE_RESUME,
	PAUSE_SETTINGS,
	PAUSE_MAIN_MENU,
	PAUSE_COUNT
};

enum
{
	LEAVE_YES,
	LEAVE_NO,
	LEAVE_COUNT
};

InGameStage::InGameStage(App* app)
	: m_app(app)
{
}

void InGameStage::Enter()
{
	//여기서 플레이어 캐릭터를 생성
	if (player == nullptr) {
		player = ObjectManager::GetInstance()->CreatePlayer();
	}

	TimeManager::GetInstance()->TimeReset();
	TimeManager::GetInstance()->TimeStart();

	gameResult = 0;
	ObjectManager::GetInstance()->setKillCount(0);
	score = 0;

	ObjectManager::GetInstance()->CreateWeapon(player->GetWeaponRadius());
	ObjectManager::GetInstance()->CreateWeapon(player->GetWeaponRadius());
	ObjectManager::GetInstance()->CreateWeapon(player->GetWeaponRadius());
	ObjectManager::GetInstance()->CreateWeapon(player->GetWeaponRadius());
	ObjectManager::GetInstance()->CreateWeapon(player->GetWeaponRadius());



	USoundManager::GetInstance()->StopBGM();
	USoundManager::GetInstance()->PlayBGM(SOUND_KEY_BGM, true);
}

void InGameStage::Update(float deltaTime)
{
	if (ObjectManager::GetInstance()->isPlayerDead())
	{
		TimeManager::GetInstance()->TimePause();
		gameResult = 2;
		openResultPopup = true;
		return;
	}
	// Pause / Clear / GameOver 상태면 게임 진행 중지
	if (!TimeManager::GetInstance()->IsRunning())
		return;

	TimeManager::GetInstance()->TimeUpdate(deltaTime);

	// 60초 생존하면 Clear
	if (TimeManager::GetInstance()->GetcurrentTime() >= 60.0f)
	{
		TimeManager::GetInstance()->TimePause();
		gameResult = 1;
		openResultPopup = true;
		return;
	}
	countTimeForEnemy += deltaTime;
	countTimeForPlayer += deltaTime;
	difficulty = TimeManager::GetInstance()->GetcurrentTime() / 10.0f;
	//적군 생성 로직
	if (countTimeForEnemy > 2.0f - (difficulty * 0.39f)) {
		countTimeForEnemy -= 2.0f-(difficulty * 0.39f);
		ObjectManager::GetInstance()->CreateEnemy(difficulty);
		//OutputDebugStringA("Enemy Creted!\n");
	}

	////플레이어 이동
	FVector moveDir(0.0f, 0.0f, 0.0f);
	if (InputManager::GetInstance()->IsKeyPressed(VK_UP) || InputManager::GetInstance()->IsKeyPressed('W'))    moveDir.y += 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_DOWN) || InputManager::GetInstance()->IsKeyPressed('S'))  moveDir.y -= 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_LEFT) || InputManager::GetInstance()->IsKeyPressed('A'))  moveDir.x -= 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_RIGHT) || InputManager::GetInstance()->IsKeyPressed('D')) moveDir.x += 1.0f;
	//인풋매니저 가져오고 델타타임이랑 이동속도 생각해서 한프레임당 이동 거리 계산해서 move호출
	
	player->MoveObject(moveDir.x * deltaTime * player->GetSpeed(), moveDir.y * deltaTime * player->GetSpeed());
	
	//무기 돌리기
	ObjectManager::GetInstance()->SpinWeapon(deltaTime, player->GetWeaponRotationSpeed());	
	//무기와 적의 충돌 체크
	ObjectManager::GetInstance()->checkWeaponIntersectWithEnemy();
	//적들 이동
	ObjectManager::GetInstance()->EnemyMove(deltaTime);
	//적들끼리 충돌하는지 체크 
	ObjectManager::GetInstance()->checkEnemiesIntersect();
	//플레이어에게 충돌하는지 체크
	ObjectManager::GetInstance()->checkPlayerIntersectWithEnemy();
	//플레이어와 벽의 충돌 체크
	ObjectManager::GetInstance()->intersectsPlayerWithWall();
	//플레이어와 레벨업 오브젝트 충돌 체크
	ObjectManager::GetInstance()->checkPlayerIntersectWithExpOrb();

	// 레벨업 경험치 도달 시 일시정지 및 팝업 on
	if (player->IsLevelUp()) 
	{
		USoundManager::GetInstance()->PlaySFX(LEVEL_UP);
		OutputDebugStringA("Level UP!");
		TimeManager::GetInstance()->TimePause();
		openAugmentPopup = true;
	}
	//공들 회전
	ObjectManager::GetInstance()->Update(deltaTime);
}

void InGameStage::RenderAugmentModal()
{
	if (!openAugmentPopup)
		return;

	if (InputManager::GetInstance()->IsKeyTriggered(VK_LEFT) || InputManager::GetInstance()->IsKeyTriggered('A'))
	{
		selectedAugment--;

		if (selectedAugment < 0)
			selectedAugment = 2;
	}

	if (InputManager::GetInstance()->IsKeyTriggered(VK_RIGHT) || InputManager::GetInstance()->IsKeyTriggered('D'))
	{
		selectedAugment++;

		if (selectedAugment >= 3)
			selectedAugment = 0;
	}

	bool augmentEnterPressed = InputManager::GetInstance()->IsKeyTriggered(VK_RETURN);


	//증강 생성 되었는지
	if (!isAugmnetSelected) {
		//증강 3개 랜덤뽑기
		aug1 = augment.GetAugmentStruct();
		aug2 = augment.GetAugmentStruct();
		aug3 = augment.GetAugmentStruct();
		augment.ResetAugment();

		isAugmnetSelected = true;
		selectedAugment = 0;
	}

	AugmentStruct* augments[3] = { &aug1, &aug2, &aug3 };

	float width = 190.0f;
	float height = 260.0f;
	float gap = 20.0f;

	float windowWidth = width * 3 + gap * 2 + 40.0f;
	float windowHeight = height + 100.0f;

	// 화면 중앙에 배치
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(
		ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
		ImGuiCond_Always,
		ImVec2(0.5f, 0.5f)
	);

	ImGui::SetNextWindowSize(
		ImVec2(windowWidth, windowHeight),
		ImGuiCond_Always
	);

	// 증강 모달 배경
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 15.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 14.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

	ImGui::PushStyleColor(
		ImGuiCol_WindowBg,
		ImVec4(0.04f, 0.05f, 0.07f, 0.96f)
	);

	ImGui::PushStyleColor(
		ImGuiCol_Border,
		ImVec4(0.15f, 0.50f, 0.60f, 0.8f)
	);

	ImGui::Begin(
		"##LevelUp",
		nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse
	);

	std::string texts[3];

	// 증강 텍스트
	texts[0] = augment.GetAugmentText(aug1);
	texts[1] = augment.GetAugmentText(aug2);
	texts[2] = augment.GetAugmentText(aug3);

	// 레벨업 텍스트
	ImGui::SetWindowFontScale(1.6f);

	const char* title = "LEVEL UP!";
	float titleWidth = ImGui::CalcTextSize(title).x;

	ImGui::SetCursorPosX(
		(ImGui::GetWindowWidth() - titleWidth) * 0.5f
	);

	ImGui::Text("%s", title);

	ImGui::SetWindowFontScale(1.0f);

	const char* subtitle = "Choose an Augment";
	float subtitleWidth = ImGui::CalcTextSize(subtitle).x;

	ImGui::SetCursorPosX(
		(ImGui::GetWindowWidth() - subtitleWidth) * 0.5f
	);

	ImGui::TextDisabled("%s", subtitle);

	ImGui::Spacing();
	ImGui::Spacing();

	// 증강 버튼 생성
	for (int i = 0; i < 3; i++)
	{
		ImGui::PushID(i);
		bool enhanced = augments[i]->isEnhanced;

		// 증강 카드
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);

		ImGui::PushStyleColor(
			ImGuiCol_ChildBg,
			selectedAugment == i
			? (enhanced
				? ImVec4(0.24f, 0.14f, 0.06f, 0.95f)   // 선택 + 강화
				: ImVec4(0.11f, 0.16f, 0.20f, 0.95f))  // 선택 + 일반
			: (enhanced
				? ImVec4(0.11f, 0.07f, 0.03f, 0.95f)   // 미선택 + 강화
				: ImVec4(0.05f, 0.07f, 0.09f, 0.95f))  // 미선택 + 일반
		);

		ImGui::PushStyleColor(
			ImGuiCol_Border,
			enhanced
			? ImVec4(1.0f, 0.65f, 0.15f, 1.0f)
			: ImVec4(0.15f, 0.50f, 0.60f, 0.8f)
		);

		ImGui::BeginChild(
			"AugmentCard",
			ImVec2(width, height),
			true,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse
		);

		ImGui::TextDisabled("AUGMENT %d", i + 1);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::SetWindowFontScale(1.2f);

		// 증강 설명
		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x);

		ImGui::TextWrapped("%s", texts[i].c_str());

		ImGui::PopTextWrapPos();

		ImGui::SetWindowFontScale(1.0f);

		// 선택 버튼을 카드 아래에 배치
		float buttonHeight = 42.0f;

		ImGui::SetCursorPosY(
			height - buttonHeight - 15.0f
		);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f);

		if (enhanced)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.45f, 0.08f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.58f, 0.12f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.00f, 0.70f, 0.18f, 1.0f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.45f, 0.55f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.60f, 0.70f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.75f, 0.85f, 1.0f));
		}

		ImGui::PushStyleColor(
			ImGuiCol_Button,
			selectedAugment == i
			? (enhanced
				? ImVec4(1.00f, 0.68f, 0.15f, 1.0f)   // 선택 + 강화
				: ImVec4(0.20f, 0.75f, 0.85f, 1.0f))  // 선택
			: (enhanced
				? ImVec4(0.55f, 0.32f, 0.05f, 1.0f)   // 미선택 + 강화
				: ImVec4(0.06f, 0.28f, 0.34f, 1.0f))  // 미선택
		);

		bool selectPressed = ImGui::Button(
			"SELECT",
			ImVec2(ImGui::GetContentRegionAvail().x, buttonHeight)
		);

		ImGui::PopStyleColor();

		if (selectPressed || (selectedAugment == i && augmentEnterPressed))
		{
			switch (i)
			{
			case 0:
				TimeManager::GetInstance()->TimeResume();
				ObjectManager::GetInstance()->UpgradePlayer(aug1);
				isAugmnetSelected = false;
				openAugmentPopup = false;
				break;

			case 1:
				TimeManager::GetInstance()->TimeResume();
				ObjectManager::GetInstance()->UpgradePlayer(aug2);
				isAugmnetSelected = false;
				openAugmentPopup = false;
				break;

			case 2:
				TimeManager::GetInstance()->TimeResume();
				ObjectManager::GetInstance()->UpgradePlayer(aug3);
				isAugmnetSelected = false;
				openAugmentPopup = false;
				break;
			}

			openAugmentPopup = false;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);

		ImGui::PopID();

		if (i < 2)
			ImGui::SameLine(0.0f, gap);
	}

	ImGui::End();

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(3);
}

void InGameStage::Render()
{
	//ObjectManager에서 오브젝트 랜더함수 호출

	ObjectManager::GetInstance()->Render();

	// ImGui UI 렌더링
	
	// TIME
	float currentTime = TimeManager::GetInstance()->GetcurrentTime();

	int totalSeconds = static_cast<int>(currentTime);
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	RenderHUD(minutes, seconds);
	RenderPauseModal();
	RenderResultModal(minutes, seconds);
	RenderAugmentModal();
}

void InGameStage::RenderHUD(int minutes, int seconds)
{
	score = minutes * 600 + seconds * 10 + ObjectManager::GetInstance()->getKillCount() * 10;

	/////////////////////////
	// InGame 상시 HUD 
	/////////////////////////

	// 전체 화면 투명 오버레이 윈도우
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowBgAlpha(0.0f); // 투명 배경

	ImGuiWindowFlags hudFlags =
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoMouseInputs; // 인게임 마우스 클릭/입력 통과

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	// HUD 창 Begin
	ImGui::Begin("HUD", nullptr, hudFlags);

	//경험치 바
	float screenWidth = viewport->Size.x;
	float screenHeight = viewport->Size.y;
	float expHeight = 10.0f;
	float expRatio = static_cast<float>(player->GetExp()) / player->GetExpTable();
	expRatio = ImClamp(expRatio, 0.0f, 1.0f);

	ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.15f, 0.75f, 0.25f, 1.0f)); // 초록색 경험치 게이지
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.08f, 0.12f, 0.8f));      // 게이지 배경

	ImGui::ProgressBar(expRatio, ImVec2(screenWidth, expHeight), "");
	ImGui::PopStyleColor(2);
	
	// 글씨 크기 억지로 키움
	ImGui::SetWindowFontScale(2.5f);

	// 시간 표기
	char timeStr[32];
	snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);

	float timeTextWidth = ImGui::CalcTextSize(timeStr).x;
	ImGui::SetCursorPos(ImVec2((screenWidth - timeTextWidth) * 0.5f, expHeight + 8.0f));
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", timeStr);

	// 스코어, 레벨
	char levelStr[32], scoreStr[32];
	snprintf(levelStr, sizeof(levelStr), "LV. %d", player->GetLevel());
	snprintf(scoreStr, sizeof(scoreStr), "SCORE: %d", score);

	float margin = 16.0f;
	float maxRightTextWidth = ImMax(ImGui::CalcTextSize(levelStr).x, ImGui::CalcTextSize(scoreStr).x);
	float rightAlignX = screenWidth - maxRightTextWidth - margin;

	ImGui::SetCursorPos(ImVec2(rightAlignX, expHeight + 6.0f));
	ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), "%s", levelStr);

	ImGui::SetCursorPos(ImVec2(rightAlignX, expHeight + 38.0f));
	ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), "%s", scoreStr);

	ImGui::SetWindowFontScale(1.0f);
	
	ImVec2 playerScreenPos(
		(player->GetLocation().x + 1.0f) * 0.5f * screenWidth,
		(1.0f - player->GetLocation().y) * 0.5f * screenHeight // DirectX NDC Y축(위쪽이 +) 반전 처리
	);

	float hpRatio = player->GetHealth() / player->GetMaxHealth();
	hpRatio = ImClamp(hpRatio, 0.0f, 1.0f);

	float hpBarWidth = 56.0f;     // HP 바 가로 폭
	float hpBarHeight = 6.0f;     // HP 바 두께
	float verticalOffset = 36.0f; // 플레이어 중심점 기준 위쪽 간격

	ImVec2 hpBarPos = ImVec2(playerScreenPos.x - (hpBarWidth * 0.5f), playerScreenPos.y + verticalOffset);

	
	ImGui::SetCursorScreenPos(hpBarPos);
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.9f, 0.2f, 0.2f, 1.0f)); // 노란색 HP
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 0.8f));

	ImGui::ProgressBar(hpRatio, ImVec2(hpBarWidth, hpBarHeight), "");

	ImGui::PopStyleColor(2);

	// HUD 창 End
	ImGui::End();
	ImGui::PopStyleVar(2);
}

void InGameStage::RenderPauseModal()
{
	// =========================
	// Pause Modal
	// =========================

	// ESC 키 입력 체크
	bool escPressed = InputManager::GetInstance()->IsKeyTriggered(VK_ESCAPE);

	// 인게임에서 ESC로 Pause 열기
	if (escPressed &&
		!ImGui::IsPopupOpen("Pause Menu") &&
		gameResult == 0)
	{
		openPausePopup = true;
	}

	if (openPausePopup)
	{
		ImGui::OpenPopup("Pause Menu");
		USoundManager::GetInstance()->PlaySFX(UI_OPEN);
		openPausePopup = false;

		TimeManager::GetInstance()->TimePause();

		escPressed = false;
	}

	ImGuiIO& io = ImGui::GetIO();

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
		ImVec2(360.0f, 300.0f),
		ImGuiCond_Always
	);

	// Pause 모달 배경
	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowPadding,
		ImVec2(30.0f, 25.0f)
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
		ImVec4(0.04f, 0.05f, 0.07f, 0.98f)
	);

	ImGui::PushStyleColor(
		ImGuiCol_Border,
		ImVec4(0.15f, 0.50f, 0.60f, 0.8f)
	);

	if (ImGui::BeginPopupModal(
		"Pause Menu",
		nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse))
	{
		const float buttonWidth = 250.0f;
		const float buttonHeight = 42.0f;

		// Pause 텍스트
		ImGui::SetWindowFontScale(1.8f);

		const char* title = "PAUSED";
		float titleWidth =
			ImGui::CalcTextSize(title).x;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() - titleWidth) * 0.5f
		);

		ImGui::Text("%s", title);

		ImGui::SetWindowFontScale(1.0f);

		const char* subtitle = "Game Paused";
		float subtitleWidth =
			ImGui::CalcTextSize(subtitle).x;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() - subtitleWidth) * 0.5f
		);

		ImGui::TextDisabled("%s", subtitle);

		ImGui::Dummy(
			ImVec2(0.0f, 18.0f)
		);

		// 버튼 스타일
		ImGui::PushStyleColor(
			ImGuiCol_Button,
			ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonHovered,
			ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonActive,
			ImVec4(0.20f, 0.75f, 0.85f, 1.0f)
		);

		// RESUME 버튼

		// ESC 키 입력 시 Leave Game? 팝업이 열려있으면 RESUME 버튼이 작동하지 않도록 체크
		bool leaveGameOpen =
			ImGui::IsPopupOpen("Leave Game?");

		bool settingsOpen =
			ImGui::IsPopupOpen("Settings");

		if (settingsOpen)
		{
			InputManager::GetInstance()->IsKeyTriggered(VK_RETURN);
		}

		enterPressed = false;

		if (!leaveGameOpen && !settingsOpen)
		{
			if (InputManager::GetInstance()->IsKeyTriggered(VK_UP) || InputManager::GetInstance()->IsKeyTriggered('W'))
			{
				selectedPauseMenu--;

				if (selectedPauseMenu < 0)
					selectedPauseMenu = PAUSE_COUNT - 1;
			}

			if (InputManager::GetInstance()->IsKeyTriggered(VK_DOWN) || InputManager::GetInstance()->IsKeyTriggered('S'))
			{
				selectedPauseMenu++;

				if (selectedPauseMenu >= PAUSE_COUNT)
					selectedPauseMenu = 0;
			}

			enterPressed = InputManager::GetInstance()->IsKeyTriggered(VK_RETURN);
		}

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() - buttonWidth) * 0.5f
		);

		ImGui::PushStyleColor(
			ImGuiCol_Button,
			selectedPauseMenu == PAUSE_RESUME
			? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
			: ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
		);

		bool resumePressed = ImGui::Button(
			"RESUME",
			ImVec2(buttonWidth, buttonHeight)
		);

		ImGui::PopStyleColor();

		if (resumePressed ||
			(selectedPauseMenu == PAUSE_RESUME &&
				enterPressed &&
				!leaveGameOpen &&
				!settingsOpen) ||
			(escPressed &&
				!leaveGameOpen &&
				!settingsOpen))
		{
			if (!openAugmentPopup)
			{
				TimeManager::GetInstance()->TimeResume();
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::Dummy(
			ImVec2(0.0f, 10.0f)
		);

		// =========================
		// Settings
		// =========================

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() - buttonWidth) * 0.5f
		);

		ImGui::PushStyleColor(
			ImGuiCol_Button,
			selectedPauseMenu == PAUSE_SETTINGS
			? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
			: ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
		);

		bool settingsPressed = ImGui::Button(
			"SETTINGS",
			ImVec2(buttonWidth, buttonHeight)
		);

		ImGui::PopStyleColor();

		if (settingsPressed ||
			(selectedPauseMenu == PAUSE_SETTINGS &&
				enterPressed &&
				!leaveGameOpen &&
				!settingsOpen))
		{
			ImGui::OpenPopup("Settings");
		}

		SettingsUI::Render();

		ImGui::Dummy(
			ImVec2(0.0f, 10.0f)
		);

		// MAIN MENU 버튼
		static bool openMainMenuConfirm = false;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() - buttonWidth) * 0.5f
		);

		ImGui::PushStyleColor(
			ImGuiCol_Button,
			selectedPauseMenu == PAUSE_MAIN_MENU
			? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
			: ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
		);

		bool mainMenuPressed = ImGui::Button(
			"MAIN MENU",
			ImVec2(buttonWidth, buttonHeight)
		);

		ImGui::PopStyleColor();

		if (mainMenuPressed ||
			(selectedPauseMenu == PAUSE_MAIN_MENU &&
				enterPressed &&
				!leaveGameOpen &&
				!settingsOpen))
		{
			openMainMenuConfirm = true;
		}

		ImGui::PopStyleColor(3);

		// =========================
		// Main Menu Confirm: 게임 진행 중 메인 메뉴로 돌아갈 때 경고창 팝업
		// =========================

		if (openMainMenuConfirm)
		{
			ImGui::OpenPopup("Leave Game?");
			openMainMenuConfirm = false;
		}

		// 경고창 화면 중앙에 배치
		ImGui::SetNextWindowPos(
			ImVec2(
				io.DisplaySize.x * 0.5f,
				io.DisplaySize.y * 0.5f
			),
			ImGuiCond_Appearing,
			ImVec2(0.5f, 0.5f)
		);

		ImGui::SetNextWindowSize(
			ImVec2(340.0f, 190.0f),
			ImGuiCond_Always
		);

		// Leave Game 모달 배경
		ImGui::PushStyleVar(
			ImGuiStyleVar_WindowPadding,
			ImVec2(25.0f, 22.0f)
		);

		ImGui::PushStyleVar(
			ImGuiStyleVar_WindowRounding,
			12.0f
		);

		ImGui::PushStyleVar(
			ImGuiStyleVar_WindowBorderSize,
			1.0f
		);

		ImGui::PushStyleColor(
			ImGuiCol_PopupBg,
			ImVec4(0.04f, 0.05f, 0.07f, 0.99f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_Border,
			ImVec4(0.15f, 0.50f, 0.60f, 0.8f)
		);

		if (ImGui::BeginPopupModal(
			"Leave Game?",
			nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse))
		{
			if (InputManager::GetInstance()->IsKeyTriggered(VK_LEFT) || InputManager::GetInstance()->IsKeyTriggered('A'))
			{
				selectedLeaveMenu--;

				if (selectedLeaveMenu < 0)
					selectedLeaveMenu = LEAVE_COUNT - 1;
			}

			if (InputManager::GetInstance()->IsKeyTriggered(VK_RIGHT) || InputManager::GetInstance()->IsKeyTriggered('D'))
			{
				selectedLeaveMenu++;

				if (selectedLeaveMenu >= LEAVE_COUNT)
					selectedLeaveMenu = 0;
			}

			bool leaveEnterPressed = InputManager::GetInstance()->IsKeyTriggered(VK_RETURN);


			// 경고창 제목
			ImGui::SetWindowFontScale(1.5f);

			const char* confirmTitle =
				"LEAVE GAME?";

			float confirmTitleWidth =
				ImGui::CalcTextSize(confirmTitle).x;

			ImGui::SetCursorPosX(
				(ImGui::GetWindowWidth() -
					confirmTitleWidth) * 0.5f
			);

			ImGui::Text("%s", confirmTitle);

			ImGui::SetWindowFontScale(1.0f);

			ImGui::Dummy(
				ImVec2(0.0f, 5.0f)
			);

			const char* warningText =
				"Current progress will be lost.";

			float warningWidth =
				ImGui::CalcTextSize(warningText).x;

			ImGui::SetCursorPosX(
				(ImGui::GetWindowWidth() -
					warningWidth) * 0.5f
			);

			ImGui::TextDisabled(
				"%s",
				warningText
			);

			ImGui::Dummy(
				ImVec2(0.0f, 18.0f)
			);

			const float confirmButtonWidth = 125.0f;
			const float confirmButtonHeight = 40.0f;
			const float confirmGap = 15.0f;

			float totalButtonWidth =
				confirmButtonWidth * 2.0f
				+ confirmGap;

			ImGui::SetCursorPosX(
				(ImGui::GetWindowWidth() -
					totalButtonWidth) * 0.5f
			);

			// YES 버튼
			ImGui::PushStyleColor(
				ImGuiCol_Button,
				ImVec4(0.40f, 0.16f, 0.18f, 1.0f)
			);

			ImGui::PushStyleColor(
				ImGuiCol_ButtonHovered,
				ImVec4(0.55f, 0.20f, 0.22f, 1.0f)
			);

			ImGui::PushStyleColor(
				ImGuiCol_ButtonActive,
				ImVec4(0.65f, 0.24f, 0.26f, 1.0f)
			);

			ImGui::PushStyleColor(
				ImGuiCol_Button,
				selectedLeaveMenu == LEAVE_YES
				? ImVec4(0.75f, 0.28f, 0.30f, 1.0f)
				: ImVec4(0.40f, 0.16f, 0.18f, 1.0f)
			);

			bool yesPressed = ImGui::Button(
				"YES",
				ImVec2(confirmButtonWidth, confirmButtonHeight)
			);

			ImGui::PopStyleColor();

			if (yesPressed ||
				(selectedLeaveMenu == LEAVE_YES && leaveEnterPressed))
			{
				ImGui::CloseCurrentPopup();
				m_app->ChangeState(new MainmenuStage(m_app));
			}

			ImGui::PopStyleColor(3);

			ImGui::SameLine(
				0.0f,
				confirmGap
			);

			// NO 버튼
			ImGui::PushStyleColor(
				ImGuiCol_Button,
				ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
			);

			ImGui::PushStyleColor(
				ImGuiCol_ButtonHovered,
				ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
			);

			ImGui::PushStyleColor(
				ImGuiCol_ButtonActive,
				ImVec4(0.20f, 0.75f, 0.85f, 1.0f)
			);

			ImGui::PushStyleColor(
				ImGuiCol_Button,
				selectedLeaveMenu == LEAVE_NO
				? ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
				: ImVec4(0.07f, 0.30f, 0.36f, 1.0f)
			);

			bool noPressed = ImGui::Button(
				"NO",
				ImVec2(confirmButtonWidth, confirmButtonHeight)
			);

			ImGui::PopStyleColor();

			if (noPressed ||
				(selectedLeaveMenu == LEAVE_NO && leaveEnterPressed))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::PopStyleColor(3);

			// 경고창에서 ESC = 취소
			if (escPressed)
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(3);

		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(4);
}

void InGameStage::RenderResultModal(int minutes, int seconds)
{
	// =========================
	// Result Modal: 게임 클리어 또는 게임 오버 시의 결과창 팝업
	// =========================

	static bool isSubmitted = false;
	static char nickname[32] = "";

	if (openResultPopup)
	{
		ImGui::OpenPopup("Game Result");
		openResultPopup = false;
	}

	ImGuiIO& io = ImGui::GetIO();

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
		ImVec2(460.0f, 390.0f),
		ImGuiCond_Always
	);

	// Result 모달 배경
	ImGui::PushStyleVar(
		ImGuiStyleVar_WindowPadding,
		ImVec2(30.0f, 25.0f)
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
		ImVec4(0.04f, 0.05f, 0.07f, 0.98f)
	);

	ImGui::PushStyleColor(
		ImGuiCol_Border,
		ImVec4(0.15f, 0.50f, 0.60f, 0.8f)
	);

	if (ImGui::BeginPopupModal(
		"Game Result",
		nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse))
	{
		// 렌더 루프 안
		if (gameResult != PrevGameResult)
		{
			if (gameResult == 1)
			{
				USoundManager::GetInstance()->StopBGM();
				USoundManager::GetInstance()->PlaySFX(GAME_CLEAR);
			}
			else if (gameResult == 2)
			{
				USoundManager::GetInstance()->StopBGM();
				USoundManager::GetInstance()->PlaySFX(GAME_OVER);
			}

			PrevGameResult = gameResult;
		}

		// 텍스트는 매 프레임 그려져야 하므로 밖으로 분리
		const char* resultText = "";

		if (gameResult == 1)
			resultText = "GAME CLEAR!";
		else if (gameResult == 2)
			resultText = "GAME OVER";

		// 결과 텍스트
		ImGui::SetWindowFontScale(2.0f);

		float resultTextWidth =
			ImGui::CalcTextSize(resultText).x;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() - resultTextWidth) * 0.5f
		);

		if (gameResult == 1)
		{
			ImGui::TextColored(
				ImVec4(0.25f, 0.80f, 0.85f, 1.0f),
				"%s",
				resultText
			);
		}
		else
		{
			ImGui::TextColored(
				ImVec4(0.85f, 0.32f, 0.32f, 1.0f),
				"%s",
				resultText
			);
		}

		ImGui::SetWindowFontScale(1.0f);

		const char* subtitle =
			gameResult == 1
			? "Mission Complete"
			: "Mission Failed";

		float subtitleWidth =
			ImGui::CalcTextSize(subtitle).x;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() - subtitleWidth) * 0.5f
		);

		ImGui::TextDisabled("%s", subtitle);

		ImGui::Dummy(ImVec2(0.0f, 18.0f));

		// 플레이 결과
		char timeValue[32];
		sprintf_s(
			timeValue,
			"%02d:%02d",
			minutes,
			seconds
		);

		char scoreValue[32];
		sprintf_s(
			scoreValue,
			"%d",
			score
		);

		// TIME / SCORE를 각각 같은 폭의 영역에 가운데 정렬
		const float columnGap = 60.0f;
		const float columnWidth = 120.0f;
		const float totalWidth =
			columnWidth * 2.0f + columnGap;

		const float startX =
			(ImGui::GetWindowWidth() - totalWidth) * 0.5f;

		const float timeCenter =
			startX + columnWidth * 0.5f;

		const float scoreCenter =
			startX + columnWidth + columnGap + columnWidth * 0.5f;

		// TIME / SCORE 라벨
		const char* timeLabel = "TIME";
		const char* scoreLabel = "SCORE";

		ImGui::SetCursorPosX(
			timeCenter - ImGui::CalcTextSize(timeLabel).x * 0.5f
		);
		ImGui::TextDisabled("%s", timeLabel);

		ImGui::SameLine();

		ImGui::SetCursorPosX(
			scoreCenter - ImGui::CalcTextSize(scoreLabel).x * 0.5f
		);
		ImGui::TextDisabled("%s", scoreLabel);

		// TIME / SCORE 값
		ImGui::SetWindowFontScale(1.5f);

		float timeWidth = ImGui::CalcTextSize(timeValue).x;
		float scoreWidth = ImGui::CalcTextSize(scoreValue).x;

		ImGui::SetCursorPosX(
			timeCenter - timeWidth * 0.5f
		);
		ImGui::Text("%s", timeValue);

		ImGui::SameLine();

		ImGui::SetCursorPosX(
			scoreCenter - scoreWidth * 0.5f
		);
		ImGui::Text("%s", scoreValue);

		ImGui::SetWindowFontScale(1.0f);

		ImGui::Dummy(ImVec2(0.0f, 15.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// 리더보드 입력 구현
		const float viewButtonWidth = 150.0f;
		const float viewButtonHeight = 30.0f;

		// 리더보드 입력 구현
		ImGui::Text("SAVE SCORE");
		ImGui::TextDisabled("Enter a nickname for the leaderboard");

		ImGui::Dummy(ImVec2(0.0f, 6.0f));

		ImGui::BeginDisabled(isSubmitted);

		const float submitButtonWidth = 100.0f;
		const float inputButtonGap = 10.0f;

		float inputWidth =
			ImGui::GetContentRegionAvail().x
			- submitButtonWidth
			- inputButtonGap;

		// 닉네임 입력창
		ImGui::SetNextItemWidth(inputWidth);

		ImGui::InputTextWithHint(
			"##Nickname",
			"Nickname",
			nickname,
			IM_ARRAYSIZE(nickname)
		);

		float inputHeight = ImGui::GetItemRectSize().y;

		ImGui::SameLine(0.0f, inputButtonGap);

		// SUBMIT 버튼
		ImGui::PushStyleColor(
			ImGuiCol_Button,
			ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonHovered,
			ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonActive,
			ImVec4(0.20f, 0.75f, 0.85f, 1.0f)
		);

		const char* buttonText =
			isSubmitted ? "SAVED" : "SUBMIT";

		if (ImGui::Button(
			buttonText,
			ImVec2(submitButtonWidth, inputHeight)))
		{
			if (strlen(nickname) > 0)
			{
				// LeaderboardManager 등에 전달
				LeaderboardManager::GetInstance()->AddScore(
					nickname,
					score
				);

				// 입력 후 버퍼 초기화 (필요 시)
				nickname[0] = '\0';
				isSubmitted = true;
			}
		}

		ImGui::PopStyleColor(3);

		ImGui::EndDisabled();

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		// 리더보드 확인
		ImGui::PushStyleColor(
			ImGuiCol_Button,
			ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonHovered,
			ImVec4(0.10f, 0.25f, 0.30f, 0.55f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonActive,
			ImVec4(0.12f, 0.35f, 0.42f, 0.7f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_Text,
			ImVec4(0.45f, 0.70f, 0.75f, 1.0f)
		);

		ImGui::PushStyleVar(
			ImGuiStyleVar_FramePadding,
			ImVec2(4.0f, 3.0f)
		);

		if (ImGui::Button("VIEW LEADERBOARD >"))
		{
			ImGui::OpenPopup("Leaderboard");
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor(4);

		// 기존 Leaderboard UI 사용
		LeaderboardUI::Render();

		ImGui::Dummy(ImVec2(0.0f, 14.0f));

		// RESTART / MAIN MENU 버튼
		const float buttonWidth = 180.0f;
		const float buttonHeight = 42.0f;
		const float gap = 15.0f;

		float totalButtonWidth =
			buttonWidth * 2.0f + gap;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowWidth() - totalButtonWidth) * 0.5f
		);

		ImGui::PushStyleColor(
			ImGuiCol_Button,
			ImVec4(0.10f, 0.45f, 0.55f, 1.0f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonHovered,
			ImVec4(0.15f, 0.60f, 0.70f, 1.0f)
		);

		ImGui::PushStyleColor(
			ImGuiCol_ButtonActive,
			ImVec4(0.20f, 0.75f, 0.85f, 1.0f)
		);

		if (ImGui::Button(
			"RESTART",
			ImVec2(buttonWidth, buttonHeight)))
		{
			ImGui::CloseCurrentPopup();

			m_app->ChangeState(
				new InGameStage(m_app)
			);

			isSubmitted = false;
			nickname[0] = '\0';
		}

		ImGui::SameLine(0.0f, gap);

		if (ImGui::Button(
			"MAIN MENU",
			ImVec2(buttonWidth, buttonHeight)))
		{
			ImGui::CloseCurrentPopup();

			m_app->ChangeState(
				new MainmenuStage(m_app)
			);

			isSubmitted = false;
			nickname[0] = '\0';
		}

		ImGui::PopStyleColor(3);

		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(4);
}

void InGameStage::Exit()
{
	ObjectManager::GetInstance()->ReleaseAllObjects();
}


