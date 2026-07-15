#include "player.h"
#include <assert.h>
#include "character.h"
#include "enemy.h"	
#include "floor.h"
#include "playScene.h"
#include "isHit.h"
#include "config.h"
#include "../Library/sceneManager.h"
#include "effectManager.h"

Player::Player(SceneBase * _scene) : GameObject(_scene)
{
	chara = new Character();
	chara->SetModel(Character::MODEL_ID::MODEL_BODY, MV1LoadModel("data\\model\\pg_red_body.mv1"));
	chara->SetModel(Character::MODEL_ID::MODEL_HEAD, MV1LoadModel("data\\model\\pg_red_head.mv1"));
	chara->SetModel(Character::MODEL_ID::MODEL_RHAND, MV1LoadModel("data\\model\\pg_red_right_hand.mv1"));
	chara->SetModel(Character::MODEL_ID::MODEL_LHAND, MV1LoadModel("data\\model\\pg_red_left_hand.mv1"));
	chara->SetModel(Character::MODEL_ID::MODEL_RFOOT, MV1LoadModel("data\\model\\pg_red_right_foot.mv1"));
	chara->SetModel(Character::MODEL_ID::MODEL_LFOOT, MV1LoadModel("data\\model\\pg_red_left_foot.mv1"));

	position = VGet(-100.0f, 0, -200.0f);
	velocity = VGet(0,0,0);
	direction = 0;
	
	rotx = 0;
	rotz = 0;
	chara->SetPosition(position);
	chara->SetRotation(VGet(0,direction,0));

	hitSound = LoadSoundMem("data\\sound\\hithead.wav");
	state = STATE::ST_STOP;

	isBoost = false;
	boostCounter = 0;
	velocityCopy = VGet(0, 0, 0);

	keyCheck = true;
	turnDirection = 0;
	turnIndex = 0;

	isHit = GetScene()->FindGameObject<IsHit>();
	enemy = GetScene()->FindGameObject<Enemy>();
	floor = GetScene()->FindGameObject<Floor>();


	myHp = PLAYER_HP_MAX;

	hpBar.image = LoadGraph("data\\texture\\HPBar_player.png");
	hpBar.pos = V2Get(0, SCREEN_HEIGHT- PHP_BAR_SIZE_Y);

	isDead = false;
	isDamage = false;

	hitCounter = 0;
	isAtkDamage = false;

	effect = SceneManager::CommonScene()->FindGameObject<EffectManager>();
	copyPos = VGet(0, 0, 0);


	retHp = PHP_SIZE + PHP_BAR_SIZE;////////////////////
	hitPos = V2Get(0, 0);

	atkSound = LoadSoundMem("data\\sound\\atkSE_Player.wav");
	hitSound = LoadSoundMem("data\\sound\\atkSE_Enemy.wav");
	jumpSound = LoadSoundMem("data\\sound\\jumpSound.wav");
	boostSound = LoadSoundMem("data\\sound\\boostSound.wav");
	ChangeVolumeSoundMem(200, boostSound);

	turnCounter = 0;
	targetDirection = 0;

	hpState = 1;
}

Player::~Player()
{
	DeleteSoundMem(hitSound);
	delete chara;
}

void Player::Update()
{
	GetJoypadXInputState(DX_INPUT_PAD1, & key);
	
	if (CheckHitKey(KEY_INPUT_RIGHT) || key.ThumbRX > 10000)//
	{
		direction += 5.0f * DX_PI_F / 180.0f;
	}
	else if (CheckHitKey(KEY_INPUT_LEFT) || key.ThumbRX < -10000)//
	{
		direction -= 5.0f * DX_PI_F / 180.0f;
	}
	
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
	//chara->Update();
}

