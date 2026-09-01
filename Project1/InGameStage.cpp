#include "InGameStage.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"

void InGameStage::Enter()
{
	//여기서 플레이어 캐릭터를 생성
	if (player == nullptr) {
		player = new Player(); // 임시로 생성
	}
	if (objectList == nullptr)
	{
		objectList = new std::vector<Object*>(); // 임시로 생성
	}
	timeManager = TimeManager();
}

void InGameStage::Update(float deltaTime)
{
	float frameCount = timeManager.GetcurrentTime() / deltaTime;	//몇프레임돌았는가?
	if ((int)frameCount % 30 == 0) {
		//여기에 적생성 로직
		Object* enemy = new Enemy();
		objectList->push_back(enemy);
	}

	//플레이어 이동
	
	//player.move(player.inputVector.x, player.inputVector.y);
	// 플레이어 공격
	//player->GetAttackRange();
	
	//적들 이동
	for (auto object : *objectList)
	{
		float targetX = player->GetLocation().x - object->GetLocation().x;
		float targetY = player->GetLocation().y - object->GetLocation().y;
		object->MoveObject(targetX, targetY);
	}
	//적들끼리 충돌하는지 체크
	for (auto object : *objectList)
	{
		InGameStage::intersects(object);
	}
	//플레이어에게 충돌하는지 체크
	InGameStage::intersectsToPlayer();
	//플레이어와 벽의 충돌 체크
	InGameStage::intersectsPlayerWithWall();
	Render();
}

void InGameStage::Render()
{
	//ObjectManager에서 오브젝트 랜더함수 호출
	for (auto object : *objectList)
	{
		//object->Render();
	}
	ImGui::Begin("In-Game Menu");
	ImGui::End();
    static float tempHP = 75.0f;
    static float tempMaxHP = 100.0f;
    static float tempRemainingTime = 83.0f;
    static int tempExp = 120;

    // 0 = None, 1 = Clear, 2 = Game Over
    static int tempResult = 0;

    bool openResultPopup = false;

    ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_Always);

    ImGuiWindowFlags hudFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_AlwaysAutoResize;

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
    int totalSeconds = static_cast<int>(tempRemainingTime);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    ImGui::Text("TIME  %02d:%02d", minutes, seconds);

    ImGui::Separator();

    // 임시 테스트 버튼
    if (ImGui::Button("Preview Clear"))
    {
        tempResult = 1;
        openResultPopup = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Preview Game Over"))
    {
        tempResult = 2;
        openResultPopup = true;
    }

    ImGui::End();


    // 팝업은 HUD 밖에서 열기
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
        if (tempResult == 1)
        {
            ImGui::Text("CLEAR!");
        }
        else if (tempResult == 2)
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
            m_app->ChangeState(new MainmenuStage(m_app));
        }

        ImGui::EndPopup();
    }
}

void InGameStage::Exit()
{
	//자원 전부 해제
	delete player;
	player = nullptr;
	delete objectList;
	objectList = nullptr;
}

//적군끼리의 충돌 체크
void InGameStage::intersects(Object* object)
{
	// 각 오브젝트들에 대해 순회
	for (auto ohterObject : *objectList)
	{
		if (object->Intersect(ohterObject)) {
			//여기에 충돌관련 처리
			//충돌한만큼 서로 밀어내기/적군과 플레이어간 충돌이면 플레이어 체력 피해
			// 법선벡터*겹쳐진범위/2만큼 서로 밀어내면 됨.(질량은 고려하지않음)
			float Dx = object->GetLocation().x - ohterObject->GetLocation().x;
			float Dy = object->GetLocation().y - ohterObject->GetLocation().y;
			float Distance = sqrt(Dx * Dx + Dy * Dy);
			float overlap = (object->GetRadius() + ohterObject->GetRadius()) - Distance;
			float pushOffsetX = Dx * overlap / (2 * Distance);
			float pushOffsetY = Dy * overlap / (2 * Distance);
			object->MoveObject(pushOffsetX, pushOffsetY);
			ohterObject->MoveObject(-pushOffsetX, -pushOffsetY);
		}
	}
}

//적과 플레이어의 충돌 체크
void InGameStage::intersectsToPlayer()
{
	// 각 오브젝트들에 대해 순회
	//만약 충돌하면 플레이어의 체력을 감소시키고, 체력이 0이되면 게임오버 처리
	for (auto ohterObject : *objectList)
	{
		if (player->Intersect(ohterObject)) {
			//여기에 충돌관련 처리
			//충돌한만큼 서로 밀어내기/플레이어 체력 피해
			// 법선벡터*겹쳐진범위/2만큼 서로 밀어내면 됨.(질량은 고려하지않음)
			float Dx = player->GetLocation().x - ohterObject->GetLocation().x;
			float Dy = player->GetLocation().y - ohterObject->GetLocation().y;
			float Distance = sqrt(Dx * Dx + Dy * Dy);
			float overlap = (player->GetRadius() + ohterObject->GetRadius()) - Distance;
			float pushOffsetX = Dx * overlap / (2 * Distance);
			float pushOffsetY = Dy * overlap / (2 * Distance);
			player->MoveObject(pushOffsetX, pushOffsetY);
			ohterObject->MoveObject(-pushOffsetX, -pushOffsetY);

			//플레이어 피해 처리
			//player.takenDamage(otherObject.damage);
		}
	}
}

void InGameStage::intersectsPlayerWithWall() {
	FVertexSimple playerLocation = player->GetLocation();
	float Radius = player->GetRadius();
	const float LeftBorder = -1.0f + Radius;
	const float RightBorder = 1.0f - Radius;
	const float TopBorder = 1.0f - Radius;
	const float BottomBorder = -1.0f + Radius;
	if (playerLocation.x < LeftBorder) {
		player->MoveObject(LeftBorder, playerLocation.y);
	}
	else if (playerLocation.x > RightBorder) {
		player->MoveObject(RightBorder, playerLocation.y);
	}
	if (playerLocation.y < BottomBorder) {
		player->MoveObject(playerLocation.x, BottomBorder);
	}
	else if (playerLocation.y > TopBorder) {
		player->MoveObject(playerLocation.x, TopBorder);
	}
}

//적과 플레이어의 공격간의 충돌 체크
void InGameStage::CheckHitCollision(float AttackRange)
{
	for (auto ohterObject : *objectList)
	{
		float Dx = player->GetLocation().x - ohterObject->GetLocation().x;
		float Dy = player->GetLocation().y - ohterObject->GetLocation().y;
		float Distance = Dx * Dx + Dy * Dy;
		
		float TargetDistance = AttackRange + ohterObject->GetRadius();
		if (TargetDistance * TargetDistance >= Distance)
		{
			//ohterObject의 피해처리
		}
	}
}