#include "player.h"
#include <assert.h>
#include "../character.h"
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

	SetModel("red");

	position = VGet(-100.0f, 0, -200.0f);
	velocity = VGet(0,0,0);
	
	rotx = 0;
	rotz = 0;

	rotation = VZero;

	hitSound = LoadSoundMem("data\\sound\\hithead.wav");
	state = STATE::ST_STOP;

	isBoost = false;
	boostCounter = 0;
	velocityCopy = VGet(0, 0, 0);

	turnDirection = 0;
	turnIndex = 0;

	myHp = PLAYER_HP_MAX;

	hpBar.image = LoadGraph("data\\texture\\HPBar_player.png");
	hpBar.pos = V2Get(0, SCREEN_HEIGHT- PHP_BAR_SIZE_Y);

	isDead = false;
	isDamage = false;

	hitCounter = 0;
	isAtkDamage = false;

	effect = SceneManager::CommonScene()->FindGameObject<EffectManager>();
	copyPos = VGet(0, 0, 0);

	retHp = PHP_SIZE + PHP_BAR_SIZE;
	hitPos = V2Get(0, 0);

	atkSound = LoadSoundMem("data\\sound\\atkSE_Player.wav");
	hitSound = LoadSoundMem("data\\sound\\atkSE_Enemy.wav");
	jumpSound = LoadSoundMem("data\\sound\\jumpSound.wav");
	boostSound = LoadSoundMem("data\\sound\\boostSound.wav");
	ChangeVolumeSoundMem(200, boostSound);

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
}

void Player::Draw()
{
	if (!isDead)
		CharaBase::Draw();

	DrawRectGraph(hpBar.pos.x, hpBar.pos.y, 0, 0, HP_BAR_X, PHP_BAR_SIZE_Y, hpBar.image, true);//フレーム
	DrawRectGraph(hpBar.pos.x + hitPos.x, hpBar.pos.y + hitPos.y, 0, PHP_BAR_SIZE_Y * 3, retHp, PHP_BAR_SIZE_Y, hpBar.image, true);//赤ゲージ
	DrawRectGraph(hpBar.pos.x + hitPos.x, hpBar.pos.y + hitPos.y, 0, PHP_BAR_SIZE_Y * hpState, PHP_SIZE + PHP_BAR_SIZE * myHp / PLAYER_HP_MAX, PHP_BAR_SIZE_Y, hpBar.image, true);//hp
}

void Player::UpdatePlay()
{
	// すべる床での移動
	velocity = VScale(velocity, 0.9f);

	//ジャンプの処理
	if (input->GetKeyPush().isJump)
	{
		velocity = velocity / 2;
		velocity.y = JUMP_SPEED;
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
		float keyVec = atan2f(input->GetKeyPush().sickLY, input->GetKeyPush().sickLX);
		velocity += CalcMoveVector(camera->GetRotation().y - keyVec + DX_PI_F * 0.5, 0.2f * PLAYER_SPEED);

		goalRotY = camera->GetRotation().y - keyVec + DX_PI_F * 0.5;
		LookAt();
		RunAnim(10.0f);
	}
	else
		RunStopAnim();

	position += velocity;
	position = floor->SetPlayerPos(position);

	CharaBase::Update();
}

