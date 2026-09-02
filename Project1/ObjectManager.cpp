#include "ObjectManager.h"
#include "Enemy.h"
#include "SoundManager.h"
#include "UItem.h"
#include "EPrimitives.h"
#include "UExpOrb.h"

#include "FPixelConstant.h"
#include "D3D11Util.h"
#include <cmath>


ObjectManager* ObjectManager::Ins = nullptr;

void ObjectManager::AddEnemy(Enemy* enemy)
{
	enemyList.push_back(enemy);
}

void ObjectManager::AddWeapon(Weapon* weapon)
{
	weaponList.push_back(weapon);
}

void ObjectManager::AddMissile(UMissile* missile)
{
	missileList.push_back(missile);
}

void ObjectManager::AddObject(Object* obj)
{
	objectList.push_back(obj);
	
}

void ObjectManager::CreateEnemy(float difficulty)
{
	Enemy* newEnemy;

	if (deadEnemyList.empty())
	{
		newEnemy = new Enemy(difficulty);
	}
	else
	{
		newEnemy = deadEnemyList.back();
		deadEnemyList.pop_back();
		newEnemy->Reset(difficulty);   // 재초기화
	}

	AddEnemy(newEnemy);
}

void ObjectManager::CreateMissile(float damage,float speed,FVector moveDir)
{
	UMissile* newmissile;


	if (inactiveMissileList.empty())
	{

		newmissile = new UMissile(damage, speed, moveDir);
	}
	else
	{
		newmissile = inactiveMissileList.back();
		inactiveMissileList.pop_back();
		newmissile->Reset(damage, speed, moveDir);   // 재초기화
	}
	newmissile->SetRotation( 0.0f, 0.0f, atan2f(moveDir.y, moveDir.x) - 1.5707963f );
	newmissile->SetLocation(objectList[0]->GetLocation().x, objectList[0]->GetLocation().y);
	AddMissile(newmissile);
}

Player* ObjectManager::CreatePlayer()
{
	Player* newPlayer = new Player();	
	AddObject(newPlayer);
	return newPlayer;
}

void ObjectManager::CreateWeapon(float radius)
{
	Weapon* newWeapon = new Weapon(radius);
	AddWeapon(newWeapon);
}

void ObjectManager::CreateExpOrb(float x,float y)
{
	UExpOrb* newExpOrb = new UExpOrb();
	newExpOrb->SetLocation(x,y);
	AddObject(newExpOrb);
}



ObjectManager::ObjectManager()
{
	Ins = this;
}

