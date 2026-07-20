#include "enemy.h"
#include "../character.h"
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
	
	position = VGet(100.0f, 0, 200.0f);
	velocity = VGet(0, 0, 0);
	/*direction = DX_PI_F;*/
	rotation.y = DX_PI_F;
	rotx = 0;		
	rotz = 0;		
	
	state = STATE::ST_STOP;	

	isAtkMove = false;

	destinationCheck = true;
	coolTimeCounter = 0;

	myHp = HP_MAX;
	isDead = false;

	hpBar.image = LoadGraph("data\\texture\\HPBar.png");
	hpBar.pos = V2Get(208, 0);
	hpState = 1;

	damageCounter = 0;

	effect = SceneManager::CommonScene()->FindGameObject<EffectManager>();

	retHp = HP_SIZE + HP_BAR_SIZE;////////////////////

	atkSound = LoadSoundMem("data\\sound\\shotSound.wav");
	ChangeVolumeSoundMem(255, atkSound);

	laserSound = LoadSoundMem("data\\sound\\laserSound.wav");
	chargeSound = LoadSoundMem("data\\sound\\kyuinSound.wav");
	assert(chargeSound >= 0);

	soundCounter = 0;
	chageCounter = 0;

	phaseCounter = 0;
}

Enemy::~Enemy()
{
	//delete chara;
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

	position = VAdd(position, velocity);
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
		destination = VGet(GetRand(floor->GetFloor().x) * moveCounter[0], 0, GetRand(floor->GetFloor().z * moveCounter[1]));
		destinationCheck = false;
	}

	target = VSub(destination, position);
	targetDirection = atan2(target.x, target.z);

	rotation.y = targetDirection + DX_PI_F / 180;
	velocity.x += sinf(rotation.y) * ENEMY_SPEED;
	velocity.z += cosf(rotation.y) * ENEMY_SPEED;

	if (isHit->_IsHit(position, destination, 100))
	{
		state = STATE::ST_SHOT;
		position.y = 0;
		destinationCheck = true;
		velocity.x = 0;
		velocity.z = 0;
	}
	/*chara->SetPosition(position);
	chara->SetRotation(VGet(DX_PI_F * RUN_DEGREE / 180, direction, 0.0f));*/
}

void Enemy::EnemyShot()
{
	if (coolTimeCounter == 0)
	{
		target = VSub(player->GetPosition(), position);
		targetDirection = atan2(target.x, target.z);

		rotation.y = targetDirection + DX_PI_F / 180;
		
		if (myHp / HP_MAX >= 0.5)
		{
			coolTime = COOLTIME;
			shot = GetScene()->FindGameObject<ShotManager>();
			shot->CreateShot(position, VGet(SHOT_SPEED, 0, SHOT_SPEED), rotation.y);
			PlaySoundMem(atkSound, DX_PLAYTYPE_BACK);
		}
		else if(myHp / HP_MAX >= 0.1)
		{
			coolTime = COOLTIME * 1.4;
			VECTOR effPos = VGet(position.x, 40, position.z);//
			effect->Creat(effPos, effect->LESER_EFFECT, 1, VGet(1, 1, 1), 200, 2);
			effect->SetRot(VGet(0, targetDirection + 0.5 * DX_PI_F, DX_PI_F * 0.5), 2);
		}
		else
		{
			coolTime = COOLTIME * 1.4;
			VECTOR effPos = VGet(position.x, 40, position.z);///////////////
			effect->Creat(effPos, effect->LESER_EFFECT, 1, VGet(1, 1, 1), 200, 2);
			effect->SetRot(VGet(0, targetDirection + 0.5 * DX_PI_F, DX_PI_F * 0.5), 2);
		}
	}
	else
	{
		if (myHp / HP_MAX >= 0.5)
		{
		}
		else if (myHp / HP_MAX >= 0.1 && effect->IsLaser()  )
		{
			if (coolTimeCounter <= 50)
			{
				target = VSub(player->GetPosition(), position);
				targetDirection = atan2(target.x, target.z);

				rotation.y = targetDirection + DX_PI_F / 180;
				effect->SetRot(VGet(0, targetDirection + 0.5 * DX_PI_F, DX_PI_F * 0.5), 2);//プレイヤー追尾
				if (chageCounter ==0 )
				{
					PlaySoundMem(chargeSound, DX_PLAYTYPE_BACK);
					chageCounter++;
				}
			}
			else  if (coolTimeCounter <= 200)
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
				shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, abs(cos(DX_PI_F / 40 * coolTimeCounter)) * SHOT_SPEED / 2 * coolTimeCounter , SHOT_SPEED / 100 * coolTimeCounter), rotation.y + DX_PI_F / 140 * (coolTimeCounter + 45));
				shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, abs(cos(DX_PI_F / 40 * coolTimeCounter)) * SHOT_SPEED / 2 * coolTimeCounter , SHOT_SPEED / 100 * coolTimeCounter), rotation.y - DX_PI_F / 140 * (coolTimeCounter + 45));
				//お遊び
				//shot = GetScene()->FindGameObject<ShotManager>();
				//shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, 0, SHOT_SPEED / 100 * coolTimeCounter), direction + DX_PI_F / 50 * (coolTimeCounter));
				//shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, 0, SHOT_SPEED / 100 * coolTimeCounter), direction - DX_PI_F / 50 * (coolTimeCounter));
				//shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, abs(cos(DX_PI_F / 80 * coolTimeCounter)) * SHOT_SPEED  * coolTimeCounter, SHOT_SPEED / 100 * coolTimeCounter), direction + DX_PI_F / 25 * (coolTimeCounter - 45));
				//shot->CreateShot(position, VGet(SHOT_SPEED / 100 * coolTimeCounter, abs(cos(DX_PI_F / 80 * coolTimeCounter)) * SHOT_SPEED  * coolTimeCounter, SHOT_SPEED / 100 * coolTimeCounter), direction - DX_PI_F / 25 * (coolTimeCounter - 45));
				
			}
			if (coolTimeCounter <= 50)
			{
				target = VSub(player->GetPosition(), position);
				targetDirection = atan2(target.x, target.z);

				rotation.y = targetDirection + DX_PI_F / 180;
				effect->SetRot(VGet(0, targetDirection + 0.5 * DX_PI_F, DX_PI_F * 0.5), 2);/////////
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
				//StopSoundMem(laserSound);
			}
	}

	coolTimeCounter++;
	if (coolTimeCounter >= coolTime)
	{
		state = STATE::ST_PLAY;
		coolTimeCounter = 0;
	}
	
	/*chara->SetPosition(position);
	chara->SetRotation(VGet(0.0f, direction, 0.0f));*/
}

