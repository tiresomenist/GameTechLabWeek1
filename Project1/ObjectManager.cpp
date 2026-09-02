#include "ObjectManager.h"
#include "Enemy.h"
#include "SoundManager.h"
ObjectManager* ObjectManager::Ins = nullptr;

void ObjectManager::AddEnemy(Enemy* enemy)
{
	enemyList.push_back(enemy);
}

void ObjectManager::AddWeapon(Weapon* weapon)
{
	weaponList.push_back(weapon);
}

void ObjectManager::AddObject(Object* obj)
{
	obejctList.push_back(obj);
	
}


void ObjectManager::CreateEnemy(float difficulty)
{
	Enemy* newEnemy = new Enemy(difficulty);
	AddEnemy(newEnemy);
}

Player* ObjectManager::CreatePlayer()
{
	Player* newPlayer = new Player();	
	AddObject(newPlayer);
	return newPlayer;
}

void ObjectManager::CreateWeapon()
{
	Weapon* newWeapon = new Weapon();
	AddWeapon(newWeapon);
}



ObjectManager::ObjectManager()
{
	Ins = this;
}
void ObjectManager::Render()
{
	
	if (obejctList.size() > 0)
	{
		ID3D11ShaderResourceView* srv = App::Ins->GetEarthTextureSRV();
		if(srv != m_lastBoundSRV)
		{
			App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			m_lastBoundSRV = srv;
		}

		for (auto object : obejctList)
		{
			object->renderer->RenderPrimitive(object->GetLocation(), object->GetRadius());
		}
	}


	if (enemyList.size() > 0)
	{
		ID3D11ShaderResourceView* srv = App::Ins->GetMeteorTextureSRV();
		if (srv != m_lastBoundSRV)
		{

			App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			m_lastBoundSRV = srv;
		}
				for (auto enemy : enemyList)
		{
			enemy->renderer->RenderPrimitive(enemy->GetLocation(), enemy->GetRadius());
		}

	}


	if (weaponList.size() > 0)
	{
		ID3D11ShaderResourceView* srv = App::Ins->GetMoonTextureSRV();
		if (srv != m_lastBoundSRV)
		{
			
			App::Ins->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			m_lastBoundSRV = srv;
		}
		for (auto weapon : weaponList)
		{
			weapon->renderer->RenderPrimitive(weapon->GetLocation(), weapon->GetRadius());
		}

	}


}
void ObjectManager::Update(float deltaTime)
{
	EnemyMove(deltaTime);
	checkEnemiesIntersect();

}
void ObjectManager::EnemyMove(float deltaTime)
{
	for (auto enemy : enemyList)
	{
		FVector moveDir(0.0f, 0.0f, 0.0f);
		float targetX = obejctList[0]->GetLocation().x - enemy->GetLocation().x;
		float targetY = obejctList[0]->GetLocation().y - enemy->GetLocation().y;

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
		if (obejctList[0]->Intersect(ohterObject)) {
			//여기에 충돌관련 처리
			//충돌한만큼 서로 밀어내기/플레이어 체력 피해
			// 법선벡터*겹쳐진범위/2만큼 서로 밀어내면 됨.(질량은 고려하지않음)
			float Dx = obejctList[0]->GetLocation().x - ohterObject->GetLocation().x;
			float Dy = obejctList[0]->GetLocation().y - ohterObject->GetLocation().y;
			float Distance = sqrt(Dx * Dx + Dy * Dy);
			float overlap = (obejctList[0]->GetRadius() + ohterObject->GetRadius()) - Distance;
			float pushOffsetX = Dx * overlap / (2 * Distance);
			float pushOffsetY = Dy * overlap / (2 * Distance);
			obejctList[0]->MoveObject(pushOffsetX, pushOffsetY);
			ohterObject->MoveObject(-pushOffsetX, -pushOffsetY);

			//플레이어 피해 처리
			obejctList[0]->GetAttacked(ohterObject->GetAttack());
			if (obejctList[0]->IsDead())
			{
				//플레이어 죽음 처리
				OutputDebugStringA("Player Dead!\n");
			}
		}
	}

}

void ObjectManager::intersectsPlayerWithWall()
{
	FVector playerLocation = obejctList[0]->GetLocation();
	float Radius = obejctList[0]->GetRadius();
	const float LeftBorder = -1.0f + Radius;
	const float RightBorder = 1.0f - Radius;
	const float TopBorder = 1.0f - Radius;
	const float BottomBorder = -1.0f + Radius;
	if (playerLocation.x < LeftBorder) {
		obejctList[0]->SetLocation(LeftBorder, playerLocation.y);
	}
	else if (playerLocation.x > RightBorder) {
		obejctList[0]->SetLocation(RightBorder, playerLocation.y);
	}
	if (playerLocation.y < BottomBorder) {
		obejctList[0]->SetLocation(playerLocation.x, BottomBorder);
	}
	else if (playerLocation.y > TopBorder) {
		obejctList[0]->SetLocation(playerLocation.x, TopBorder);
	}

}
void ObjectManager::checkWeaponIntersectWithEnemy() 
{
	for (auto enemy : enemyList)
		enemy->InvincibleTimerUpdate(TimeManager::GetInstance()->GetDeltaTime());
	//무기와 적 충돌 처리
	Player* player = static_cast<Player*>(obejctList[0]);
	for (auto weapon : weaponList)
	{
		for (auto enemy : enemyList)
		{
			
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
					enemy->SetisHit(true);
				}
				
				if (enemy->IsDead())
				{
					//적 제거
					killCount++;
					auto it = std::find(enemyList.begin(), enemyList.end(), enemy);
					if (it != enemyList.end()) {
						delete* it; // 메모리 해제
						enemyList.erase(it); // 리스트에서 제거
					}
				}
			}
		}
	}
}

void ObjectManager::SpinWeapon(float deltaTime, float rotationSpeed)
{
	Player* player = static_cast<Player*>(obejctList[0]);
	orbitAngle += player->GetWeaponRotationSpeed() * deltaTime;      // 전체 회전

	if (orbitAngle > 6.2831853f)
		orbitAngle -= 6.2831853f;               // 오버플로 방지

	int count = static_cast<int>(weaponList.size());
	if (count == 0) return;

	float step = 6.2831853f / count;
	FVector center = player->GetLocation();

	for (int i = 0; i < count; ++i) {
		weaponList[i]->UpdateOrbit(deltaTime, player->GetWeaponRotationSpeed(), orbitAngle + step * i,center,player->GetOrbitRadius());
	}



}


void ObjectManager::ReleaseAllObjects()
{
		for (auto obj : obejctList) {
			delete obj;
		}
		obejctList.clear();
		for (auto enemy : enemyList) {
			delete enemy;
		}
		enemyList.clear();

		for (auto weapon : weaponList) {
			delete weapon;
		}
		weaponList.clear();
};