void ObjectManager::Render()
{
	
	if (objectList.size() > 0)
	{
		ID3D11ShaderResourceView* srv = App::Ins->GetEarthTextureSRV();
		if(srv != m_lastBoundSRV)
		{
			App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			m_lastBoundSRV = srv;
		}

			{
			UINT Offset = 0;
			ID3D11Buffer* VB = App::Ins->GetSphereVertexBuffer();
			UINT Stride = sizeof(FVertexSimple);
			App::Ins->GetDeviceContext()->IASetVertexBuffers(0, 1, &VB, &Stride, &Offset);
			objectList[0]->renderer->RenderPrimitive(objectList[0]->GetLocation(), objectList[0]->GetRadius(), objectList[0]->GetRotation(), Priv::Sphere);
			}

			srv = App::Ins->GetExpOrbTextureSRV();
			if (srv != m_lastBoundSRV)
			{
				App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
				m_lastBoundSRV = srv;
			}


			UINT Offset = 0;
			ID3D11Buffer* VB = App::Ins->GetPlaneVertexBuffer();
			UINT Stride = sizeof(FVertexSimple);
			App::Ins->GetDeviceContext()->IASetVertexBuffers(0, 1, &VB, &Stride, &Offset);

			float factor[4] = { 0, 0, 0, 0 };
			App::Ins->GetDeviceContext()->OMSetBlendState(App::Ins->GetBlendState(), nullptr, 0xFFFFFFFF);
			for (size_t i = 1; i < objectList.size(); ++i)
			{
				objectList[i]->renderer->RenderPlane(objectList[i]->GetLocation(), objectList[i]->GetRadius(), objectList[i]->GetRadius(), objectList[i]->GetRotation());
			}

			App::Ins->GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	}


	if (enemyList.size() > 0)
	{
		UINT Offset = 0;
		ID3D11Buffer* VB = App::Ins->GetSphereVertexBuffer();
		UINT Stride = sizeof(FVertexSimple);
		App::Ins->GetDeviceContext()->IASetVertexBuffers(0, 1, &VB, &Stride, &Offset);
		ID3D11ShaderResourceView* srv = App::Ins->GetMeteorTextureSRV();
		if (srv != m_lastBoundSRV)
		{
			App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			m_lastBoundSRV = srv;
		}

		// PS 상수버퍼 설정
		ID3D11Buffer* hitFlashBuffer =
			App::Ins->GetHitFlashConstantBuffer();

		App::Ins->GetDeviceContext()->PSSetConstantBuffers(
			0, 1, &hitFlashBuffer
		);

		for (auto enemy : enemyList)
		{
			FPixelConstant hitFlashCB = {};
			hitFlashCB.hitFlashAmount = enemy->GetHitFlashAmount();

			D3D11Util::UpdateConstantBuffer(
				App::Ins->GetDeviceContext(),
				hitFlashBuffer,
				hitFlashCB
			);

			enemy->renderer->RenderPrimitive(enemy->GetLocation(), enemy->GetRadius(), enemy->GetRotation(), Priv::Sphere);
		}

		// enemy 렌더가 전부 끝난 뒤 상수버퍼 초기화
		FPixelConstant resetCB = {};
		resetCB.hitFlashAmount = 0.0f;
		resetCB.alpha = 1.0f;
		
		D3D11Util::UpdateConstantBuffer(
			App::Ins->GetDeviceContext(),
			hitFlashBuffer,
			resetCB
		);
	}


	if (weaponList.size() > 0)
	{
		UINT Offset = 0;
		ID3D11Buffer* VB = App::Ins->GetSphereVertexBuffer();
		UINT Stride = sizeof(FVertexSimple);
		App::Ins->GetDeviceContext()->IASetVertexBuffers(0, 1, &VB, &Stride, &Offset);
		ID3D11ShaderResourceView* srv = App::Ins->GetMoonTextureSRV();
		if (srv != m_lastBoundSRV)
		{
			
			App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			m_lastBoundSRV = srv;
		}
		for (auto weapon : weaponList)
		{
			weapon->renderer->RenderPrimitive(weapon->GetLocation(), weapon->GetRadius(), weapon->GetRotation(), Priv::Sphere);
		}

	}

	if (hitEffects.size() > 0)
	{
		UINT Offset = 0;
		ID3D11Buffer* VB = App::Ins->GetPlaneVertexBuffer();
		UINT Stride = sizeof(FVertexSimple);
		App::Ins->GetDeviceContext()->IASetVertexBuffers(0, 1, &VB, &Stride, &Offset);
		ID3D11ShaderResourceView* srv = App::Ins->GetHitEffectTextureSRV();
		if (srv != m_lastBoundSRV)
		{
			App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			m_lastBoundSRV = srv;
		}

		App::Ins->GetDeviceContext()->OMSetBlendState(App::Ins->GetBlendState(), nullptr, 0xFFFFFFFF);

		ID3D11Buffer* pixelBuffer =	App::Ins->GetHitFlashConstantBuffer();

		App::Ins->GetDeviceContext()->PSSetConstantBuffers(0, 1, &pixelBuffer);

		for (auto& hitEffect : hitEffects)
		{
			FPixelConstant cb = {};
			cb.hitFlashAmount = 0.0f;
			cb.alpha = hitEffect.alpha;

			D3D11Util::UpdateConstantBuffer(
				App::Ins->GetDeviceContext(),
				pixelBuffer,
				cb
			);

			objectList[0]->renderer->RenderPlane(
				hitEffect.location,
				hitEffect.size,
				hitEffect.size,
				{ 0.0f, 0.0f, 0.0f }
			);
		}
		FPixelConstant resetCB = {};
		resetCB.hitFlashAmount = 0.0f;
		resetCB.alpha = 1.0f;

		D3D11Util::UpdateConstantBuffer(App::Ins->GetDeviceContext(),pixelBuffer,resetCB);

		App::Ins->GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
		hitEffects.erase(std::remove_if(hitEffects.begin(), hitEffects.end(),[](const FHitEffect& effect) { return effect.lifetime >= effect.duration; }),hitEffects.end());

	}

	ID3D11ShaderResourceView* srv = App::Ins->GetRocketTextureSRV();
	if (srv != m_lastBoundSRV)
	{
		App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
		m_lastBoundSRV = srv;
	}
	if (missileList.size() > 0)
	{
		UINT Offset = 0;
		ID3D11Buffer* VB = App::Ins->GetPlaneVertexBuffer();
		UINT Stride = sizeof(FVertexSimple);
		App::Ins->GetDeviceContext()->IASetVertexBuffers(0, 1, &VB, &Stride, &Offset);

		float factor[4] = { 0, 0, 0, 0 };
		App::Ins->GetDeviceContext()->OMSetBlendState(App::Ins->GetBlendState(), nullptr, 0xFFFFFFFF);
		for (auto missile : missileList)
		{
			missile->renderer->RenderPlane(
				missile->GetLocation(), missile->GetRadius(), missile->GetRadius(), missile->GetRotation());
		}
		App::Ins->GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

	}
}

