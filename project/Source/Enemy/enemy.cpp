#include "enemy.h"
#include "../Floor/floor.h"	
#include "../playScene.h"
#include "../Player/player.h"
#include "../Ishit.h"
#include "Shot/Shotmanager.h"
#include "../../Library/sceneManager.h"
#include "../Common/Effect/effectManager.h"
#include <assert.h>

Enemy::Enemy(SceneBase * _scene) : CharaBase(_scene)
{
	floor = GetScene()->FindGameObject<Floor>();
	isHit = new IsHit();
	SetModel("blue");

	atkSound = LoadSoundMem("data\\sound\\shotSound.wav");
	laserSound = LoadSoundMem("data\\sound\\laserSound.wav");
	chargeSound = LoadSoundMem("data\\sound\\kyuinSound.wav");
	ChangeVolumeSoundMem(255, atkSound);
	assert(chargeSound >= 0);

	hpBar.image = LoadGraph("data\\texture\\HPBar.png");
	hpBar.pos = V2Get(208, 0);
	hpState = 1;
	
	position = SpawnPos;
	velocity = VZero;
	rotation.y = DX_PI_F;
	rotx = 0;		
	rotz = 0;		
	
	state = STATE::ST_STOP;	

	isAtkMove = false;

	destinationCheck = true;
	coolTimeCounter = 0;

	myHp = HP_MAX;
	isDead = false;

	damageCounter = 0;

	effect = SceneManager::CommonScene()->FindGameObject<EffectManager>();

	retHp = HP_SIZE + HP_BAR_SIZE;////////////////////

	soundCounter = 0;
	chageCounter = 0;

	phaseCounter = 0;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	switch (state)
	{
	case STATE::ST_STOP:
		break;
	case STATE::ST_PLAY:
		EnemyMove();
		break;
	case STATE::ST_SHOT:
		EnemyShot();
		break;
	case STATE::ST_DEAD:
		DeadEnemy();
		break;
	case STATE::ST_DAMAGE:
		DamageMove();
		break;
	}

	position += velocity;
	position = floor->SetPlayerPos(position);

	RetGauge();
}

void Enemy::EnemyMove()
{
	position.y = 10;
	if (destinationCheck)
	{
		for (int i = 0; i < XZ; i++)
		{
			moveCounter[i] = GetRand(1);
			if (moveCounter[i] == 0)
				moveCounter[i] = -1;
		}
		destination = VGet((float)GetRand((int)floor->GetFloor().x) * moveCounter[0], 0, (float)GetRand((int)floor->GetFloor().z * moveCounter[1]));
		destinationCheck = false;
	}

	target = destination - position;
	targetDirection = atan2f(target.x, target.z);

	rotation.y = targetDirection + DX_PI_F / 180;

	velocity += CalcMoveVector(rotation.y, ENEMY_SPEED);

	if (isHit->_IsHit(position, destination, AttackRange))
	{
		state = STATE::ST_SHOT;
		position.y = 0;
		destinationCheck = true;
		velocity.x = 0;
		velocity.z = 0;
	}
}

void Enemy::EnemyShot()
{
	if (coolTimeCounter == 0)
	{
		target = player->GetPosition() - position;
		targetDirection = atan2f(target.x, target.z);

		rotation.y = targetDirection + DX_PI_F / 180;
		
		if (myHp / HP_MAX >= LaserAttack)
		{
			coolTime = COOLTIME;
			shot = GetScene()->FindGameObject<ShotManager>();
			shot->CreateShot(position, VGet(SHOT_SPEED, 0, SHOT_SPEED), rotation.y);
			PlaySoundMem(atkSound, DX_PLAYTYPE_BACK);
		}
		else if(myHp / HP_MAX >= LastAttack)
		{
			coolTime = LaserCoolTime;
			VECTOR effPos = VGet(position.x, 40, position.z);//
			effect->Creat(effPos, effect->LESER_EFFECT, 1, VGet(1, 1, 1), LaserEffTime, 2);
			effect->SetRot(VGet(0, targetDirection + 0.5f * DX_PI_F, DX_PI_F * 0.5f), 2);
		}
		else
		{
			coolTime = LaserCoolTime;
			VECTOR effPos = VGet(position.x, 40.0f, position.z);///////////////
			effect->Creat(effPos, effect->LESER_EFFECT, 1, VGet(1, 1, 1), LaserEffTime, 2);
			effect->SetRot(VGet(0, targetDirection + 0.5f * DX_PI_F, DX_PI_F * 0.5), 2);
		}
	}
	else
	{
		if (myHp / HP_MAX >= LaserAttack)
		{
		}
		else if (myHp / HP_MAX >= LastAttack && effect->IsLaser()  )
		{
			if (coolTimeCounter <= ChargeMaxTime)
			{
				target = VSub(player->GetPosition(), position);
				targetDirection = atan2f(target.x, target.z);

				rotation.y = targetDirection + DX_PI_F / 180;
				effect->SetRot(VGet(0, targetDirection + 0.5f * DX_PI_F, DX_PI_F * 0.5), 2);//プレイヤー追尾
				if (chageCounter ==0 )
				{
					PlaySoundMem(chargeSound, DX_PLAYTYPE_BACK);
					chageCounter++;
				}
			}
			else  if (coolTimeCounter <= LaserMaxTime)
			{
				chageCounter = 0;
				HitLaser();
				if (soundCounter == 0)
				{
					PlaySoundMem(laserSound, DX_PLAYTYPE_BACK);
					soundCounter++;
				}
			}
		}
		else if (myHp / HP_MAX < 0.1 && effect->IsLaser())
		{
			if (coolTimeCounter % 2 == 0)
			{
				shot = GetScene()->FindGameObject<ShotManager>();
				shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, 0, SHOT_SPEED / 100 * coolTimeCounter), rotation.y + DX_PI_F / 70 * (coolTimeCounter - 45));
				shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, 0, SHOT_SPEED / 100 * coolTimeCounter), rotation.y - DX_PI_F / 70 * (coolTimeCounter - 45));
				shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, (float)abs(cos(DX_PI_F / 40 * coolTimeCounter)) * SHOT_SPEED / 2 * coolTimeCounter , SHOT_SPEED / 100 * coolTimeCounter), rotation.y + DX_PI_F / 140 * (coolTimeCounter + 45));
				shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, (float)abs(cos(DX_PI_F / 40 * coolTimeCounter)) * SHOT_SPEED / 2 * coolTimeCounter , SHOT_SPEED / 100 * coolTimeCounter), rotation.y - DX_PI_F / 140 * (coolTimeCounter + 45));
			}
			if (coolTimeCounter <= 50)
			{
				target = player->GetPosition() - position;
				targetDirection = atan2f(target.x, target.z);

				rotation.y = targetDirection + DX_PI_F / 180;
				effect->SetRot(VGet(0, targetDirection + 0.5f * DX_PI_F, DX_PI_F * 0.5f), 2);/////////
				if (chageCounter == 0)
				{
					PlaySoundMem(chargeSound, DX_PLAYTYPE_BACK);
					chageCounter++;
				}

			}
			else  if (coolTimeCounter <= 200)
			{
				HitLaser();
				chageCounter = 0;
				if (soundCounter == 0)
				{
					PlaySoundMem(laserSound, DX_PLAYTYPE_BACK);
					soundCounter++;
				}
			}
		}
		if(!effect->IsLaser())
			if (soundCounter > 0)
			{
				soundCounter = 0;
			}
	}

	coolTimeCounter++;
	if (coolTimeCounter >= coolTime)
	{
		state = STATE::ST_PLAY;
		coolTimeCounter = 0;
	}
}