void Enemy::Damage(float _playerDamage)
{
	myHp -= _playerDamage;
	effect->EffDestroy(2);
	soundCounter = 0;
	VECTOR effPos = VGet(position.x, 50, position.z);
	effect->Creat(effPos, effect->PLAYERATK_EFFECT, 5, VGet(1.5, 1.5, 1.5), 100 , 1);
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
		/*chara->SetRotation(VGet(DX_PI_F * -0.25, player->GetRotation().y + DX_PI_F, 0.0f));

		chara->HitCharacter(Character::MODEL_ID::MODEL_HEAD, 2, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_BODY, 2, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_RHAND, 1, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_LHAND, 1, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_RFOOT, 1, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_LFOOT, 1, true);///*/

		AllHitCharacter(true);
	}
	else if (damageCounter >= 20)
	{
		/*chara->HitCharacter(Character::MODEL_ID::MODEL_HEAD, 2, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_BODY, 2, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_RHAND, 1, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_LHAND, 1, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_RFOOT, 1, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_LFOOT, 1, false);///*/
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
			effect->Creat(position, effect->ENEMY_PHASE3, 1, VGet(100, 100, 100), 100, 5);
			phaseCounter++;
			hpState = 3;
		}
	}
	else if (myHp / HP_MAX <= 0.5)
	{
		if (phaseCounter == 0)
		{
			effect->Creat(position, effect->ENEMY_PHASE2, 1, VGet(100, 100, 100), 100, 4);
			phaseCounter ++;
			hpState = 2;
		}
	}

	if (myHp <= 0)
	{
		VECTOR effPos = VGet(position.x, 50, position.z);
		effect->Creat(effPos, effect->ENEMY_BOON, 1, VGet(1.5, 1.5, 1.5), 100, 6);
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

	DrawRectGraph(hpBar.pos.x, hpBar.pos.y, 0, 0, HP_BAR_X, HP_BAR_Y, hpBar.image, true);//フレーム
	DrawRectGraph(hpBar.pos.x, hpBar.pos.y, 0, HP_BAR_Y * 3, retHp, HP_BAR_Y, hpBar.image, true);//赤ゲージ
	DrawRectGraph(hpBar.pos.x, hpBar.pos.y, 0, HP_BAR_Y * hpState, HP_SIZE + HP_BAR_SIZE * myHp / HP_MAX, HP_BAR_Y, hpBar.image, true);//hp
}

void Enemy::AddVelocity(VECTOR v) 
{
	velocity = VAdd(velocity, v);
}

void Enemy::PlayerData(Player* _player)
{
	player = _player;
}

void Enemy::HitLaser()
{
	int index = 1;
	VECTOR pPos = player->GetPosition();
	float distance = VSize(VSub(position, pPos));

	while (true)
	{
		VECTOR lc = VGet(position.x + sinf(rotation.y) * index, 0, position.z + cosf(rotation.y) * index);
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