void ObjectManager::RenderMainMenu()
{
	if (objectList.empty())
		return;

	m_lastBoundSRV = nullptr;

	UINT Offset = 0;
	ID3D11Buffer* VB = App::Ins->GetSphereVertexBuffer();
	UINT Stride = sizeof(FVertexSimple);

	App::Ins->GetDeviceContext()->IASetVertexBuffers(
		0, 1, &VB, &Stride, &Offset
	);

	// 달 렌더
	auto RenderWeapon = [&](auto weapon)
		{
			ID3D11ShaderResourceView* srv =
				App::Ins->GetMoonTextureSRV();

			if (srv != m_lastBoundSRV)
			{
				App::Ins->GetDeviceContext()->PSSetShaderResources(
					0, 1, &srv
				);

				m_lastBoundSRV = srv;
			}

			float depthScale =
				1.0f + weapon->GetDepth() * 0.12f;

			weapon->renderer->RenderPrimitive(
				weapon->GetLocation(),
				weapon->GetRadius() * depthScale,
				weapon->GetRotation(),
				Priv::Sphere
			);
		};

	// 지구 렌더
	auto RenderEarth = [&]()
		{
			ID3D11ShaderResourceView* srv =
				App::Ins->GetEarthTextureSRV();

			if (srv != m_lastBoundSRV)
			{
				App::Ins->GetDeviceContext()->PSSetShaderResources(
					0, 1, &srv
				);

				m_lastBoundSRV = srv;
			}

			objectList[0]->renderer->RenderPrimitive(
				objectList[0]->GetLocation(),
				objectList[0]->GetRadius(),
				objectList[0]->GetRotation(),
				Priv::Sphere
			);
		};

	// 지구 뒤쪽에 있는 달 먼저 렌더
	for (auto weapon : weaponList)
	{
		if (weapon->GetDepth() < 0.0f)
		{
			RenderWeapon(weapon);
		}
	}

	// 지구 렌더
	RenderEarth();

	// 지구 앞쪽에 있는 달 나중에 렌더
	for (auto weapon : weaponList)
	{
		if (weapon->GetDepth() >= 0.0f)
		{
			RenderWeapon(weapon);
		}
	}

	// 제목 렌더
	{
		ID3D11ShaderResourceView* srv =
			App::Ins->GetTitleTextureSRV();

		App::Ins->GetDeviceContext()->PSSetShaderResources(
			0, 1, &srv
		);

		// Plane VB로 변경
		UINT offset = 0;
		UINT stride = sizeof(FVertexSimple);

		ID3D11Buffer* planeVB =
			App::Ins->GetPlaneVertexBuffer();

		App::Ins->GetDeviceContext()->IASetVertexBuffers(
			0, 1, &planeVB, &stride, &offset
		);

		objectList[0]->renderer->RenderPlane(
			FVector(0.0f, 0.55f, 0.0f),   // 제목 위치
			0.3f, 0.5f,                         // 크기
			FVector(0.0f, 0.0f, 0.0f)
		);
	}
}

void ObjectManager::Update(float deltaTime)
{
	for (auto obj : objectList)
	{
		obj->UpdateState();
	}
	for (auto obj : weaponList) {
		obj->UpdateState(deltaTime);
	}
	for (auto enemy : enemyList)
	{
		enemy->UpdateState();
	}

}