void Enemy::Damage(float _playerDamage)
{
	myHp -= _playerDamage;
	effect->EffDestroy(2);
	soundCounter = 0;
	VECTOR effPos = VGet(position.x, 50, position.z);
	effect->Creat(effPos, effect->PLAYERATK_EFFECT, 5, VGet(1.5, 1.5, 1.5), HitEffTime, 1);
	state = STATE::ST_DAMAGE;
}

void Enemy::DeadEnemy()
{
	isDead = true;
}

void Enemy::DamageMove()
{
	damageCounter++;
	if (damageCounter == 1)
	{
		AllHitCharacter(true);
	}
	else if (damageCounter >= 20)
	{
		AllHitCharacter(false);
		destinationCheck = true;//////
		velocity = VGet(0, 0, 0);///

		state = STATE::ST_PLAY;
		coolTimeCounter = 0;
		damageCounter = 0;
	}

	if (myHp / HP_MAX <= 0.1)
	{
		if (phaseCounter == 1)
		{
			const VECTOR3 EffectSize = VECTOR3(100.0f, 100.0f, 100.0f);
			effect->Creat(position, effect->ENEMY_PHASE3, 1, EffectSize, PhaseEffTime, 5);
			phaseCounter++;
			hpState = 3;
		}
	}
	else if (myHp / HP_MAX <= 0.5)
	{
		if (phaseCounter == 0)
		{
			effect->Creat(position, effect->ENEMY_PHASE2, 1, VGet(100, 100, 100), PhaseEffTime, 4);
			phaseCounter ++;
			hpState = 2;
		}
	}

	if (myHp <= 0)
	{
		VECTOR effPos = VGet(position.x, 50, position.z);
		effect->Creat(effPos, effect->ENEMY_BOON, 1, VGet(1.5, 1.5, 1.5), PhaseEffTime, 6);
		state = STATE::ST_DEAD;
	}
}

void Enemy::RetGauge()
{
	float damageCopy = HP_SIZE + HP_BAR_SIZE * myHp / HP_MAX;
	if (retHp >= damageCopy)
		retHp -= HP_SPEED;
}

void Enemy::StartPlay()
{
	state = STATE::ST_PLAY;
}

void Enemy::Draw()
{
	if (!isDead)
		CharaBase::Draw();

	DrawRectGraph((int)hpBar.pos.x, (int)hpBar.pos.y, 0, 0, HP_BAR_X, HP_BAR_Y, hpBar.image, true);//フレーム
	DrawRectGraph((int)hpBar.pos.x, (int)hpBar.pos.y, 0, HP_BAR_Y * 3, (int)retHp, HP_BAR_Y, hpBar.image, true);//赤ゲージ
	DrawRectGraph((int)hpBar.pos.x, (int)hpBar.pos.y, 0, HP_BAR_Y * hpState, HP_SIZE + HP_BAR_SIZE * (int)myHp / HP_MAX, HP_BAR_Y, hpBar.image, true);//hp
}

void Enemy::PlayerData(Player* _player)
{
	player = _player;
}

void Enemy::HitLaser()
{
	int index = 1;
	const VECTOR pPos = player->GetPosition();
	float distance = VSize(VSub(position, pPos));

	while (true)
	{
		const VECTOR lc = VGet(position.x + sinf(rotation.y) * index, 0, position.z + cosf(rotation.y) * index);
		if (VSize(VSub(lc, pPos)) <= distance)
		{
			index++;
			distance = VSize(VSub(lc, pPos));
		}
		else
		{
			if (isHit->_IsHit(lc, pPos, 75))
				player->HitLaser();
			break;
		}
	}
}

void Enemy::StStop()
{
	state = STATE::ST_STOP;
	effect->EffDestroy(2);
}
