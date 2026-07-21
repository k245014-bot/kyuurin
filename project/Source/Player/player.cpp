#include "player.h"
#include <assert.h>
#include "../Enemy/enemy.h"	
#include "../Floor/floor.h"
#include "../playScene.h"
#include "../isHit.h"
#include "../config.h"
#include "../../Library/sceneManager.h"
#include "../Common/Effect/effectManager.h"
#include "../Common/Imput/inputManager.h"
#include "../Camera/camera.h"

Player::Player(SceneBase* _scene) : CharaBase(_scene)
{
	isHit = GetScene()->FindGameObject<IsHit>();
	enemy = GetScene()->FindGameObject<Enemy>();
	floor = GetScene()->FindGameObject<Floor>();
	input = GetScene()->FindGameObject<InputManager>();
	camera = GetScene()->FindGameObject <Camera>();
	effect = SceneManager::CommonScene()->FindGameObject<EffectManager>();

	SetModel("red");

	atkSound = LoadSoundMem("data\\sound\\atkSE_Player.wav");
	hitSound = LoadSoundMem("data\\sound\\atkSE_Enemy.wav");
	jumpSound = LoadSoundMem("data\\sound\\jumpSound.wav");
	boostSound = LoadSoundMem("data\\sound\\boostSound.wav");
	ChangeVolumeSoundMem(200, boostSound);

	hpBar.image = LoadGraph("data\\texture\\HPBar_player.png");
	hpBar.pos = V2Get(0, SCREEN_HEIGHT - HpBarSizeY);

	state = STATE::ST_STOP;

	position = SpawnPos;
	velocity = VZero;
	
	rotx = 0;
	rotz = 0;

	isBoost = false;
	boostCounter = 0;
	velocityCopy = VZero;

	turnDirection = 0;
	turnIndex = 0;

	myHp = (float)playerHpMax;

	isDead = false;
	isDamage = false;

	hitCounter = 0;
	isAtkDamage = false;

	copyPos = VZero;

	retHp = HpSize + HpBarSize;
	hitPos = V2Get(0, 0);

	turnCounter = 0;
	targetDirection = 0;

	hpState = 1;

	goalRotY = 0;

	copyRotY = 0;
}

Player::~Player()
{
	DeleteSoundMem(hitSound);
}

void Player::Update()
{
	camera->SetPlayerPosition(position);
	switch (state)
	{
	case STATE::ST_STOP:
			break;
		case STATE::ST_PLAY:
			UpdatePlay();
			break;
		case STATE::ST_JUMP:
			JumpMove();
			break;
		case STATE::ST_ATK:
			Atk();
			break;
		case STATE::ST_DAMAGE:
			DamageMove();
			break;
		case STATE::ST_LASER:
			LaserDamage();
			break;
	};

	RetGauge();
	position = floor->SetPlayerPos(position);
}

void Player::Draw()
{
	if (!isDead)
		CharaBase::Draw();

	DrawRectGraph((int)hpBar.pos.x, (int)hpBar.pos.y, 0, 0, (int)HpBarSizeX, (int)HpBarSizeY, hpBar.image, true);//フレーム
	DrawRectGraph((int)hpBar.pos.x + (int)hitPos.x, (int)hpBar.pos.y + (int)hitPos.y, 0, (int)HpBarSizeY * 3, (int)retHp, (int)HpBarSizeY, hpBar.image, true);//赤ゲージ
	DrawRectGraph((int)hpBar.pos.x + (int)hitPos.x, (int)hpBar.pos.y + (int)hitPos.y, 0, (int)HpBarSizeY * (int)hpState, (int)HpSize + HpBarSize * (int)myHp / playerHpMax, (int)HpBarSizeY, hpBar.image, true);//hp
}

void Player::UpdatePlay()
{
	// すべる床での移動
	velocity *= Deceleration;

	//ジャンプの処理
	if (input->GetKeyPush().isJump)
	{
		velocity = velocity / 2;
		velocity.y = JumpSpeed;
		state = STATE::ST_JUMP;
		PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
	}
	//攻撃の処理
	if (input->GetKeyPush().isAtk)
	{
		turnDirection = copyRotY = rotation.y;
		state = STATE::ST_ATK;
	}
	
	//キャラの移動の処理
	if (input->GetKeyPush().sickLX != 0 || input->GetKeyPush().sickLY != 0)
	{
		const float keyVec = atan2f(input->GetKeyPush().sickLY, input->GetKeyPush().sickLX);
		velocity += CalcMoveVector(camera->GetRotation().y - keyVec + AngleOffset, PlayerMoveSpeed);

		goalRotY = camera->GetRotation().y - keyVec + AngleOffset;
		LookAt();
		RunAnim(10.0f);
	}
	else
		RunStopAnim();

	position += velocity;
	/*position = floor->SetPlayerPos(position);*/

	CharaBase::Update();
}

