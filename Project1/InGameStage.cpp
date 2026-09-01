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
	//인풋매니저 가져오고 델타타임이랑 이동속도 생각해서 한프레임당 이동 거리 계산해서 move호출
	//
	player->MoveObject(deltaTime * player->GetSpeed() / 1000, deltaTime * player->GetSpeed() / 1000);
	// 플레이어 공격
	int nextAttackFrame = (int)(30.0f / player->GetAttackSpeed());
	if ((int)frameCount % nextAttackFrame == 0) {
		CheckHitCollision(player->GetAttackRange());

	}
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
	FVector playerLocation = player->GetLocation();
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