void ObjectManager::EnemyMove(float deltaTime)
{
	for (auto enemy : enemyList)
	{
		FVector moveDir(0.0f, 0.0f, 0.0f);
		float targetX = objectList[0]->GetLocation().x - enemy->GetLocation().x;
		float targetY = objectList[0]->GetLocation().y - enemy->GetLocation().y;

		targetX /= sqrt(targetX * targetX + targetY * targetY);
		targetY /= sqrt(targetX * targetX + targetY * targetY);
		enemy->MoveObject(targetX * deltaTime * enemy->GetSpeed(), targetY * deltaTime * enemy->GetSpeed());
	}

}

void ObjectManager::checkEnemiesIntersect()
{
	for (auto enemy : enemyList)
	{
		for (auto otherEnemy : enemyList)
		{
			if (enemy != otherEnemy && enemy->Intersect(otherEnemy))
			{
				float Dx = enemy->GetLocation().x - otherEnemy->GetLocation().x;
				float Dy = enemy->GetLocation().y - otherEnemy->GetLocation().y;
				float Distance = sqrt(Dx * Dx + Dy * Dy);
				float overlap = (enemy->GetRadius() + otherEnemy->GetRadius()) - Distance;
				float pushOffsetX = Dx * overlap / (2 * Distance);
				float pushOffsetY = Dy * overlap / (2 * Distance);
				enemy->MoveObject(pushOffsetX, pushOffsetY);
				otherEnemy->MoveObject(-pushOffsetX, -pushOffsetY);
			}
		}
	}

}

void ObjectManager::checkPlayerIntersectWithEnemy()
{
	for (auto ohterObject : enemyList)
	{
		if (objectList[0]->Intersect(ohterObject)) {
			//여기에 충돌관련 처리
			//충돌한만큼 서로 밀어내기/플레이어 체력 피해
			// 법선벡터*겹쳐진범위/2만큼 서로 밀어내면 됨.(질량은 고려하지않음)
			float Dx = objectList[0]->GetLocation().x - ohterObject->GetLocation().x;
			float Dy = objectList[0]->GetLocation().y - ohterObject->GetLocation().y;
			float Distance = sqrt(Dx * Dx + Dy * Dy);
			float overlap = (objectList[0]->GetRadius() + ohterObject->GetRadius()) - Distance;
			float pushOffsetX = Dx * overlap / (2 * Distance);
			float pushOffsetY = Dy * overlap / (2 * Distance);
			objectList[0]->MoveObject(pushOffsetX, pushOffsetY);
			ohterObject->MoveObject(-pushOffsetX, -pushOffsetY);

			//플레이어 피해 처리
			objectList[0]->GetAttacked(ohterObject->GetAttack());
			if (objectList[0]->IsDead())
			{
				//플레이어 죽음 처리
				OutputDebugStringA("Player Dead!\n");
			}
		}
	}

}

void ObjectManager::intersectsPlayerWithWall()
{
	FVector playerLocation = objectList[0]->GetLocation();
	float Radius = objectList[0]->GetRadius();
	const float LeftBorder = -1.0f + Radius;
	const float RightBorder = 1.0f - Radius;
	const float TopBorder = 1.0f - Radius;
	const float BottomBorder = -1.0f + Radius;
	if (playerLocation.x < LeftBorder) {
		objectList[0]->SetLocation(LeftBorder, playerLocation.y);
	}
	else if (playerLocation.x > RightBorder) {
		objectList[0]->SetLocation(RightBorder, playerLocation.y);
	}
	if (playerLocation.y < BottomBorder) {
		objectList[0]->SetLocation(playerLocation.x, BottomBorder);
	}
	else if (playerLocation.y > TopBorder) {
		objectList[0]->SetLocation(playerLocation.x, TopBorder);
	}

}

