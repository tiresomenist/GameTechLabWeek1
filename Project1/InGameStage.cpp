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

}

void InGameStage::RenderAugmentModal()
{
	if (!openAugmentPopup)
		return;
	
	//증강 생성 되었는지
	if (!isAugmnetSelected) {
		//증강 3개 랜덤뽑기
		aug1 = augment.GetAugmentStruct();
		aug2 = augment.GetAugmentStruct();
		aug3 = augment.GetAugmentStruct();
		augment.ResetAugment();

		isAugmnetSelected = true;
	}

	float width = 160.0f;
	float height = 220.0f;
	float gap = 30.0f;

	float windowWidth = width * 3 + gap * 2 + 40.0f;
	float windowHeight = height + 60.0f;

	ImGui::SetNextWindowSize(
		ImVec2(windowWidth, windowHeight),
		ImGuiCond_Always
	);

	ImGui::Begin(
		"Upgrade!!",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse
	);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	std::string texts[3];

	// 증강 텍스트
	texts[0] = augment.GetAugmentText(aug1);
	texts[1] = augment.GetAugmentText(aug2);
	texts[2] = augment.GetAugmentText(aug3);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.45f, 0.55f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.60f, 0.70f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.75f, 0.85f, 1.0f));

	ImGui::SetWindowFontScale(1.4f);
	ImGui::PushStyleVar(
		ImGuiStyleVar_ButtonTextAlign,
		ImVec2(0.5f, 0.4f)
	);

	// 증강 버튼 생성
	for (int i = 0; i < 3; i++)
	{
		if (ImGui::Button(texts[i].c_str(), ImVec2(width, height)))
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

		if (i < 2)
			ImGui::SameLine(0.0f, gap);

	}
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();
	ImGui::SetWindowFontScale(1.0f);

	ImGui::End();
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
	static bool prevEscDown = false;

	bool escDown = InputManager::GetInstance()->IsKeyPressed(VK_ESCAPE);

	bool escPressed = escDown && !prevEscDown;

	prevEscDown = escDown;

	// 인게임에서 ESC로 Pause 열기
	if (escPressed && !ImGui::IsPopupOpen("Pause Menu") && gameResult == 0)
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

	ImGui::SetNextWindowSize(
		ImVec2(320.0f, 280.0f),
		ImGuiCond_Always
	);

	if (ImGui::BeginPopupModal(
		"Pause Menu",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse))
	{
		const float buttonWidth = 220.0f;
		const float buttonHeight = 40.0f;

		ImGui::Text("PAUSED");
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 15.0f));

		// RESUME 버튼

		// ESC 키 입력 시 Leave Game? 팝업이 열려있으면 RESUME 버튼이 작동하지 않도록 체크
		bool leaveGameOpen = ImGui::IsPopupOpen("Leave Game?");

		ImGui::SetCursorPosX(
			(ImGui::GetWindowSize().x - buttonWidth) * 0.5f
		);

		if (ImGui::Button("RESUME",ImVec2(buttonWidth, buttonHeight)) || (escPressed && !leaveGameOpen))
		{
			if(!openAugmentPopup)
				TimeManager::GetInstance()->TimeResume();
			ImGui::CloseCurrentPopup();
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// =========================
		// Settings 
		// =========================

		ImGui::SetCursorPosX(
			(ImGui::GetWindowSize().x - buttonWidth) * 0.5f
		);

		if (ImGui::Button(
			"SETTINGS",
			ImVec2(buttonWidth, buttonHeight)))
		{
			ImGui::OpenPopup("Settings");
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		SettingsUI::Render();


		// MAIN MENU 버튼
		static bool openMainMenuConfirm = false;

		ImGui::SetCursorPosX(
			(ImGui::GetWindowSize().x - buttonWidth) * 0.5f
		);

		if (ImGui::Button(
			"MAIN MENU",
			ImVec2(buttonWidth, buttonHeight)))
		{
			openMainMenuConfirm = true;
		}

	


		// =========================
		// Main Menu Confirm: 게임 진행 중 메인 메뉴로 돌아갈 때 경고창 팝업
		// =========================

		if (openMainMenuConfirm)
		{
			ImGui::OpenPopup("Leave Game?");
			openMainMenuConfirm = false;
		}

		ImGui::SetNextWindowSize(
			ImVec2(300.0f, 170.0f),
			ImGuiCond_Always
		);

		if (ImGui::BeginPopupModal(
			"Leave Game?",
			nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse))
		{
			ImGui::Text("Return to Main Menu?");
			ImGui::Text("Current progress will be lost.");

			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			const float confirmButtonWidth = 100.0f;

			float totalButtonWidth =
				confirmButtonWidth * 2.0f
				+ ImGui::GetStyle().ItemSpacing.x;

			ImGui::SetCursorPosX(
				(ImGui::GetWindowSize().x - totalButtonWidth) * 0.5f
			);

			// YES 버튼
			if (ImGui::Button(
				"YES",
				ImVec2(confirmButtonWidth, 35.0f)))
			{
				ImGui::CloseCurrentPopup();

				m_app->ChangeState(
					new MainmenuStage(m_app)
				);
			}

			ImGui::SameLine();

			// NO 버튼
			if (ImGui::Button(
				"NO",
				ImVec2(confirmButtonWidth, 35.0f)))
			{
				ImGui::CloseCurrentPopup();
			}

			// 경고창에서 ESC = 취소
			if (ImGui::IsKeyPressed(ImGuiKey_Escape))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndPopup();
	}
}

void InGameStage::RenderResultModal(int minutes, int seconds)
{
	// =========================
	// Result Modal: 게임 클리어 또는 게임 오버 시의 결과창 팝업
	// =========================

	static bool isSubmitted = false;
	if (openResultPopup)
	{
		ImGui::OpenPopup("Game Result");
		openResultPopup = false;
	}

	ImGui::SetNextWindowSize(
		ImVec2(360.0f, 250.0f),
		ImGuiCond_Always
	);

	if (ImGui::BeginPopupModal(
		"Game Result",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse))
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
		if (gameResult == 1)
			ImGui::Text("GAME CLEAR!");
		else if (gameResult == 2)
			ImGui::Text("GAME OVER");

		ImGui::Separator();

		ImGui::Text(
			"Elapsed Time : %02d:%02d",
			minutes,
			seconds
		);

		ImGui::Text("Score : %d", score);

		ImGui::Separator();

		ImGui::BeginDisabled(isSubmitted);
		// 리더보드 입력 구현
		static char nickname[32] = "";
		ImGui::InputText("Nickname", nickname, IM_ARRAYSIZE(nickname));
		const char* buttonText = isSubmitted ? "Submitted" : "Submit";

		if (ImGui::Button(buttonText)) {
			if (strlen(nickname) > 0) {
				// LeaderboardManager 등에 전달
				LeaderboardManager::GetInstance()->AddScore(nickname, score);

				// 입력 후 버퍼 초기화 (필요 시)
				nickname[0] = '\0';
				isSubmitted = true;
			}
		}

		ImGui::Separator();
		ImGui::EndDisabled();

		if (ImGui::Button("RESTART", ImVec2(140.0f, 40.0f)))
		{
			ImGui::CloseCurrentPopup();
			m_app->ChangeState(new InGameStage(m_app));
			isSubmitted = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("MAIN MENU", ImVec2(140.0f, 40.0f)))
		{
			ImGui::CloseCurrentPopup();
			m_app->ChangeState(new MainmenuStage(m_app));
			isSubmitted = false;

		}

		ImGui::EndPopup();
	}
}

void InGameStage::Exit()
{
	ObjectManager::GetInstance()->ReleaseAllObjects();
}


