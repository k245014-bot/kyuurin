#include "shotManager.h"
#include "shot.h"
#include "isHit.h"
#include "player.h"

ShotManager::ShotManager(SceneBase* _scene) :GameObject(_scene)
{
	for (int i = 0; i < SHOT_MAX; i++)
	{
		shot[i] = nullptr;
	}
	isHit = new IsHit;
	player = GetScene()->FindGameObject<Player>();
}

ShotManager::~ShotManager()
{
	for (int i = 0; i < SHOT_MAX; i++)
	{
		if (shot[i] != nullptr)
			shot[i]->DestroyMe();
	}
}

void ShotManager::Update()
{
	for (int i = 0; i < SHOT_MAX; i++)
	{
		if (shot[i] != nullptr)
		{
			if (shot[i]->IsActive() == false)
			{
				shot[i]->DestroyMe();
				shot[i] = nullptr;
				continue;
			}
			
			if (isHit->_IsHit(shot[i]->GetPosition(), player->GetPosition(), 80))
			{
				player->Damage(SHOT_DAMAGE);
				shot[i]->DestroyMe();
				shot[i] = nullptr;
			}
		}
	}
}

void ShotManager::Draw()
{
}

void ShotManager::CreateShot(VECTOR _pos, VECTOR _vel, float _dir)
{
	int i = 0;
	while (true)
	{
		if (shot[i] == nullptr)
		{
			shot[i] = GetScene()->CreateGameObject<Shot>();
			shot[i]->CreateShot(_pos, _vel, _dir);
			return;
		}
		i++;
	} 
}