void ObjectManager::checkWeaponIntersectWithEnemy() 
{
	float deltaTime = TimeManager::GetInstance()->GetDeltaTime();

	for (auto enemy : enemyList)
		enemy->InvincibleTimerUpdate(deltaTime);

	// HitEffect의 lifetime 업데이트
	for (auto& hitEffect : hitEffects)
	{
		hitEffect.lifetime += deltaTime;
		hitEffect.alpha = 1.0f - (hitEffect.lifetime / hitEffect.duration);
		hitEffect.size = 0.1f + (hitEffect.lifetime / hitEffect.duration) * 0.05f;
	}

	//무기와 적 충돌 처리
	Player* player = static_cast<Player*>(objectList[0]);
	for (auto weapon : weaponList)
	{
		for (auto enemy : enemyList)
		{
			if (enemy->GetPendingRemove())
				continue;

			if (weapon->Intersect(enemy)) {
				//여기에 충돌관련 처리
				//충돌한만큼 서로 밀어내기/적 체력 피해
				// 법선벡터*겹쳐진범위만큼 적만 밀어내면 됨.(질량은 고려하지않음)
				float Dx = weapon->GetLocation().x - enemy->GetLocation().x;
				float Dy = weapon->GetLocation().y - enemy->GetLocation().y;
				float Distance = sqrt(Dx * Dx + Dy * Dy);
				float overlap = (weapon->GetRadius() + enemy->GetRadius()) - Distance;
				float nv = sqrt((enemy->GetLocation().x - player->GetLocation().x) * (enemy->GetLocation().x - player->GetLocation().x) + (enemy->GetLocation().y - player->GetLocation().y) * (enemy->GetLocation().y - player->GetLocation().y));
				float nx = (enemy->GetLocation().x - player->GetLocation().x) / nv;
				float ny = (enemy->GetLocation().y - player->GetLocation().y) / nv;
				float pushOffsetX = weapon->GetRadius() * 2;
				float pushOffsetY = weapon->GetRadius() * 2;
				enemy->MoveObject(nx * pushOffsetX, ny * pushOffsetY);
				//적 피해 처리
				if (enemy->GetisHit() == false)
				{
					USoundManager::GetInstance()->PlaySFX(ENEMY_HIT);
					enemy->GetAttacked(player->GetAttack());
					enemy->SetHitFlashAmount(1.0f);
					enemy->SetisHit(true);
					hitEffects.push_back(FHitEffect(enemy->GetLocation(), 0.0f, 0.5f, 0.2f, 1.0f));
				}
				
 				if (enemy->IsDead())
				{
					//경험치 오브 생성
					CreateExpOrb(enemy->GetLocation().x, enemy->GetLocation().y);
					killCount++;
					enemy->SetPendingRemove(true);
				}
			}
		}
	}

	for (int i = (int)enemyList.size() - 1; i >= 0; )
	{
		if (enemyList[i]->GetPendingRemove())
		{
			int s = enemyList.size() - 1;
			deadEnemyList.push_back(enemyList[i]);
			enemyList.erase(enemyList.begin() + i);

			if (i == s)
				--i;
		}
		else
			--i;


	}


}

void ObjectManager::checkPlayerIntersectWithExpOrb()
{
	for(size_t i = 1; i < objectList.size(); ++i)
	{
		UExpOrb* expOrb = static_cast<UExpOrb*>(objectList[i]);

		float leftDownX = expOrb->GetLocation().x - expOrb->GetRadius();
		float leftDonwY = expOrb->GetLocation().y - expOrb->GetRadius();

		float rightUpX = expOrb->GetLocation().x + expOrb->GetRadius();
		float rightUpY = expOrb->GetLocation().y + expOrb->GetRadius();


		float sphereCenterX = objectList[0]->GetLocation().x;
		float sphereCenterY = objectList[0]->GetLocation().y;

		float closestX = sphereCenterX;
		if (closestX < leftDownX)  
				closestX = leftDownX;

		if (closestX > rightUpX)   
				closestX = rightUpX;

		float closestY = sphereCenterY;
		if (closestY < leftDonwY)  
				closestY = leftDonwY;

		if (closestY > rightUpY)   
				closestY = rightUpY;

		float dx = sphereCenterX - closestX;
		float dy = sphereCenterY - closestY;

		if((dx*dx + dy* dy) <= (objectList[0]->GetRadius() * objectList[0]->GetRadius()))
		{
			USoundManager::GetInstance()->PlaySFX(PICK_EXP_ORB);
			Player* player = static_cast<Player*>(objectList[0]);
			player->AddExp(expOrb->GetGiveExp());
			//USoundManager::GetInstance()->PlaySFX(EXP_ORB_COLLECT);
			auto it = std::find(objectList.begin(), objectList.end(), expOrb);
			if (it != objectList.end()) {
				delete* it; // 메모리 해제
				objectList.erase(it); // 리스트에서 제거
			}
		}

	}


}

