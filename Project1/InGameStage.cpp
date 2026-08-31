#include "InGameStage.h"

void InGameStage::Enter()
{
	//여기서 플레이어 캐릭터를 생성
	if (player == nullptr) {
		player = new Object(); // 임시로 생성
	}
	if (objectList == nullptr)
	{
		objectList = new std::vector<Object*>(); // 임시로 생성
	}
}

void InGameStage::Update(float deltaTime)
{
	//특정 주기마다 적 생성 로직이 필요함
	//특정 주기는 어떻게 판단함?
	//인수로 받는건 프레임당 시간임.

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
	Render();
}

void InGameStage::Render()
{
	//ObjectManager에서 오브젝트 랜더함수 호출
	for (auto object : *objectList)
	{
		//object->Render();
	}
}

void InGameStage::Exit()
{
	//자원 전부 해제
	delete player;
	player = nullptr;
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