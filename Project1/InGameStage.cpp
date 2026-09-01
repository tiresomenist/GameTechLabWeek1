#include "InGameStage.h"
#include "App.h"
#include "MainmenuStage.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"

#include "ObjectManager.h"

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
	if (objectList == nullptr)
	{
		objectList = new std::vector<Object*>(); // 임시로 생성
	}
	TimeManager::GetInstance()->TimeReset();
	TimeManager::GetInstance()->TimeStart();

	gameResult = 0;

	//테스트용으로 적한번 찍어본거.
	ObjectManager::GetInstance()->CreateEnemy();
}

void InGameStage::Update(float deltaTime)
{
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
		countTimeForEnemy -= 2.0f-(difficulty * 0.3f);
		ObjectManager::GetInstance()->CreateEnemy();
		OutputDebugStringA("Enemy Creted!\n");

	}

	////플레이어 이동
	FVector moveDir(0.0f, 0.0f, 0.0f);
	if (InputManager::GetInstance()->IsKeyPressed(VK_UP))    moveDir.y += 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_DOWN))  moveDir.y -= 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_LEFT))  moveDir.x -= 1.0f;
	if (InputManager::GetInstance()->IsKeyPressed(VK_RIGHT)) moveDir.x += 1.0f;
	//인풋매니저 가져오고 델타타임이랑 이동속도 생각해서 한프레임당 이동 거리 계산해서 move호출
	
	player->MoveObject(moveDir.x * deltaTime * player->GetSpeed(), moveDir.y * deltaTime * player->GetSpeed());
	// 플레이어 공격
	if (countTimeForPlayer > (1.0f / player->GetAttackSpeed())) {
		countTimeForPlayer -= (1.0f / player->GetAttackSpeed());
		//공격 범위를 그리는 함수가 필요합니다.
		//컬러값 (1.0f,0.0f,0.0f,0.5f)로 반지름 0.08f. 반투명 빨간색 생각중.
		//CheckHitCollision(player->GetAttackRange());
	}
	//적들 이동
	ObjectManager::GetInstance()->EnemyMove(deltaTime);
	//적들끼리 충돌하는지 체크 
	ObjectManager::GetInstance()->checkEnemiesIntersect();
	//플레이어에게 충돌하는지 체크
	ObjectManager::GetInstance()->checkPlayerIntersectWithEnemy();
	//플레이어와 벽의 충돌 체크
	ObjectManager::GetInstance()->intersectsPlayerWithWall();
}

void InGameStage::Render()
{
	//ObjectManager에서 오브젝트 랜더함수 호출

	ObjectManager::GetInstance()->Render();


	/////////////////////////
	// InGame ImGui 시작
	/////////////////////////

	// 임시 UI 데이터
    static float tempHP = 75.0f;
    static float tempMaxHP = 100.0f;
    static int tempExp = 120;

	bool openPausePopup = false;

	// ESC 키 입력 체크
	static bool prevEscDown = false;

	bool escDown =
		InputManager::GetInstance()->IsKeyPressed(VK_ESCAPE);

	bool escPressed =
		escDown && !prevEscDown;

	prevEscDown = escDown;

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

    float hpRatio = tempHP / tempMaxHP;

    char hpText[32];
    sprintf_s(hpText, "%.0f / %.0f", tempHP, tempMaxHP);

    ImGui::ProgressBar(
        hpRatio,
        ImVec2(300.0f, 25.0f),
        hpText
    );

    // TIME
	float currentTime = TimeManager::GetInstance()->GetcurrentTime();

	int totalSeconds = static_cast<int>(currentTime);
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	ImGui::Text("TIME  %02d:%02d", minutes, seconds);

    ImGui::Separator();

    // 임시 테스트 버튼
	// TODO: 게임 종료조건 API 붙으면 삭제 예정
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

	// =========================
	// Pause Modal
	// =========================

	// 인게임에서 ESC로 Pause 열기
	if (escPressed && !ImGui::IsPopupOpen("Pause Menu"))
	{
		openPausePopup = true;
	}

	if (openPausePopup)
	{
		ImGui::OpenPopup("Pause Menu");
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

	// =========================
	// Result Modal: 게임 클리어 또는 게임 오버 시의 결과창 팝업
	// =========================

    if (openResultPopup)
    {
        ImGui::OpenPopup("Game Result");
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
        if (gameResult == 1)
        {
            ImGui::Text("CLEAR!");
        }
        else if (gameResult == 2)
        {
            ImGui::Text("GAME OVER");
        }

        ImGui::Separator();

        ImGui::Text(
            "Remaining Time : %02d:%02d",
            minutes,
            seconds
        );

        ImGui::Text("EXP : %d", tempExp);

        ImGui::Separator();

        if (ImGui::Button("RESTART", ImVec2(140.0f, 40.0f)))
        {
            ImGui::CloseCurrentPopup();
            m_app->ChangeState(new InGameStage(m_app));
        }

        ImGui::SameLine();

        if (ImGui::Button("MAIN MENU", ImVec2(140.0f, 40.0f)))
        {
			ImGui::CloseCurrentPopup();
            m_app->ChangeState(new MainmenuStage(m_app));
        }

        ImGui::EndPopup();

		/////////////////////////
		// InGame ImGui 끝
		/////////////////////////
    }
}

void InGameStage::Exit()
{
	ObjectManager::GetInstance()->ReleaseAllObjects();
}


