#include "effectManager.h"
#include <EffekseerForDXLib.h>
#include "effect.h"
#include <assert.h>

EffectManager::EffectManager(SceneBase* _scene) : GameObject(_scene)
{
	for (int i = 0; i < EffectID::EFFECT_NUM; i++)
	{
		effectDate[i] = -1;
	}

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		effect[i] = nullptr;
	}

	effectDate[EffectID::PLAYERATK_EFFECT] = LoadEffekseerEffect("data\\effect\\playerAtkEffect.efk", 13.0f);
	effectDate[EffectID::ENEMYATK_EFFECT] = LoadEffekseerEffect("data\\effect\\enemyAtkEffect.efk", 13.0f);
	effectDate[EffectID::LESER_EFFECT] = LoadEffekseerEffect("data\\effect\\LaserEffect.efk", 13.0f);
	effectDate[EffectID::PLAYER_DEAD] = LoadEffekseerEffect("data\\effect\\player_Dead.efk", 13.0f);
	effectDate[EffectID::ENEMY_PHASE2] = LoadEffekseerEffect("data\\effect\\phase2.efk", 30.0f);
	effectDate[EffectID::ENEMY_PHASE3] = LoadEffekseerEffect("data\\effect\\phase3.efk", 30.0f);
	effectDate[EffectID::ENEMY_BOON] = LoadEffekseerEffect("data\\effect\\enemy_boon.efk", 20.0f);
	assert(effectDate[EffectID::ENEMY_BOON] >= 0);

	isLaser = false;
}

EffectManager::~EffectManager()
{
	for (int i = 0; i < EffectID::EFFECT_NUM; i++)
	{
		DeleteEffekseerEffect(effectDate[i]);
	}
}

void EffectManager::Update()
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (effect[i] != nullptr)
		{
			if (!effect[i]->IsActive())
			{
				effect[i]->DestroyMe();
				effect[i] = nullptr;
			}
		}
	}

	if (isLaser)
	{
		copyTime--;
		if (copyTime <= 0)
			isLaser = false;
	}

	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();
}

void EffectManager::Creat(VECTOR _pos, EffectID _id,float _speed, VECTOR _size, int _time,int index)
{
	if (effect[index] == nullptr)
	{
		effect[index] = GetScene()->CreateGameObject<EffectBase>();
		effect[index]->Creat(_pos, effectDate[_id], _speed, _size, _time);
	}
	if (_id == LESER_EFFECT)
	{
		copyTime = _time;
		isLaser = true;
	}
	else
		isLaser = false;
}

void EffectManager::SetPos(VECTOR _pos, int index)
{
	if (effect[index] != nullptr)
	{
		effect[index]->SetPos(_pos);
	}
}

void EffectManager::SetRot(VECTOR _rot, int index)
{
	if (effect[index] != nullptr)
	{
		effect[index]->SetRotation(_rot);
	}
}

void EffectManager::EffDestroy(int index)
{
	if (effect[index] != nullptr)
	{
		effect[index]->Active();
		effect[index]->Destroy();
		effect[index]->DestroyMe();
	}	
}