void ObjectManager::CheckMissileIntersectWithEnemy()
{
	for (int mi = (int)missileList.size() - 1; mi >= 0; --mi)
	{
		UMissile* missile = missileList[mi];
		if (missile->isInActive) continue;

		float leftDownX = missile->GetLocation().x - missile->GetRadius();
		float leftDownY = missile->GetLocation().y - missile->GetRadius();
		float rightUpX = missile->GetLocation().x + missile->GetRadius();
		float rightUpY = missile->GetLocation().y + missile->GetRadius();

		for (int ei = (int)enemyList.size() - 1; ei >= 0; --ei)
		{
			Enemy* enemy = enemyList[ei];
			if (enemy->GetPendingRemove()) continue;

			float sphereCenterX = enemy->GetLocation().x;
			float sphereCenterY = enemy->GetLocation().y;

			float closestX = sphereCenterX;
			if (closestX < leftDownX) closestX = leftDownX;
			if (closestX > rightUpX)  closestX = rightUpX;

			float closestY = sphereCenterY;
			if (closestY < leftDownY) closestY = leftDownY;
			if (closestY > rightUpY)  closestY = rightUpY;

			float dx = sphereCenterX - closestX;
			float dy = sphereCenterY - closestY;
			float r = enemy->GetRadius();


			if ((dx * dx + dy * dy) <= (r * r))
			{
				USoundManager::GetInstance()->PlaySFX(ENEMY_HIT);
				enemy->GetAttacked(missile->GetDMG());
				if (enemy->IsDead())
				{
					CreateExpOrb(enemy->GetLocation().x, enemy->GetLocation().y);
					killCount++;
					enemy->SetPendingRemove(true);
				}

				missile->isInActive = true;   // 표시만
				break;
			}
		}
	}
	for (int i = (int)enemyList.size() - 1; i >= 0; --i)
	{
		if (enemyList[i]->GetPendingRemove())
		{
			deadEnemyList.push_back(enemyList[i]);
			enemyList.erase(enemyList.begin() + i);
		}
	}

	for (int i = (int)missileList.size() - 1; i >= 0; --i)
	{
		if (missileList[i]->isInActive)
		{
			inactiveMissileList.push_back(missileList[i]);
			missileList.erase(missileList.begin() + i);
		}
	}

}

void ObjectManager::ShootMissileToEnemy()
{
	if (enemyList.size() == 0)
		return;

	FVector nearestEnemyLocation = FindNearestEnemyLocation();
	Player* player = static_cast<Player*>(objectList[0]);

	FVector targetDir = { nearestEnemyLocation.x - player->GetLocation().x,
					  nearestEnemyLocation.y - player->GetLocation().y,
					  0.0f };

	float len = sqrtf(targetDir.x * targetDir.x + targetDir.y * targetDir.y);
	if (len > 0.0001f)
	{
		targetDir.x /= len;
		targetDir.y /= len;
	}

	CreateMissile(player->GetMissileDmg(), player->GetMissileMoveSpeed(), targetDir);

	//미사일을 적쪽으로 꺾이게 만들어야함

	

}

void ObjectManager::MoveMissile(float deltaTime)
{
	for(auto& m : missileList)
	{
		if (m->isInActive)
			continue;
		m->AddLifetime(deltaTime);
		m->UpdateLocation(deltaTime);

	if (m->GetLifetime() >= m->GetDuration())
		m->isInActive = true;
	}


	for (int i = (int)missileList.size() - 1; i >= 0; --i)
	{
		if (missileList[i]->isInActive)
		{
			inactiveMissileList.push_back(missileList[i]);
			missileList.erase(missileList.begin() + i);
		}
	}

}

FVector ObjectManager::FindNearestEnemyLocation()
{
	FVector result = FVector(0.0f, 0.0f, 0.0f);

	float minDistance = 6;

	FVector playerLocation = objectList[0]->GetLocation();

	for (auto enemy : enemyList)
	{
		if (enemy->GetPendingRemove())
			continue;
		
		FVector enemyLocation = enemy->GetLocation();

		float distance = (playerLocation.x - enemyLocation.x) * (playerLocation.x - enemyLocation.x) + (playerLocation.y - enemyLocation.y) * (playerLocation.y - enemyLocation.y);

		if (minDistance > distance)
		{
			minDistance = distance;
			result = enemyLocation;
		}

	}


	return result;
}