void Player::UpdatePlay()
{
	GetJoypadXInputState(DX_INPUT_PAD1, &key);
	// すべる床での移動
	velocity = VScale(velocity, 0.9f);
	
	if (abs(key.ThumbLX) > 10000 || abs(key.ThumbLY) > 10000)
	{
		velocity.z += cosf(direction - atan2(key.ThumbLY, key.ThumbLX) + DX_PI_F * 0.5) * 0.2f * PLAYER_SPEED;
		velocity.x += sinf(direction - atan2(key.ThumbLY, key.ThumbLX) + DX_PI_F * 0.5) * 0.2f * PLAYER_SPEED;
		chara->Run(10.0f);
	}
	else
		chara->Stop();

	if (GetJoypadNum() == 0)
	{
		if (CheckHitKey(KEY_INPUT_UP))
		{
			velocity.z += cosf(direction) * 0.2f * PLAYER_SPEED;
			velocity.x += sinf(direction) * 0.2f * PLAYER_SPEED;
			chara->Run(10.0f);
		}
		else if (CheckHitKey(KEY_INPUT_DOWN))
		{
			velocity.z -= cosf(direction) * PLAYER_SPEED / 10;
			velocity.x -= sinf(direction) * PLAYER_SPEED / 10;
			chara->Run(1.0f);
		}
		else
			chara->Stop();
	}
	
	if (CheckHitKey(KEY_INPUT_Z) || key.Buttons[XINPUT_BUTTON_A])
	{
		velocity.y = JUMP_SPEED;
		keyCheck = false;
		state = STATE::ST_JUMP;
		PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
	}
	else if (CheckHitKey(KEY_INPUT_Z) == 0 || key.Buttons[XINPUT_BUTTON_A] == 0)
	{
		keyCheck = true;
		if ((CheckHitKey(KEY_INPUT_X) || key.Buttons[XINPUT_BUTTON_B]))
		{
			turnDirection = direction;
			state = STATE::ST_ATK;
		}
	}
	
	position = VAdd(position, velocity);
	chara->SetPosition(position);
	chara->SetRotation(VGet(0, direction, 0));
	position = floor->SetPlayerPos(position);
	chara->Update();
}

void Player::JumpMove()
{
	chara->Reset();
	velocity.y -= JUMP_GRAVITY;
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
			velocity.z += cosf(direction - inputDirection) * BOOST_SPEED;
			velocity.x += sinf(direction - inputDirection) * BOOST_SPEED;
		}
		else
		{
			velocity.z -= cosf(direction - inputDirection) * 0.1;
			velocity.x -= sinf(direction - inputDirection) * 0.1;
			if (boostCounter >= BOOST_SPEED)
			{
				isBoost = false;//
				boostCounter = 0;
			}
		}
		boostCounter++;
	}
	else if ((CheckHitKey(KEY_INPUT_Z) || key.Buttons[XINPUT_BUTTON_A] )&& velocity.y < 0 && keyCheck)
	{
		if (CheckHitKey(KEY_INPUT_UP) != 0 || key.ThumbLY > 10000)
		{
			_boost.z = 1;
		}
		else if (CheckHitKey(KEY_INPUT_DOWN) != 0 || key.ThumbLY < -10000)//DOWN
		{
			_boost.z = -1;
		}
		else
			_boost.z = 0;

		if (CheckHitKey(KEY_INPUT_RIGHT) != 0 || key.ThumbLX > 10000)//RIGHT
		{
			_boost.x = 1;
		}
		else if (CheckHitKey(KEY_INPUT_LEFT) != 0 || key.ThumbLX < -10000)//LEFT
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
		//velocity = VGet(0, 0, 0);
		boostCounter = 0;
	}

	if (CheckHitKey(KEY_INPUT_Z) ==0 || abs(key.ThumbLX) < 10000)
		keyCheck = true;

	position = VAdd(position, velocity);
	chara->SetPosition(position);
	position = floor->SetPlayerPos(position);
}