void Player::JumpMove()
{
	Reset();
	velocity.y -= JumpGravity;
	//地面に着地した時
	if (position.y < 0)
	{
		velocity = VECTOR3(velocity.x / 2, 0, velocity.z / 2);
		position.y = 0;
		isBoost = false;//
		state = STATE::ST_PLAY;
	}

	if (isBoost)
	{
		const float InputDirection = atan2f(_boost.z, _boost.x) + InputOffset;

		if (boostCounter == 0)
		{
			velocity += CalcMoveVector(camera->GetRotation().y - InputDirection, BoostSpeed);
		}
		else
		{
			const float _Speed = 0.1f;
			velocity += CalcMoveVector(camera->GetRotation().y - InputDirection, _Speed);
			if (boostCounter >= BoostSpeed)
			{
				isBoost = false;//
				boostCounter = 0;
			}
		}
		boostCounter++;
	}
	else if (input->GetKeyPush().isJump && velocity.y < 0 )
	{
		const float stickParam = 0.25f;
		if (input->GetKeyPush().sickLY > stickParam)
		{
			_boost.z = 1;
		}
		else if (input->GetKeyPush().sickLY < -stickParam)//DOWN
		{
			_boost.z = -1;
		}
		else
			_boost.z = 0;

		if (input->GetKeyPush().sickLX > stickParam)//RIGHT
		{
			_boost.x = 1;
		}
		else if (input->GetKeyPush().sickLX < -stickParam)//LEFT
		{
			_boost.x = -1;
		}
		else
			_boost.x = 0;

		if (_boost.x == 0 && _boost.z == 0)
			_boost.z = 1;

		if (_boost.x != 0 || _boost.z != 0)
			PlaySoundMem(boostSound, DX_PLAYTYPE_BACK);

		isBoost = true;
		boostCounter = 0;
	}
	
	position += velocity;
	//position = floor->SetPlayerPos(position);
}

void Player::Atk()
{
	Reset();
	switch (turnIndex)
	{
	case 0://予備動作
		turnDirection += AngularSpeed;
		velocity *= 0.5f;
		if (turnDirection >= AngleAtkMax + copyRotY)
			turnIndex = 1;
		break;
	case 1:
		turnDirection -= AngularSpeed * 3;
		VECTOR enemyPos = enemy->GetPosition();
		VECTOR atkDirection = VGet(sinf(copyRotY) * AtkRange * 0.3f , 0, cosf(copyRotY) * AtkRange * 0.3f);//当たり判定を前にずらす数値
		
		if (isHit->_IsHit(VAdd(position, atkDirection), enemyPos, AtkRange * 1.4f ))
		{
			VECTOR target = VSub(enemyPos, position);
			targetDirection = atan2f(target.x, target.z) + DX_PI_F / 180;

			velocity += CalcMoveVector(targetDirection, AtkPlayerSpeed);

			isAtkDamage = true;
		}
		
		if (turnDirection <= -AngleAtkMax + copyRotY)
		{	
			if (isAtkDamage)
			{
				if (isHit->_IsHit(VAdd(position, atkDirection), enemyPos, AtkRange))
				{
					enemy->Damage(PlayerDamage);
					PlaySoundMem(atkSound, DX_PLAYTYPE_BACK);
					turnIndex = 3;
				}
				else
					isAtkDamage = false;
			}

			if(!isAtkDamage)
				turnIndex = 2;
		}
		break;
	case 2://元に戻る処理
		turnDirection += AngularSpeed;
		if (turnDirection >= copyRotY)
		{
			turnDirection = copyRotY;
			state = STATE::ST_PLAY;
			turnIndex = 0;
		}
		break;
	case 3://敵にダメージが通った時
		if(turnCounter == 0)
			velocityCopy = velocity;

		velocity = VZero;
		turnCounter++;
		if (turnCounter >= 20)
		{
			turnIndex = 2;
			velocity = velocityCopy;
			turnCounter = 0;
		}
			
	}

	position += velocity;
	rotation = VECTOR3(0, turnDirection, 0);
}