void ObjectManager::SpinWeapon(float deltaTime, float rotationSpeed)
{
	Player* player = static_cast<Player*>(objectList[0]);
	orbitAngle += player->GetWeaponRotationSpeed() * deltaTime;      // 전체 회전

	while (orbitAngle > 6.2831853f)
		orbitAngle -= 6.2831853f;               // 오버플로 방지

	int count = static_cast<int>(weaponList.size());
	if (count == 0) return;

	float step = 6.2831853f / count;
	FVector center = player->GetLocation();

	for (int i = 0; i < count; ++i) {
		weaponList[i]->UpdateOrbit(deltaTime, player->GetWeaponRotationSpeed(), orbitAngle + step * i,center,player->GetOrbitRadius());
	}
}

void ObjectManager::SpinWeaponMainMenu(float deltaTime, float rotationSpeed)
{
	Player* player = static_cast<Player*>(objectList[0]);
	orbitAngle += rotationSpeed * deltaTime;      // 전체 회전

	if (orbitAngle > 6.2831853f)
		orbitAngle -= 6.2831853f;               // 오버플로 방지

	int count = static_cast<int>(weaponList.size());
	if (count == 0) return;

	float step = 6.2831853f / count;
	FVector center = player->GetLocation();

	for (int i = 0; i < count; ++i) {
		float angle = orbitAngle + step * i;
		weaponList[i]->SetDepth(sinf(angle));
		weaponList[i]->UpdateOrbitMainMenu(angle, center, player->GetOrbitRadius());
	}
}

void ObjectManager::AddPlayerExp(int x) {
	Player* player = static_cast<Player*>(objectList[0]);

	player->AddExp(x);
}

bool ObjectManager::IsPlayerLevelUp() {
	Player* player = static_cast<Player*>(objectList[0]);

	return player->IsLevelUp();
}

void ObjectManager::UpgradePlayer(AugmentStruct aug) {
	Player* player = static_cast<Player*>(objectList[0]);

	switch (aug.type)
	{
	case AugmentType::Attack:
		// 공격력 증가
		player->IncreaseAttack(aug.value);
		break;

	case AugmentType::AttackRange:
		// 공격 범위 증가
		player->IncreaseAttackRange(aug.value);
		break;

	case AugmentType::AttackSpeed:
		// 공격 속도 증가
		player->IncreaseAttackSpeed(aug.value);
		break;

	case AugmentType::MoveSpeed:
		// 이동 속도 증가
		player->IncreaseMoveSpeed(aug.value);
		break;

	case AugmentType::WeaponSize:
		// 무기 크기 증가

		for (Weapon* weapon : weaponList)
		{
			weapon->IncreaseRadius(aug.value);
			
		}
		player->SetWeaponRadius(player->GetWeaponRadius() * (1 + aug.value / 100.0f)); // 플레이어의 무기 반지름도 증가
		break;

	case AugmentType::HealHP:
		// 체력 회복
		player->IncreaseHealHp(aug.value);
		break;

	case AugmentType::AddWeapon:

		for (int i = 0; i < static_cast<int>(aug.value); i++) {
			CreateWeapon(player->GetWeaponRadius());
		}
		
		break;

	case AugmentType::RocketAdd:
		
		player->SetHasRocket(true);
		break;

	case AugmentType::RocketSpeed:
		player->IncreaseMissileMoveSpeed(aug.value);
		break;

	case AugmentType::RocketDamage:
		player->IncreaseMissileDmg(aug.value);
		break;
	
	case AugmentType::RocketAttackSpeed:
		player->IncreaseMissileRapidSpeed(aug.value);
		break;
	}
}


void ObjectManager::ReleaseAllObjects()
{
		for (auto obj : objectList) {
			delete obj;
		}
		objectList.clear();
		for (auto enemy : enemyList) {
			delete enemy;
		}
		enemyList.clear();

		for (auto weapon : weaponList) {
			delete weapon;
		}
		weaponList.clear();

		for (auto* e : deadEnemyList) delete e;
		deadEnemyList.clear();

		for (auto* m : missileList) delete m;
		missileList.clear();

		for (auto* m : inactiveMissileList) delete m;
		inactiveMissileList.clear();
};




