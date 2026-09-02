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

	ObjectManager::GetInstance()->CreateWeapon();
	ObjectManager::GetInstance()->CreateWeapon();
	ObjectManager::GetInstance()->CreateWeapon();
	ObjectManager::GetInstance()->CreateWeapon();
	ObjectManager::GetInstance()->CreateWeapon();

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
	if (InputManager::GetInstance()->IsKeyPressed(VK_UP))    moveDir.y += 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_DOWN))  moveDir.y -= 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_LEFT))  moveDir.x -= 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_RIGHT)) moveDir.x += 1.0f;
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


	// 경험치 상승 임시구현
	player->AddExp(1);

	// 레벨업 경험치 도달 시 일시정지 및 팝업 on
	if (player->IsLevelUp()) {
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
		"test",
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
	/////////////////////////
	// InGame 상시 HUD 
	/////////////////////////

	ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_Always);

	ImGuiWindowFlags hudFlags =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_AlwaysAutoResize;

	// HUD 창 Begin
	ImGui::Begin("HUD", nullptr, hudFlags);

	// HP
	ImGui::Text("HP");

	float hpRatio = player->GetHealth() / player->GetMaxHealth();

	char hpText[32];
	sprintf_s(hpText, "%.0f / %.0f", player->GetHealth(), player->GetMaxHealth());

	ImGui::ProgressBar(
		hpRatio,
		ImVec2(300.0f, 25.0f),
		hpText
	);

	ImGui::Text("Level : %d", player->GetLevel());
	ImGui::Text("EXP : %d/%d", player->GetExp(), player->GetExpTable());
	score = seconds * 10 + ObjectManager::GetInstance()->getKillCount() * 10;

	ImGui::Text("TIME  %02d:%02d", minutes, seconds);
	ImGui::Text("Score: %d", score);
	ImGui::Separator();

	// 임시 테스트 버튼
	// TODO: 게임 종료조건 API 붙으면 삭제 예정
	// +) Clear 조건은 붙었으나 테스트용으로 남겨둠
	if (ImGui::Button("Preview Clear"))
	{
		gameResult = 1;
		openResultPopup = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Preview Game Over"))
	{
		gameResult = 2;
		openResultPopup = true;
	}

	ImGui::Separator();

	// Pause 열기
	if (ImGui::Button("||"))
	{
		
		openPausePopup = true;
	}

	// HUD 창 End
	ImGui::End();
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
	if (escPressed && !ImGui::IsPopupOpen("Pause Menu"))
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
		ImVec2(320.0f, 220.0f),
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

		if (ImGui::Button(
			"RESUME",
			ImVec2(buttonWidth, buttonHeight)) || (escPressed && !leaveGameOpen))
		{
			TimeManager::GetInstance()->TimeResume();
			ImGui::CloseCurrentPopup();
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

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