void Player::Damage(int _damage)
{
	if (state == STATE::ST_LASER)
	{
		myHp -= _damage;
		effect->Creat(VGet(position.x, 50, position.z), effect->ENEMYATK_EFFECT, 1, VGet(1.5, 1.5, 1.5), 150, 0);
		PlaySoundMem(hitSound, DX_PLAYTYPE_BACK);
	    
		return;
	}

	Reset();
	myHp -= _damage;
	copyPos = VGet(position.x, 50, position.z);
	state = STATE::ST_DAMAGE;
	isDamage = true;

	if (myHp <= 0)
	{
		DeadEff();
	}
	else
	{
		effect->Creat(copyPos, effect->ENEMYATK_EFFECT, 1, VGet(1.5, 1.5, 1.5), 150, 0);
		PlaySoundMem(hitSound, DX_PLAYTYPE_BACK);
	}
}

void Player::DamageMove()
{
	hitCounter++;
	if (hitCounter <= 2)
	{
		AllHitCharacter(true);

		velocity = VGet(0, 0, 0);
		position.y = 0;
	}
	else if (hitCounter <= 10)
	{
		velocity = VGet(0, 0, 0);
	}
	else if (hitCounter <= HitStoop)
	{
		velocity += CalcMoveVector(enemy->GetRotation().y, DamageMoveSpeed);
	}
	else if (hitCounter <= 40)
	{
		AllHitCharacter(false);
	}
	else
	{
		state = STATE::ST_PLAY;
		hitCounter = 0;
		isDamage = false;///
	}
	if (hitCounter > HitStoop)
	{
		copyPos = VGet(position.x, 50, position.z);
		effect->SetPos(copyPos, 0);
	}

	position += velocity;
}

void Player::LaserDamage()
{
	AllHitCharacter(true);

	if (!effect->IsLaser())
	{
		hitCounter++;
		if (hitCounter == 1)
		{
			myHp -= 5;
			velocity.x = 10 * cosf(Theta * DX_PI_F / 180) * sinf(enemy->GetRotation().y);
			velocity.y = 10 * sinf(Theta * DX_PI_F / 180);
			velocity.z = 10 * sinf(Theta * DX_PI_F / 180) * cosf(enemy->GetRotation().y);
			rotx = 0;
			rotx -= 0.5f * DX_PI_F / (120.0f * 10.0f * sinf(Theta * DX_PI_F / 180.0f) / Gravity);
		}
		else if (hitCounter <= 120 * 10 * sinf(Theta * DX_PI_F / 180) / Gravity)
		{
			velocity.y -= Gravity / 60;
			rotx -= 0.5f * DX_PI_F / (120 * 10 * sinf(Theta * DX_PI_F / 180) / Gravity);
		}
		else
		{
			if (myHp <= 0)
			{
				DeadEff();
			}
			velocity = VGet(0, 0, 0);
		}
		
		if (hitCounter >= 120 * 10 * sinf(Theta * DX_PI_F / 180) / Gravity + 20)
		{
			velocity = VGet(0, 0, 0);
			rotx = 0;
			state = STATE::ST_PLAY;
			hitCounter = 0;

			AllHitCharacter(false);
		}
	}
	else
	{
		myHp -= 0.08f;
		rotx = -Theta * DX_PI_F / 360;
		if (position.y <= 10 || velocity.y > 0)
		{
			velocity = VGet(0, 0, 0);
			position.y = 0;
		}
		velocity.z += cosf(enemy->GetRotation().y) * PlayerSpeed / 5;
		velocity.x += sinf(enemy->GetRotation().y) * PlayerSpeed / 5;
	}

	position += velocity;
	rotation = VECTOR3(rotx, rotation.y, 0);
}

void Player::HitLaser()
{
	state = STATE::ST_LASER;
}

void Player::RetGauge()
{
	float damageCopy = HpSize + HpBarSize * myHp / playerHpMax;
	if (retHp >= damageCopy)
		retHp-=RetHpSpeed;

	if(myHp / playerHpMax <= 0.3)
		hpState = 3;
	else if (myHp / playerHpMax <= 0.5)
		hpState = 2;
}

void Player::DeadEff()
{
	isDead = true;
	VECTOR copyPos = VGet(position.x, 50, position.z);
	effect->Creat(copyPos, effect->PLAYER_DEAD, 1, VGet(2, 2, 2), 100, 3);
	PlaySoundMem(hitSound, DX_PLAYTYPE_BACK);
	state = STATE::ST_STOP;
}

void Player::StartPlay()
{
	state = STATE::ST_PLAY;
}

void Player::StopPlay()
{
	state = STATE::ST_STOP;
}

void Player::LookAt()
{
	//向く方向の計算
	float sign = goalRotY - rotation.y;
	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2;
	const float LOOK_SPEED = 0.2f;

	//実際に向く処理
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	if (sign != 0)
		rotation.y += LOOK_SPEED * sign / DX_PI_F;
	else
		rotation.y = goalRotY;
}