void Player::JumpMove()
{
	Reset();
	velocity.y -= JUMP_GRAVITY;
	//地面に着地した時
	if (position.y < 0)
	{
		velocity.y = 0;
		velocity.x = velocity.x / 2;
		velocity.z = velocity.z / 2;

		position.y = 0;
		isBoost = false;//
		state = STATE::ST_PLAY;
	}

	if (isBoost)
	{
		float inputDirection = atan2(_boost.z, _boost.x) + (DX_PI_F * 1.5);

		if (boostCounter == 0)
		{
			velocity += CalcMoveVector(camera->GetRotation().y - inputDirection, BOOST_SPEED);
		}
		else
		{
			velocity += CalcMoveVector(camera->GetRotation().y - inputDirection, 0.1);
			if (boostCounter >= BOOST_SPEED)
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
	position = floor->SetPlayerPos(position);
}

void Player::Atk()
{
	Reset();
	switch (turnIndex)
	{
	case 0:
		turnDirection += ANGULAR_SPEED;
		velocity = VScale(velocity, 0.5);
		if (turnDirection >= ANGULAR_SPEED * 10 + copyRotY)
			turnIndex = 1;
		break;
	case 1:
		turnDirection -= ANGULAR_SPEED * 3;
		VECTOR enemyPos = enemy->GetPosition();
		VECTOR atkDirection = VGet(sinf(copyRotY) * ATK_RANGE * 0.3f , 0, cosf(copyRotY) * ATK_RANGE * 0.3f);//当たり判定を前にずらす数値
		
		if (isHit->_IsHit(VAdd(position, atkDirection), enemyPos, ATK_RANGE * 1.4 ))
		{
			VECTOR target = VSub(enemyPos, position);
			targetDirection = atan2(target.x, target.z) + DX_PI_F / 180;

			velocity.x += sinf(targetDirection) * PLAYER_SPEED * 0.7f;
			velocity.z += cosf(targetDirection) * PLAYER_SPEED * 0.7f;

			isAtkDamage = true;
		}
		
		if (turnDirection <= ANGULAR_SPEED * -10 + copyRotY)
		{	
			if (isAtkDamage)
			{
				if (isHit->_IsHit(VAdd(position, atkDirection), enemyPos, ATK_RANGE))
				{
					enemy->Damage(PLAYER_DAMAGE);
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
	case 2:
		turnDirection += ANGULAR_SPEED;
		if (turnDirection >= copyRotY)
		{
			turnDirection = copyRotY;
			state = STATE::ST_PLAY;
			turnIndex = 0;
		}
		break;
	case 3:
		if(turnCounter == 0)
			velocityCopy = velocity;

		velocity = VGet(0, 0, 0);
		turnCounter++;
		if (turnCounter >= 20)
		{
			turnIndex = 2;
			velocity = velocityCopy;
			turnCounter = 0;
		}
			
	}

	position = VAdd(position, velocity);
	rotation = VECTOR3(0, turnDirection, 0);
	position = floor->SetPlayerPos(position);
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
	else if (hitCounter <= HIT_STOOP)
	{
		velocity.z += cosf(enemy->GetDirection()) * PLAYER_SPEED / 5;
		velocity.x += sinf(enemy->GetDirection()) * PLAYER_SPEED / 5;
		//hitPos = V2Get(0, 0);
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
	if (hitCounter > HIT_STOOP)
	{
		copyPos = VGet(position.x, 50, position.z);
		effect->SetPos(copyPos, 0);
	}

	position = VAdd(position, velocity);
	position = floor->SetPlayerPos(position);
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
			velocity.x = VZERO * cosf(THETA * DX_PI_F / 180) * sinf(enemy->GetDirection());
			velocity.y = VZERO * sinf(THETA * DX_PI_F / 180);
			velocity.z = VZERO * sin(THETA * DX_PI_F / 180) * cos(enemy->GetDirection());
			rotx = 0;
			rotx -= 0.5f * DX_PI_F / (120 * VZERO * sin(THETA * DX_PI_F / 180) / GRAVITY);
		}
		else if (hitCounter <= 120 * VZERO * sinf(THETA * DX_PI_F / 180) / GRAVITY)
		{
			velocity.y -= GRAVITY / 60;
			rotx -= 0.5f * DX_PI_F / (120 * VZERO * sinf(THETA * DX_PI_F / 180) / GRAVITY);
		}
		else
		{
			if (myHp <= 0)
			{
				DeadEff();
			}
			velocity = VGet(0, 0, 0);
		}
		
		if (hitCounter >= 120 * VZERO * sinf(THETA * DX_PI_F / 180) / GRAVITY + 20)
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
		myHp -= 0.08;
		rotx = -THETA * DX_PI_F / 360;
		if (position.y <= 10 || velocity.y > 0)
		{
			velocity = VGet(0, 0, 0);
			position.y = 0;
		}
		velocity.z += cosf(enemy->GetDirection()) * PLAYER_SPEED / 5;
		velocity.x += sinf(enemy->GetDirection()) * PLAYER_SPEED / 5;
	}

	position = VAdd(position, velocity);
	rotation = VECTOR3(rotx, rotation.y, 0);
	position = floor->SetPlayerPos(position);
}

void Player::HitLaser()
{
	state = STATE::ST_LASER;
}

void Player::RetGauge()
{
	float damageCopy = PHP_SIZE + PHP_BAR_SIZE * myHp / PLAYER_HP_MAX;
	if (retHp >= damageCopy)
		retHp-=PHP_SPEED;

	if(myHp / PLAYER_HP_MAX <= 0.3)
		hpState = 3;
	else if (myHp / PLAYER_HP_MAX <= 0.5)
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
		rotation.y += LOOK_SPEED * sign / DX_PI;
	else
		rotation.y = goalRotY;
}