void Player::Atk()
{
	chara->Reset();
	switch (turnIndex)
	{
	case 0:
		turnDirection += ANGULAR_SPEED;
		velocity = VScale(velocity, 0.5);
		if (turnDirection >= ANGULAR_SPEED * 10 + direction)
			turnIndex = 1;
		break;
	case 1:
		turnDirection -= ANGULAR_SPEED * 3;
		VECTOR enemyPos = enemy->GetPosition();
		VECTOR atkDirection = VGet(sinf(direction) * ATK_RANGE * 0.3f , 0, cosf(direction) * ATK_RANGE * 0.3f);//当たり判定を前にずらす数値
		
		if (isHit->_IsHit(VAdd(position, atkDirection), enemyPos, ATK_RANGE * 1.4 ))
		{
			VECTOR target = VSub(enemyPos, position);
			targetDirection = atan2(target.x, target.z) + DX_PI_F / 180;

			velocity.x += sinf(targetDirection) * PLAYER_SPEED * 0.7f;
			velocity.z += cosf(targetDirection) * PLAYER_SPEED * 0.7f;

			isAtkDamage = true;
		}
		
		if (turnDirection <= ANGULAR_SPEED * -10 + direction)
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
		if (turnDirection >= direction)
		{
			turnDirection = direction;
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
	chara->SetPosition(position);
	chara->SetRotation(VGet(0, turnDirection, 0));
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

	chara->Reset();
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
		chara->HitCharacter(Character::MODEL_ID::MODEL_HEAD, 2, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_BODY, 2, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_RHAND, 1, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_LHAND, 1, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_RFOOT, 1, true);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_LFOOT, 1, true);///
		velocity = VGet(0, 0, 0);
		position.y = 0;
		//hitPos = V2Get(20, -20);
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
		chara->HitCharacter(Character::MODEL_ID::MODEL_HEAD, 2, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_BODY, 2, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_RHAND, 1, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_LHAND, 1, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_RFOOT, 1, false);///
		chara->HitCharacter(Character::MODEL_ID::MODEL_LFOOT, 1, false);///
	}
	else
	{
		state = STATE::ST_PLAY;
		hitCounter = 0;
		isDamage = false;///
	}
	//if(hitCounter == HIT_STOOP)
		//effect->Creat(copyPos, effect->ENEMYATK_EFFECT, 1, VGet(1, 1, 1), 200, 0);
	if (hitCounter > HIT_STOOP)
	{
		copyPos = VGet(position.x, 50, position.z);
		effect->SetPos(copyPos, 0);
	}


	position = VAdd(position, velocity);
	chara->SetPosition(position);
	position = floor->SetPlayerPos(position);
}

void Player::LaserDamage()
{
	chara->HitCharacter(Character::MODEL_ID::MODEL_HEAD, 2, true);///
	chara->HitCharacter(Character::MODEL_ID::MODEL_BODY, 2, true);///
	chara->HitCharacter(Character::MODEL_ID::MODEL_RHAND, 1, true);///
	chara->HitCharacter(Character::MODEL_ID::MODEL_LHAND, 1, true);///
	chara->HitCharacter(Character::MODEL_ID::MODEL_RFOOT, 1, true);///
	chara->HitCharacter(Character::MODEL_ID::MODEL_LFOOT, 1, true);///

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
			chara->HitCharacter(Character::MODEL_ID::MODEL_HEAD, 2, false);///
			chara->HitCharacter(Character::MODEL_ID::MODEL_BODY, 2, false);///
			chara->HitCharacter(Character::MODEL_ID::MODEL_RHAND, 1, false);///
			chara->HitCharacter(Character::MODEL_ID::MODEL_LHAND, 1, false);///
			chara->HitCharacter(Character::MODEL_ID::MODEL_RFOOT, 1, false);///
			chara->HitCharacter(Character::MODEL_ID::MODEL_LFOOT, 1, false);///
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
	chara->SetPosition(position);
	chara->SetRotation(VGet(rotx, direction, 0));
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

void Player::Draw()
{
	if (!isDead)
		chara->Draw();

	DrawRectGraph(hpBar.pos.x, hpBar.pos.y, 0, 0, HP_BAR_X, PHP_BAR_SIZE_Y, hpBar.image, true);//フレーム
	DrawRectGraph(hpBar.pos.x + hitPos.x, hpBar.pos.y + hitPos.y, 0, PHP_BAR_SIZE_Y * 3, retHp, PHP_BAR_SIZE_Y, hpBar.image, true);//赤ゲージ
	DrawRectGraph(hpBar.pos.x + hitPos.x, hpBar.pos.y + hitPos.y, 0, PHP_BAR_SIZE_Y * hpState, PHP_SIZE + PHP_BAR_SIZE * myHp / PLAYER_HP_MAX, PHP_BAR_SIZE_Y, hpBar.image, true);//hp
}

void Player::StartPlay()
{
	state = STATE::ST_PLAY;
}

void Player::StopPlay()
{
	state = STATE::ST_STOP;
}
